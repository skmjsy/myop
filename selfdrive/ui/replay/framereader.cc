#include "selfdrive/ui/replay/framereader.h"

#include <cassert>
#include "libyuv.h"

namespace {

struct buffer_data {
  const uint8_t *data;
  int64_t offset;
  size_t size;
};

int readPacket(void *opaque, uint8_t *buf, int buf_size) {
  struct buffer_data *bd = (struct buffer_data *)opaque;
  buf_size = std::min((size_t)buf_size, bd->size - bd->offset);
  if (!buf_size) return AVERROR_EOF;

  memcpy(buf, bd->data + bd->offset, buf_size);
  bd->offset += buf_size;
  return buf_size;
}

enum AVPixelFormat get_hw_format(AVCodecContext *ctx, const enum AVPixelFormat *pix_fmts) {
  enum AVPixelFormat *hw_pix_fmt = reinterpret_cast<enum AVPixelFormat *>(ctx->opaque);
  for (const enum AVPixelFormat *p = pix_fmts; *p != -1; p++) {
    if (*p == *hw_pix_fmt) return *p;
  }
  printf("Please run replay with the --no-cuda flag!\n");
  assert(0);
  return AV_PIX_FMT_NONE;
}

}  // namespace

FrameReader::FrameReader(bool local_cache, int chunk_size, int retries) : FileReader(local_cache, chunk_size, retries) {
  input_ctx = avformat_alloc_context();
  sws_frame.reset(av_frame_alloc());
}

FrameReader::~FrameReader() {
  for (auto &f : frames_) {
    av_packet_unref(&f.pkt);
  }

  if (decoder_ctx) avcodec_free_context(&decoder_ctx);
  if (input_ctx) avformat_close_input(&input_ctx);
  if (hw_device_ctx) av_buffer_unref(&hw_device_ctx);

  if (rgb_sws_ctx_) sws_freeContext(rgb_sws_ctx_);
  if (yuv_sws_ctx_) sws_freeContext(yuv_sws_ctx_);

  if (avio_ctx_) {
    av_freep(&avio_ctx_->buffer);
    avio_context_free(&avio_ctx_);
  }
}

bool FrameReader::load(const std::string &url, bool no_cuda, std::atomic<bool> *abort) {
  std::string content = read(url, abort);
  if (content.empty()) return false;

  struct buffer_data bd = {
    .data = (uint8_t *)content.data(),
    .offset = 0,
    .size = content.size(),
  };
  const int avio_ctx_buffer_size = 64 * 1024;
  unsigned char *avio_ctx_buffer = (unsigned char *)av_malloc(avio_ctx_buffer_size);
  avio_ctx_ = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, readPacket, nullptr, nullptr);
  input_ctx->pb = avio_ctx_;

  input_ctx->probesize = 10 * 1024 * 1024;  // 10MB
  int ret = avformat_open_input(&input_ctx, url.c_str(), NULL, NULL);
  if (ret != 0) {
    char err_str[1024] = {0};
    av_strerror(ret, err_str, std::size(err_str));
    printf("Error loading video - %s - %s\n", err_str, url.c_str());
    return false;
  }

  ret = avformat_find_stream_info(input_ctx, nullptr);
  if (ret < 0) {
    printf("cannot find a video stream in the input file\n");
    return false;
  }

  AVStream *video = input_ctx->streams[0];
  AVCodec *decoder = avcodec_find_decoder(video->codec->codec_id);
  if (!decoder) return false;

  decoder_ctx = avcodec_alloc_context3(decoder);
  ret = avcodec_parameters_to_context(decoder_ctx, video->codecpar);
  if (ret != 0) return false;

  width = (decoder_ctx->width + 3) & ~3;
  height = decoder_ctx->height;

  if (!no_cuda) {
    if (!initHardwareDecoder(AV_HWDEVICE_TYPE_CUDA)) {
      printf("No CUDA capable device was found. fallback to CPU decoding.\n");
    }
  }

  rgb_sws_ctx_ = sws_getContext(decoder_ctx->width, decoder_ctx->height, sws_src_format,
                                width, height, AV_PIX_FMT_BGR24,
                                SWS_BILINEAR, NULL, NULL, NULL);
  if (!rgb_sws_ctx_) return false;
  yuv_sws_ctx_ = sws_getContext(decoder_ctx->width, decoder_ctx->height, sws_src_format,
                                width, height, AV_PIX_FMT_YUV420P,
                                SWS_BILINEAR, NULL, NULL, NULL);
  if (!yuv_sws_ctx_) return false;

  ret = avcodec_open2(decoder_ctx, decoder, NULL);
  if (ret < 0) return false;

  frames_.reserve(60 * 20);  // 20fps, one minute
  while (!(abort && *abort)) {
    Frame &frame = frames_.emplace_back();
    ret = av_read_frame(input_ctx, &frame.pkt);
    if (ret < 0) {
      frames_.pop_back();
      valid_ = (ret == AVERROR_EOF);
      break;
    }
    // some stream seems to contian no keyframes
    key_frames_count_ += frame.pkt.flags & AV_PKT_FLAG_KEY;
  }
  return valid_;
}

bool FrameReader::initHardwareDecoder(AVHWDeviceType hw_device_type) {
  for (int i = 0;; i++) {
    const AVCodecHWConfig *config = avcodec_get_hw_config(decoder_ctx->codec, i);
    if (!config) {
      printf("decoder %s does not support hw device type %s.\n",
             decoder_ctx->codec->name, av_hwdevice_get_type_name(hw_device_type));
      return false;
    }
    if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX && config->device_type == hw_device_type) {
      hw_pix_fmt = config->pix_fmt;
      break;
    }
  }

  int ret = av_hwdevice_ctx_create(&hw_device_ctx, hw_device_type, nullptr, nullptr, 0);
  if (ret < 0) {
    printf("Failed to create specified HW device %d.\n", ret);
    return false;
  }

  // get sws source format
  AVHWFramesConstraints *hw_frames_const = av_hwdevice_get_hwframe_constraints(hw_device_ctx, nullptr);
  assert(hw_frames_const != 0);
  for (AVPixelFormat *p = hw_frames_const->valid_sw_formats; *p != AV_PIX_FMT_NONE; p++) {
    if (sws_isSupportedInput(*p)) {
      sws_src_format = *p;
      break;
    }
  }
  av_hwframe_constraints_free(&hw_frames_const);

  decoder_ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
  decoder_ctx->opaque = &hw_pix_fmt;
  decoder_ctx->get_format = get_hw_format;
  return true;
}

bool FrameReader::get(int idx, uint8_t *rgb, uint8_t *yuv) {
  assert(rgb || yuv);
  if (!valid_ || idx < 0 || idx >= frames_.size()) {
    return false;
  }
  return decode(idx, rgb, yuv);
}

bool FrameReader::decode(int idx, uint8_t *rgb, uint8_t *yuv) {
  auto get_keyframe = [=](int idx) {
    for (int i = idx; i >= 0 && key_frames_count_ > 1; --i) {
      if (frames_[i].pkt.flags & AV_PKT_FLAG_KEY) return i;
    }
    return idx;
  };

  int from_idx = idx;
  if (idx > 0 && !frames_[idx].decoded && !frames_[idx - 1].decoded) {
    // find the previous keyframe
    from_idx = get_keyframe(idx);
  }

  for (int i = from_idx; i <= idx; ++i) {
    Frame &frame = frames_[i];
    if ((!frame.decoded || i == idx) && !frame.failed) {
      AVFrame *f = decodeFrame(&frame.pkt);
      frame.decoded = f != nullptr;
      frame.failed = !frame.decoded;
      if (frame.decoded && i == idx) {
        return copyBuffers(f, rgb, yuv);
      }
    }
  }
  return false;
}

AVFrame *FrameReader::decodeFrame(AVPacket *pkt) {
  int ret = avcodec_send_packet(decoder_ctx, pkt);
  if (ret < 0) {
    printf("Error sending a packet for decoding\n");
    return nullptr;
  }

  av_frame_.reset(av_frame_alloc());
  ret = avcodec_receive_frame(decoder_ctx, av_frame_.get());
  if (ret != 0) {
    return nullptr;
  }

  if (av_frame_->format == hw_pix_fmt) {
    hw_frame.reset(av_frame_alloc());
    if ((ret = av_hwframe_transfer_data(hw_frame.get(), av_frame_.get(), 0)) < 0) {
      printf("error transferring the data from GPU to CPU\n");
      return nullptr;
    }
    return hw_frame.get();
  } else {
    return av_frame_.get();
  }
}

bool FrameReader::copyBuffers(AVFrame *f, uint8_t *rgb, uint8_t *yuv) {
  if (yuv) {
    if (sws_src_format == AV_PIX_FMT_NV12) {
      // libswscale crash if height is not 16 bytes aligned for NV12->YUV420 conversion
      assert(sws_src_format == AV_PIX_FMT_NV12);
      uint8_t *u = yuv + width * height;
      uint8_t *v = u + (width / 2) * (height / 2);
      libyuv::NV12ToI420(f->data[0], f->linesize[0],
                         f->data[1], f->linesize[1],
                         yuv, width,
                         u, width / 2,
                         v, width / 2,
                         width, height);
    } else {
      av_image_fill_arrays(sws_frame->data, sws_frame->linesize, yuv, AV_PIX_FMT_YUV420P, width, height, 1);
      int ret = sws_scale(yuv_sws_ctx_, (const uint8_t **)f->data, f->linesize, 0, f->height, sws_frame->data, sws_frame->linesize);
      if (ret < 0) return false;
    }
  }

  // images is going to be written to output buffers, no alignment (align = 1)
  av_image_fill_arrays(sws_frame->data, sws_frame->linesize, rgb, AV_PIX_FMT_BGR24, width, height, 1);
  int ret = sws_scale(rgb_sws_ctx_, (const uint8_t **)f->data, f->linesize, 0, f->height, sws_frame->data, sws_frame->linesize);
  return ret >= 0;
}
