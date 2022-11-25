#!/usr/bin/bash

GET_PROP_ATZ=$(getprop persist.sys.timezone)
if [ -f "/data/params/d/OPKRTimeZone" ]; then
    GET_PROP_STZ=$(cat /data/params/d/OPKRTimeZone)
fi

if [ "$GET_PROP_STZ" != "" ] && [ "$GET_PROP_ATZ" != "$GET_PROP_STZ" ]; then
    setprop persist.sys.timezone $GET_PROP_STZ
fi

if [ -f "/data/bootanimation.zip" ]; then
    DIFF=$(diff /data/bootanimation.zip /system/media/bootanimation.zip)
    if [ "$DIFF" != "" ]; then
        sleep 3
        mount -o remount,rw /system
        cp -f /data/bootanimation.zip /system/media/bootanimation.zip
        chmod 644 /system/media/bootanimation.zip
        mount -o remount,r /system
    fi
fi

export PASSIVE="0"
exec ./launch_chffrplus.sh

