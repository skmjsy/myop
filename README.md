![boltpilot](https://i.imgur.com/azNrGqZ.gif)

Table of Contents
=======================

* [What is boltpilot?](#what-is-boltpilot)
* [Hardware requirements](#hardware-requirements)
* [Hardware installation](#hardware-installation)
* [Software installation](#software-installation)
* [Boltpilot usage](#usage)
* [Donations](#donations)
* [Community and Contributing](#community-and-contributing)
* [User Data and comma Account](#user-data-and-comma-account)
* [Safety and Testing](#safety-and-testing)
* [Directory Structure](#directory-structure)
* [Licensing](#licensing)

---


What is boltpilot?
------

This "boltpilot" fork is a community supported fork of the open source driver assistance system called openpilot. Although the Bolt EV is not currently supported by Comma.ai in the main openpilot release, this customized fork fills that gap. Boltpilot was designed spefically for Chevrolet Bolt EV models that lack ACC (Adaptive Cruise Control). Compatibility has been verified with 2017, 2018 and 2019 Bolt EV models with lane keep assist. More recent year models might also be compatible but have not yet been tested.

Currently, boltpilot performs the functions of Adaptive Cruise Control (ACC) and Automated Lane Centering (ALC). It will control your steering wheel to keep your vehicle centred in the lane and can optionally also control your vehicle's speed. The system does not offer full self-driving capabilities and will not, for example, stop for red lights or stop signs, although some of these features are currently under development by Comma.ai. The driver must remain alert and able to take control at all times. 

This is an experimental application used for research and development. It is not a product and carries no warranty exporessed or implied. Use at your own risk. Issue reports are welcome via github, or contact kjnice@gmail.com.


Hardware requirements
------

This Bolt-specific fork requires a Comma 3 device (https://comma.ai/shop), a harness box (https://comma.ai/shop/products/harness-box), a custom wiring harness made for GM vehicles (make your own or purchase one online), and a pedal interceptor designed specifically for GM vehicles (make your own or purchase one online). 

Making a custom Bolt EV wiring harness and pedal interceptor requires specialized equipment and considerable skill. However, both of these devices are currently available for purchase from BearTechWorkshop's Etsy store (https://www.etsy.com/ca/shop/BearTechWorkshop).

Earlier comma devices including the Comma 2 and Eon are not supported by the actively maintained branches of this fork. If you intend on using a Comma 2 or Eon you must use the comma2_Eon-final branch, and you may need additional hardware as described in the Chevy-Bolt openpilot Wiki (https://github.com/commaai/openpilot/wiki/Chevy-Bolt).


Hardware installation
------

A video showing installation of a Comma 2 into a Bolt EV can be found at https://www.youtube.com/watch?v=5D21wzCcycE. Installation of the Comma 3 requires the same basic procedure.

A video showing installation of a pedal interceptor on a Bolt EV can be found here: https://www.youtube.com/watch?v=wLepOnjGoh8

Thanks to Jason Shuler of Stand Back Labs for making these videos available.


Software installation
------

1. Select your WiFi network and enter Wi-Fi password on your Comma 3 device.

2. When asked to choose between Dashcam and Custom Software, choose Custom Software and enter the URL https://smiskol.com/fork/jc01rho-openpilot-BoltEV2019-KoKr/[branch] where [branch] is the desired branch name. For example, to install the 'develop' branch, enter https://smiskol.com/fork/jc01rho-openpilot-BoltEV2019-KoKr/develop. The 'develop' branch is currently recommended for most purposes as it is actively maintained and contains the most up-to-date changes and features. You can also use the shortcut https://tiny.one/boltpilot-develop to install the 'develop' branch. This will automatically redirect to the longer URL listed above.

3. Your device will reboot after installation. When prompted, scroll to accept the terms and conditions, then follow the on-screen instructions to complete the openpilot training.

4. Click the Gear icon to open settings. Under the 'Toggles' menu, ensure 'Enable openpilot' and 'Use Metric System' toggles are enabled. This fork is not compatible with imperial measures. 

5. Under the 'Vehicle' settings, ensure the 'Use Comma Pedal' toggle is enabled. The two buttons at the top of the Vehicle settings are used to set steering control and vehicle settings. Click the top right button and select 'Chevrolet Bolt EV' for your vehicle. Next, click the top left button and select the type of steering control you wish to use. Each option represents a a different mathematical approach for steering control calculations. Good results have been reported with the PID, TORQUE and LQR options. The INDI option will soon be phased out. You may wish to experiment with all steering control options to determine which one produces the best results on your vehicle. Reboot your device by selecting the 'Device' menu, and then press the 'Reboot' button. Important: you must reboot your device after changing steering control options for the settings to take effect. 


Boltpilot usage
------

Calibration: Your device requires a one-time calibration after software installation. This takes only a couple minutes and will occur automatically at the beginning of your first drive.

Gear selection: This fork is designed for use in L-mode only. You must place the gear shifter in L. Your vehicle's regenerative braking will be used to lower the speed when requried.

Bolt pilot has two main operation modes: (1) full control mode, which controls both steering and speed, and (2) lateral control mode, which controls only the steering.

Full control mode: To engage both steering and speed control, make sure the stock cruise control is turned off, and then while driving press the Set/- button (bottom button on the left steering wheel control pad). Pressing the X (left) keypad button or the brake pedal will disengage openpilot. 

Lateral control mode: Use the stock cruise control button (right button on steering wheel keypad) to toggle steering control only. 


Acknowledgments
------

This fork is maintained by @whrho (Discord user @jc01rho) of Korea (kjnice@gmail.com). Special thanks to Jason Shuler of Stand Back Labs for his extensive openpilot development work on GM vehicles--especially the Bolt EV, GM giraffe, GM harness, GM Pedal, pedal firmware, and panda coding. Without his work the creation of this fork would not have been possible. Starting with version 0.8.14 this fork is based on the work of @neokii with the Hyundai-Kia fork. Thanks to hanabi95 (@hanabi95) for safety and CAN bus related content. Thank you @Hammie K for the lat_icon_image, and @neokii for the screenrecorder.


Donations
------

If you find this fork useful, please consider donating to support the continued development and maintenance. Donations can be made at https://jc01rho.com/donation


# 볼트EV + 콤마3 전용 오픈파일럿 포크

### 콤마2 혹은 lepro 이온 사용자분들은 comma2_Eon-final을 사용하세요.
### 일반 사용자분들은 "develop" 혹은 "release" 브랜치를 사용하세요. 버전이 올라가도 최신을 그곳에 유지하겠습니다.
### 0.8.14부터 @neokii님의 HKG향 코드를 기반으로 하고있습니다. 항상 감사드립니다.
### pedal 사용자의 경우 옵션에서 켜주셔야합니다 꼭.

- hanabi95(@hanabi95) 님의 작업에서 safety 및 can 관련 내용을 기본으로 하고 있습니다. 항상 감사드립니다.
- lat_icon_image, thanks to @Hammie K 
- screenrecorder, thanks to neokii

### 문의사항은 오카방 혹은 이슈 생성으로 주십시오. 감사합니다.
working tested : BoltEV premier 2017, 2018, 2019
rkjnice@gmail.com


What is openpilot?
------

[openpilot](http://github.com/commaai/openpilot) is an open source driver assistance system. Currently, openpilot performs the functions of Adaptive Cruise Control (ACC), Automated Lane Centering (ALC), Forward Collision Warning (FCW) and Lane Departure Warning (LDW) for a growing variety of [supported car makes, models and model years](docs/CARS.md). In addition, while openpilot is engaged, a camera based Driver Monitoring (DM) feature alerts distracted and asleep drivers. See more about [the vehicle integration](docs/INTEGRATION.md) and [limitations](docs/LIMITATIONS.md).

<table>
  <tr>
    <td><a href="https://youtu.be/NmBfgOanCyk" title="Video By Greer Viau"><img src="https://i.imgur.com/1w8c6d2.jpg"></a></td>
    <td><a href="https://youtu.be/VHKyqZ7t8Gw" title="Video By Logan LeGrand"><img src="https://i.imgur.com/LnBucik.jpg"></a></td>
    <td><a href="https://youtu.be/VxiR4iyBruo" title="Video By Charlie Kim"><img src="https://i.imgur.com/4Qoy48c.jpg"></a></td>
    <td><a href="https://youtu.be/-IkImTe1NYE" title="Video By Aragon"><img src="https://i.imgur.com/04VNzPf.jpg"></a></td>
  </tr>
  <tr>
    <td><a href="https://youtu.be/iIUICQkdwFQ" title="Video By Logan LeGrand"><img src="https://i.imgur.com/b1LHQTy.jpg"></a></td>
    <td><a href="https://youtu.be/XOsa0FsVIsg" title="Video By PinoyDrives"><img src="https://i.imgur.com/6FG0Bd8.jpg"></a></td>
    <td><a href="https://youtu.be/bCwcJ98R_Xw" title="Video By JS"><img src="https://i.imgur.com/zO18CbW.jpg"></a></td>
    <td><a href="https://youtu.be/BQ0tF3MTyyc" title="Video By Tsai-Fi"><img src="https://i.imgur.com/eZzelq3.jpg"></a></td>
  </tr>
</table>


Community and Contributing
------

openpilot is developed by [comma](https://comma.ai/) and by users like you. We welcome both pull requests and issues on [GitHub](http://github.com/commaai/openpilot). Bug fixes and new car ports are encouraged. Check out [the contributing docs](docs/CONTRIBUTING.md).

Documentation related to openpilot development can be found on [docs.comma.ai](https://docs.comma.ai). Information about running openpilot (e.g. FAQ, fingerprinting, troubleshooting, custom forks, community hardware) should go on the [wiki](https://github.com/commaai/openpilot/wiki).

You can add support for your car by following guides we have written for [Brand](https://blog.comma.ai/how-to-write-a-car-port-for-openpilot/) and [Model](https://blog.comma.ai/openpilot-port-guide-for-toyota-models/) ports. Generally, a car with adaptive cruise control and lane keep assist is a good candidate. [Join our Discord](https://discord.comma.ai) to discuss car ports: most car makes have a dedicated channel.

Want to get paid to work on openpilot? [comma is hiring](https://comma.ai/jobs/).

And [follow us on Twitter](https://twitter.com/comma_ai).

User Data and comma Account
------

By default, openpilot uploads the driving data to comma.ai's servers. You can also access your data through [comma connect](https://connect.comma.ai/). We use your data to train better models and improve openpilot for everyone.

openpilot is open source software: the user is free to disable data collection if they wish to do so.

openpilot logs the road facing cameras, CAN, GPS, IMU, magnetometer, thermal sensors, crashes, and operating system logs.
The driver facing camera is only logged if you explicitly opt-in in settings. The microphone is not recorded.

By using openpilot, you agree to [our Privacy Policy](https://comma.ai/privacy). You understand that use of this software or its related services will generate certain types of user data, which may be logged and stored at the sole discretion of comma. By accepting this agreement, you grant an irrevocable, perpetual, worldwide right to comma for the use of this data.

Safety and Testing
----

* openpilot observes ISO26262 guidelines, see [SAFETY.md](docs/SAFETY.md) for more details.
* openpilot has software in the loop [tests](.github/workflows/selfdrive_tests.yaml) that run on every commit.
* The code enforcing the safety model lives in panda and is written in C, see [code rigor](https://github.com/commaai/panda#code-rigor) for more details.
* panda has software in the loop [safety tests](https://github.com/commaai/panda/tree/master/tests/safety).
* Internally, we have a hardware in the loop Jenkins test suite that builds and unit tests the various processes.
* panda has additional hardware in the loop [tests](https://github.com/commaai/panda/blob/master/Jenkinsfile).
* We run the latest openpilot in a testing closet containing 10 comma devices continuously replaying routes.

Directory Structure
------
    .
    ├── cereal              # The messaging spec and libs used for all logs
    ├── common              # Library like functionality we've developed here
    ├── docs                # Documentation
    ├── opendbc             # Files showing how to interpret data from cars
    ├── panda               # Code used to communicate on CAN
    ├── third_party         # External libraries
    ├── pyextra             # Extra python packages
    └── system              # Generic services
        ├── logcatd         # systemd journal as a service
        └── proclogd        # Logs information from /proc
    └── selfdrive           # Code needed to drive the car
        ├── assets          # Fonts, images, and sounds for UI
        ├── athena          # Allows communication with the app
        ├── boardd          # Daemon to talk to the board
        ├── camerad         # Driver to capture images from the camera sensors
        ├── car             # Car specific code to read states and control actuators
        ├── common          # Shared C/C++ code for the daemons
        ├── controls        # Planning and controls
        ├── debug           # Tools to help you debug and do car ports
        ├── locationd       # Precise localization and vehicle parameter estimation
        ├── loggerd         # Logger and uploader of car data
        ├── modeld          # Driving and monitoring model runners
        ├── proclogd        # Logs information from proc
        ├── sensord         # IMU interface code
        ├── navd            # Turn-by-turn navigation
        ├── test            # Unit tests, system tests, and a car simulator
        └── ui              # The UI

Licensing
------

openpilot is released under the MIT license. Some parts of the software are released under other licenses as specified.

Any user of this software shall indemnify and hold harmless Comma.ai, Inc. and its directors, officers, employees, agents, stockholders, affiliates, subcontractors and customers from and against all allegations, claims, actions, suits, demands, damages, liabilities, obligations, losses, settlements, judgments, costs and expenses (including without limitation attorneys’ fees and costs) which arise out of, relate to or result from any use of this software by user.

**THIS IS ALPHA QUALITY SOFTWARE FOR RESEARCH PURPOSES ONLY. THIS IS NOT A PRODUCT.
YOU ARE RESPONSIBLE FOR COMPLYING WITH LOCAL LAWS AND REGULATIONS.
NO WARRANTY EXPRESSED OR IMPLIED.**

---

<img src="https://d1qb2nb5cznatu.cloudfront.net/startups/i/1061157-bc7e9bf3b246ece7322e6ffe653f6af8-medium_jpg.jpg?buster=1458363130" width="75"></img> <img src="https://cdn-images-1.medium.com/max/1600/1*C87EjxGeMPrkTuVRVWVg4w.png" width="225"></img>

[![openpilot tests](https://github.com/commaai/openpilot/workflows/openpilot%20tests/badge.svg?event=push)](https://github.com/commaai/openpilot/actions)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/commaai/openpilot.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/commaai/openpilot/alerts/)
[![Language grade: Python](https://img.shields.io/lgtm/grade/python/g/commaai/openpilot.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/commaai/openpilot/context:python)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/commaai/openpilot.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/commaai/openpilot/context:cpp)
[![codecov](https://codecov.io/gh/commaai/openpilot/branch/master/graph/badge.svg)](https://codecov.io/gh/commaai/openpilot)
