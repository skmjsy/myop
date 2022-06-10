0. This fork is for all people who love Comma's Openpilot. Thanks to ku7, xx979xx, tk211x, xps-genesis, atom, hoya, moksatang, mamul, neokii, oricialworks, sunnyhaibin, dragonpilot, shane, kegman, dnv26, move-fast and everyone helping me or contributing for HKGs.

1. Branches
 - OPKR: main branch, stable, not latest. This will be updated if test branch is done.
 - _test: test branches, not stable, latest, for testing new functions, codes, or the other things.
 - Old branches are in openpilot_bak repository.

2. How to Install
 - Use fork installer : Type https://opkr.tk/fork/opkr on custom URL window(Short URL. This will install OPKR branch directly.) or you can use Shane's fork installer(https://smiskol.com/fork)
 - Use a command : cd /data; mv openpilot openpilot_bak; git clone https://github.com/openpilotkr/openpilot.git -b OPKR; reboot

3. Setting Menu
 - Device(Function Name: Explanation)
   - Driving Camera: You can preview/unview Openpilot Driving Camera.
 - Network(Function Name: Explanation)
   - HotSpot on Boot: Turn on Hotspot when boot. (reboot required)
   - Use Legacy SSH Key: Use old ssh key access(below 0.8.2). (no reboot required)
 - Toggles(Function Name: Explanation)
   - Enable Lane selector Mode: Show a lane mode button on driving screen Laneline/LaneLess/AUTO. AUTO will automatically switch based on presents of lane marker. (no reboot required)
   - Enable Driver Monitoring: On/Off driver monitoring for the EON without filterless IR camera or Someone cannot use front cam due to certain reasons.(reboot required)
   - Enable Driving Log Record: Record driving logs to Local, not to online server. (reboot required)
   - Enable Sending Log to Server: Enable log upload to online server. (reboot required)
   - Use Comma Stock UI: this use original Comma's UI. Also this can be applied on driving screen in realtime (click MaxSpeed box at top-left corner). (no reboot required)
 - Software(Function Name: Explanation)
   - Check for Updates: You can confirm new commits of your fork, press ok will update and reboot.(like git pull).
   - Commit(Local/Remote): Commit name of local(EON) and Remote.(run once when boot in manager.py, search gitcommit.sh at the file, internet connection required)
   - Git Pull On Boot: run 'git pull' command when boot.
   - Load Preset/Save Preset: Load your Parameters or Save Your Parameters. located /data/preset1 or /data/preset2. This function can save/load your settings)
   - Parameter Init: Remove your settings changes and restore initial values.
   - Git Reset: Remove your local changes and inintalize to the original status of the branch.
   - Cancel Git Pull: Move back to previous version of fork if last update is not desired.
   - Panda Flashing: Run Panda flashing command manually. Basically this is not necessary on normal operation.
   - Change Repo/Branch: You can install others fork/branch thru typing Git ID, Git Repository, Git Branch.
 - UserMenu(Function Name: Explanation)
   - EON AutoShutdown: When car ignition is turned off, the device will be shutdown after the set time.
   - EON ForceShutdown: The device will be shutdown by force at offroad status after set time.
   - EON Volume Control(%): set device volume manually.
   - EON Brightness Control(%): set device brightness automatically or manually.
   - EON SCR Off Timer: The Brightness at SCR Off(%) will be in effect after set time when driving.
   - Brightness at SCR Off(%): Work with (EON SCR Off Timer) setting, set screen brightness level %.
   - EON Detach Alert Sound: None/KOR/ENG, Will play alert sound when your car ignition is turned off. Can be used as a reminder to        remove device from mount to protect from sun, theft, etc in case you forgot.
   - Enable Battery Charging Control: battery charging control, min and max eon battery setting, not applicable for batteryless unit.
   - Use Auto Screen Record: At departure, screen recording starts automatically, when vehicle is stationary, screen recording stop.
   - Number of Recorded Files: Max mp4 files that will be recorded before oldest file is overwritten. (to prevent device storage full)
   - Recording Quality: Low/Mid/High/U-High, Video quality of recordings.
   - Delete All Recorded Files: from device /sdcard/videos
   - Delete All Driving Logs: from device/sdcard/realdata
   - Driver Monitoring Mode: Defalut/Unsleep, Default is Comma's. If you choose Unsleep, dm will be more sensitive than default. You can switch the Mode on driving screen in realtime (touch monitoring face at bottom-left corner), no background is Default Mode. Light green background is Unsleep Mode. (no reboot required)
   - E2E EYE Threshold: Experimental setting.
   - Normal EYE Threshold: set the value below threshold of your face recognition.
   - Blink Threshold: For Driver Monitoring. Set the value below the threshold of your eyes blink recognition. Driver Monitoring camera shows the values of your face recognition, eyes and the other things. Preview 'Driver Camera' and then check the recognition value of your eye blink to modify the value on menu.
   - Navigation Select: Mappy(for Korea), Waze(for Global)
   - RUN Navigation on Boot: Run your selected Mappy or Waze on boot. If it runs well, will go to background after few seconds.
   - Display Date on Screen: shows the device date
   - Display Time on Screen: shows the device time
   - API Server: Choose driver log server, Retropilot, Comma, User's
   - User's API: Set User driver log server url.
   - Mapbox Style: Choose three styles of the Mapbox, Comma, OPKR(locallized in Korea), User's, if you want to your own, Edit the file with yours(/data/params/d/MapboxStyleCustom). Make your mapbox style at https://studio.mapbox.com/. If you publish the style you can use it.
   - Use Auto Resume at Stop: after standstill, op will auto resume when leadcar start moving.
   - Use Cruise Button Spamming: SCC set speed is changed up and down automatically. turn on to use many functions related to auto speed control.
   - Cruise Start Mode: Set your custom Cruise mode when boot. There are 6 modes. OpenpilotStock/Dist+Curv/Dist/Curv/Oneway/CamSpeed only. OpenpilotStock : Scc button will set scc speed, will work like stock button to set op. Dist+Curv is changed by distance to leadcar and curvature. Dist is distance only. Curv is curvature only. Oneway change camera offset to approach the edge of a road. CamSpeed is changing set speed only by value of speed sign (OSM, Mappy).
   - LaneChange Speed: minimum lane change speed
   - LaneChange Delay: Nudge/Nudgeless (adjust delay time before lane change)
   - LaneChange Time(km/h: value): How quick should lane change be completed, faster, increase the value, decrease for slower.
   - LeftCurv Offset: if you are not satisfy with Left Curve Section, this can move your car to left or right side.(no reboot required)
   - RightCurv Offset: if you are not satisfy with Right Curve Section, this can move your car to left or right side.(no reboot required)
   - Show BSM Status: Show when a car is in blindspot. need car BSM function.
   - Max Steering Angle: Default is 90. If you want more, increase this. Some car will not accept value above 90.
   - Str Angle Adjust: To keep car on a straight road, If the value of steering angle is not 0.0, adjust this to be 0.0
   - Stop Steer Assist on Turn Signals: Openpilot doesn't steer your car when turn signal is active.
   - Reset MaxSpeed Over CurrentSpeed: Sync scc speed with car current speed, the OP MaxSpeed synchronize to your car speed.
   - Enable OSM SpeedLimit: Use OSM SpeedLimit, active internet required. (reboot required).
   - Use Stock SafetyCAM Speed: Some cars have the signal in CAN message. not for all HKG cars.
   - SpeedLimit Offset (% or +-): Use to set scc speed above or below the OSM or Stock can reported speed. This can be % or Speed amount + or -. This work with OSM and Stock Can Auto Speeedlimit function.
   - SafetyCam SignType: You can select 2 options to show on the screen, circular (EU) type and retangular (US) type of speedlimit sign.
   - SafetyCamDist Adj(%): Change the target distance if you are in the decel situation of safetycam.
   - Change Cruise Gap at Stop: Cruise Gap changed to 1 step for departure faster, it gets back to orignal Gap after few second.
   - VisionCurvDecel(ModelSpeed: CarSpeed): set speed is changed by Curvature of vision.
   - OSM CurvDecel(%): If OSM has the value of curv speed, up/down this value if you want to add/subtract.
   - Use Auto Engagement: When OP is in disengagement status, Auto engagement is enabled when your car is moving. Cruise Standby status is needed at least.
   - Auto Engage Speed(km/h): Auto Engagement is enabled at this speed.
   - Use Auto RES while Driving: SCC speed automatically set again when releasing SCC.(reboot required)
   - AutoRES Option: CruiseSet/MaxSpeedSet, MaxSpeedSet: Your OP MAX Speed set with RES Set speed. CruiseSet:only set to current set speed, not changed OP MAX Speed.
   - AutoRES Condition: RelBrake/OnGas, RelBrake: SCC speed set again when you release from brake pedal. OnGas: SCC speed set again when you step gas pedal.
   - AutoRES Allow(sec): time to allow for AutoRES
   - RES Count at Standstill: Adjust RES CAN message count to start from StandStill. upper, if the departure is failed. lower, if your car generate cluster error or can error.(no reboot required)
   - Steer Wind Down: to mitiate torque at error status of your lkas
   - MainSwitch Openpilot ON/OFF: You can turn on/off OP using Cruise Button on steering wheel.
   - DEBUG UI 1: Show debug UI on screen. 2 lines bottom of screen.(no reboot required)
   - DEBUG UI 2: Show debug UI on screen. other lines except 2 lines bottom.(no reboot required)
   - Show TMUX Error: Turn this on, if you want to show tmux error on screen related to process such as controlsd, plannerd and so on.(reboot required)
   - Show LongControl LOG: show long control log at DEBUG UI 1.(no reboot required)
   - Use Smart Prebuilt: Your device can be booted quickly. The file, Prebuilt is removed when you do push 'CHECK' button on the Menu or type 'gi' command on command line, after then it will be created again when boot&compile is completed.(reboot required)
   - Use FingerPrint 2.0(reboot required)
   - Set LDWS Vehicles
   - Set DriveGear by Force: for cars don't have dbc info of gear(reboot required)
   - Turn Off Communication Issue Alarm: Turn this on if you do not want alert of communication error. Sometimes you could get an alarm with error commuication issue. I don't know actually what error is. seems a bug or uncertain reason.
   - Support WhitePanda: Turn this on if you use WhitePanda. this is related to issue stopping processes when you turn device off.(reboot required)
   - Ignore of Steering Warning: Some cars have Steerwarning, so that not engaged.
   - Ignore Can Error on ISG: for ISG cars. In order to ignore can error, if you want to prevent disengagement.
   - Set BatteryLess Eon: Screen doesn't show information of battery status.
   - Use GoogleMap for Mapbox: Use google map when you search your destination at a web browser.
   - Timezone setting(reboot required)
   - Enable Calibration by Force: developer for engagment test
   - Open Android Settings
   - SoftKey RUN/SET: softkey application
   - RUN Mixplorer: file manager application
   - CAR Force Recognition: If your car is not recognized, choose your car at this.(reboot required)
   - Pand Value Edit: not recommended. enough at current status.
 - Tuning(Function Name: Explanation)
   - CameraOffset: set your camera offset
   - PathOffset: i'm not sure this. but i recommend if you move your camera offset, adjust this as the value.
   - Use Live SteerRatio: Use Live Parameter value.
   - LiveSR Adjsut(%): in some cases, live parameter is higher than original steeratio, i set this to minus value to not steer aggressively.
   - SteerRatio: Your default SteerRatio
   - SteerRatioMax: Max SteerRatio if you use Varaible SteerRatio not Live SR.
   - SteerActuatorDelay: level how your car reacts to upcoming road curvature.
   - SteerRateCost: How your car make steer strong to turn with the road curvature. you want make it strong lower the value. too much low values, it will make the steering little unstable.
   - SteerLimitTimer: timer how long op hold the steer. and timer for alert.
   - TireStiffnessFactor: lower value makes your steer more aggressive.
   - SteerMaxDefault: SteerMax Default value
   - SteerMaxMax: SteerMax Max value if you use variable SteerMax.
   - SteerMaxV: multiply to the output scale. it effects steer saturation or others.
   - Use variable SteerMax: use variable steermax by road curvature. it works above 30km/h.
   - SteerDeltaUpDefault: how fast steer inside in certain timing scope.
   - SteerDeltaUpMax: max value if you use variable steerdelta
   - SteerDeltaDownDefault: how fast steer outside in certain timing scope.
   - SteerDeltaDownMax: max value if you use variable steerdelta
   - Use variable SteerDelta: use variable steerdelta by road curvature. it works above 30km/h.
   - SteerThreshold: driver steering torque threshold
   - LatControl(Reboot): PID/INDI/LQR
   - Use LiveTune and Show UI: this will show button on the screen, you can up/down the values of tune. it will be applied in realtime. you can also touch top-right corner(comma wheel icon) to show tune panel on the screen.
   - Tune Values: change and find appropriate values.
   - LONG CONTROL MENU(RadarHareness required)
    - Use DynamicTR: TR changed by car speed.
    - CruiseGap: set TR of other Cruise Gaps
    - Use Radar Long Assist: when your car approaches to lead car, use combined value both OP gas/brake and Radar one.
    - Adjust Stopping Distance: help stopping distance more close to lead car(not recommended)
    - Enable E2E Long: Use Comma E2E long, sometimes it is not comfortable. think it's earlier to release.

4. Additional Features except above or more things to explain.
 - It can change cruise mode pushing GAP Button at Cruise Standby status.(OpenpilotStock, Dist+Curv, Dist only, Curv only, OneWay mode, Speedlimit decelation mode only)
 - MapBox support, appreciate to Dragonpilot
 - Showing IP Address/SSID name
 - It has Cruise Button Spamming to adjust car set speed, it uses OP target speed to keep certain distance to lead car. It makes your car smooth when you approch to lead car and acceleration slower than stock.
 - SmartMDPS support
 - Auto Recognition if SCC bus is on 2.

![](https://user-images.githubusercontent.com/37757984/127420744-89ca219c-8f8e-46d3-bccf-c1cb53b81bb1.png)

Table of Contents
=======================

* [What is openpilot?](#what-is-openpilot)
* [Running in a car](#running-in-a-car)
* [Running on PC](#running-on-pc)
* [Community and Contributing](#community-and-contributing)
* [User Data and comma Account](#user-data-and-comma-account)
* [Safety and Testing](#safety-and-testing)
* [Directory Structure](#directory-structure)
* [Licensing](#licensing)

---

What is openpilot?
------

[openpilot](http://github.com/commaai/openpilot) is an open source driver assistance system. Currently, openpilot performs the functions of Adaptive Cruise Control (ACC), Automated Lane Centering (ALC), Forward Collision Warning (FCW) and Lane Departure Warning (LDW) for a growing variety of [supported car makes, models and model years](docs/CARS.md). In addition, while openpilot is engaged, a camera based Driver Monitoring (DM) feature alerts distracted and asleep drivers. See more about [the vehicle integration](docs/INTEGRATION.md) and [limitations](docs/LIMITATIONS.md).

<table>
  <tr>
    <td><a href="https://www.youtube.com/watch?v=mgAbfr42oI8" title="YouTube" rel="noopener"><img src="https://i.imgur.com/kAtT6Ei.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=394rJKeh76k" title="YouTube" rel="noopener"><img src="https://i.imgur.com/lTt8cS2.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=1iNOc3cq8cs" title="YouTube" rel="noopener"><img src="https://i.imgur.com/ANnuSpe.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=Vr6NgrB-zHw" title="YouTube" rel="noopener"><img src="https://i.imgur.com/Qypanuq.png"></a></td>
  </tr>
  <tr>
    <td><a href="https://www.youtube.com/watch?v=Ug41KIKF0oo" title="YouTube" rel="noopener"><img src="https://i.imgur.com/3caZ7xM.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=NVR_CdG1FRg" title="YouTube" rel="noopener"><img src="https://i.imgur.com/bAZOwql.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=tkEvIdzdfUE" title="YouTube" rel="noopener"><img src="https://i.imgur.com/EFINEzG.png"></a></td>
    <td><a href="https://www.youtube.com/watch?v=_P-N1ewNne4" title="YouTube" rel="noopener"><img src="https://i.imgur.com/gAyAq22.png"></a></td>
  </tr>
</table>


Running in a car
------

To use openpilot in a car, you need four things
* This software. It's free and available right here.
* One of [the 140+ supported cars](docs/CARS.md). We support Honda, Toyota, Hyundai, Nissan, Kia, Chrysler, Lexus, Acura, Audi, VW, and more. If your car is not supported, but has adaptive cruise control and lane keeping assist, it's likely able to run openpilot.
* A supported device to run this software. This can be a [comma two](https://comma.ai/shop/products/two), [comma three](https://comma.ai/shop/products/three), or if you like to experiment, a [Ubuntu computer with webcams](https://github.com/commaai/openpilot/tree/master/tools/webcam).
* A way to connect to your car. With a comma two or three, you need only a [car harness](https://comma.ai/shop/products/car-harness). With an EON Gold or PC, you also need a [black panda](https://comma.ai/shop/products/panda).

We have detailed instructions for [how to install the device in a car](https://comma.ai/setup).

Running on PC
------

All of openpilot's services can run as normal on a PC, even without special hardware or a car. To develop or experiment with openpilot you can run openpilot on recorded or simulated data.

With openpilot's tools you can plot logs, replay drives and watch the full-res camera streams. See [the tools README](tools/README.md) for more information.

You can also run openpilot in simulation [with the CARLA simulator](tools/sim/README.md). This allows openpilot to drive around a virtual car on your Ubuntu machine. The whole setup should only take a few minutes, but does require a decent GPU.


Community and Contributing
------

openpilot is developed by [comma](https://comma.ai/) and by users like you. We welcome both pull requests and issues on [GitHub](http://github.com/commaai/openpilot). Bug fixes and new car ports are encouraged. Check out [the contributing docs](docs/CONTRIBUTING.md).

Documentation related to openpilot development can be found on [docs.comma.ai](https://docs.comma.ai). Information about running openpilot (e.g. FAQ, fingerprinting, troubleshooting, custom forks, community hardware) should go on the [wiki](https://github.com/commaai/openpilot/wiki).

You can add support for your car by following guides we have written for [Brand](https://blog.comma.ai/how-to-write-a-car-port-for-openpilot/) and [Model](https://blog.comma.ai/openpilot-port-guide-for-toyota-models/) ports. Generally, a car with adaptive cruise control and lane keep assist is a good candidate. [Join our Discord](https://discord.comma.ai) to discuss car ports: most car makes have a dedicated channel.

Want to get paid to work on openpilot? [comma is hiring](https://comma.ai/jobs/).

And [follow us on Twitter](https://twitter.com/comma_ai).

User Data and comma Account
------

By default, openpilot uploads the driving data to our servers. You can also access your data through [comma connect](https://connect.comma.ai/). We use your data to train better models and improve openpilot for everyone.

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
        ├── logcatd         # Android logcat as a service
        ├── loggerd         # Logger and uploader of car data
        ├── modeld          # Driving and monitoring model runners
        ├── proclogd        # Logs information from proc
        ├── sensord         # IMU interface code
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
