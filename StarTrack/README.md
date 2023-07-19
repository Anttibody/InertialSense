# StarTrack

### Known Issues:

- The detection of the USB drive is intermittent (it sometimes fails to mount properly).  There is not a clear way to tell if the drive is mounted or not, as its designed to be operated both with and without.
This is mostly observed at initial startup. We recommend turning on the device, waiting for the BLUE led to light, then remove the USB drive, re-insert it again, and press-hold the button for 7 seconds, to
force a reset. This will ensure that the config is loaded and the log is started to the correct location.
- When logging to the USB drive, some log data can be lost if the drive is removed before a RESET is performed.  To ensure that all logs are complete, it is recommended to ALWAYS perform a RESET before removing
the USB drive.

### Quick-Start

This project uses CMake, in conjuction with a 'install.sh' script (located under ./scripts), to build all necessary components, install, and register the application for execution on a Raspberry Pi.  The following
steps are only necessary for the initial setup of a new device.  Once configured correctly, the device should automatically start up with each power up.

#### Stock Host configuration:

The Raspberry Pi runs Ubuntu 23.04, for ARM64, and is configured to use WIFI, with Dynamic IP assignment.  It uses the AVAHI service to publish its address onto the network, and is assigned the hostname 'startrack'.
From most Linux and Mac devices, you should be able to connect to the device using the following command:

```bash
ssh startrack@startrack.local
```

By default, all the source code should be installed the 'startrack' users home directory, under the directory name 'StarTrack' (ie, /home/startrack/StarTrack).  If you are building a new device from scratch, follow
these instructions:

#### Fresh Build:

If you are installing on a clean system, it is recommended to use the Ubuntu 23.04 Server build for ARM64 (targeting a Raspberry Pi 4b) though these are loose requirements and shouldn't change the performance or
operation of the system, there maybe difference in building/configuration, dependency installation and other steps, which may introduce unforeseen complications.

#### Building and Install

```bash
cd ~
git clone https://github.com/inertialsense/StarTrack.git
cd StarTrack
./install.sh
````

This command should build all necessary components, and register all services for an automatic startup.


### Application Usage:

Power on the device by plugging a 5VDC 3A USB-C power supply into the side of the device.  Wait for the large, PRIMARY LED to turn on.  This may take upwards of 10-30 seconds.

If the LED is BLINKING RED-YELLOW, double check USB connections between the Raspberry Pi, and the RUG3/IMX.  Sometimes the cable can be dislodged from the RUG3 module.

The LED should turn SOLID BLUE as soon as the RUG3 is detected, connected, and configured, and the system is ready to begin operation.

The LED will show SOLID BLUE while waiting for the FIRST sample point, and SOLID YELLOW when waiting for additional sample points. 

With each data sample (button press), the LED will blink for 2 seconds, either MAGENTA, RED, or GREEN.
 - If there is a sequence of "Calibration Points" in the StarTrack.yaml configuration file, each of the first N-number of buttons presses is registered as a calibration point.  The LED will BLINK MAGENTA if the
recorded point is a calibration point.
 - If the sample location is part of the sequence defined in the configuration file, and the location of the sampled point matches the location in the sequence, the LED will blink GREEN.
 - If the sample location does NOT match the sequence location or sequence order, the LED will blink RED.

Once the sequence has been completely sampled, the LED will CONTINUALLY RAPID-BLINK either RED or GREEN, depending on whether the sequence locations were sampled correctly, and in the correct order.

At any time, the user can PRESS AND HOLD the sample button for 6 seconds to reinitialize the sequence and start the process over again (including all calibration points, etc).  When Resetting, holding the button
for > 2 seconds will cause the LED to being flashing WHITE. After holding the button for 6 seconds, the LED will quickly blink RED/WHITE, indicating that the Reset sequence has been registered.  The reset is
completed when the user releases the button.

Notes:
- Once a BLINKING RED condition occurs, the sequence becomes invalid for all further sampled points.  In other words, once the LED blinks RED, it will ALWAYS BLINK RED, until the device is reset (even when it
returns to a YELLOW IDLE, all further sampled points will also BLINK RED).
- There is no normal condition which leaves the LED in an OFF state without the button pressed.  This is intentional to reminder the user that the device is powered, and collecting data.  If the device is powered,
and the LED remains off for more than 30 seconds, this should be considered a fatal error of the system.
- The device is ALWAYS collecting data (even when sitting idle), and can consume large amounts of data when left for extended periods of time.  It is not recommended to leave the device unattended, as it may
exhaust available storage space and prevent further operation.
- If the primary LED fails to illuminate after turning on the device, there are 2 smaller LEDs located at the top of the wrench.  A RED led indicates that the device is powered.  A FLICKERING GREEN led indicated
disk/storage activity.
  - If there is NO LEDs at all, confirm your power supply is sufficient.  The Raspberry PI device can consume significant power, and may require a USB power supply which can deliver 3-Amps, or 15-watts.
  - If the RED LED is on, but there is no activity/flickering from the GREEN LED, this could indicate a failure to boot. This is typically attributed to either a full SD card, or a corrupt SD card.


### LED Pattern/Colors and Meaning:
 - SOLID BLUE : Waiting for first sample point (either Calibration or Sequence)
 - SOLID YELLOW : Waiting for additional sample points
 - BLINK MAGENTA : Recording calibration point
 - BLINK GREEN : Sampled point is valid and in correct order
 - BLINK RED : Sampled point is invalid or out-of-order
 - RAPID-BLINK GREEN : All points sampled, and all points are correct, and the sequence is collect!
 - RAPID-BLINK RED : All points sampled, but one or more points are incorrect, or the sequence is out of order.
 - BLINK WHITE : Indicates that a RESET is necessary.
 - RAPID-BLINK RED/WHITE : A RESET has been performed.
 - BLINK RED/YELLOW : The RUG3/IMX is not detected -- Check the USB cable, and connection at the base of the RUG3 device.

### Application Flow

 - Application starts at boot-up (systemd service 'startrack')
 - Initialization
   - Check for StarTrack.yaml (first in "./StarTrack.yaml", then "~/StarTrack.yaml", finally "/etc/StarTrack.yaml")
     - Note that running as a service, it does not (typically) run with a home directory. Default behavior is to fall back to /etc/StarTrack.yaml.
   - Configure GPIO for LED and Button
   - Check for button-press on startup
     - if button-pressed, loop until release
       - if button-press duration exceeds 10 seconds, enable debug logging.
   - Configure logging to /var/log/StarTrack/{TIMESTAMP}, unless otherwise defined in StarTrack.yaml
   - Set "Idle" LED pattern to BLINKING RED
   - Wait for connection to RUG3/IMX
   - Configure RUG3/IMX
   - Enable IMX logging to /var/log/StarTrack/{TIMESTAMP}
   - Set "Idle" LED pattern to SOLID BLUE
 - Wait for button Transition  (either from PRESS TO RELEASE, or from RELEASE TO PRESS)
   - If Button Pressed:
     - Enter "Stationary Mode"
     - Turn off LED
     - If button HELD > 2 seconds, Blink WHITE
       - If button HELD > 4 seconds, Rapid-Blink WHITE
         - If button HELD > 6 seconds, Enter "Reset" State, and Rapid-Blink RED-WHITE
   - If Button Released
     - If "Reset" State is set, Re-Initialize
     - Otherwise
       - Sample wrench location from IMX
       - Exit "Stationary Mode" (resume "In-Motion")
       - Record sampled location and evaluate recorded sequence
       - If Sequence is complete (all points sampled)
         - Blink White (for 2 seconds, then resume IDLE pattern)
         - Otherwise, If Sample Point is Calibration point, Blink MAGENTA (for 2 seconds, then resume IDLE pattern)
         - Otherwise
           - If validation matches (up to number of points sampled), Blink GREEN
           - Otherwise, Blink RED
           - If sequence is complete
             - If Sequence is correct, Set "Idle" LED pattern to Rapid-Blink GREEN
             - Otherwise, Set "Idle" LED pattern to Rapid-Blink RED

### Log Data

The StarTrack device is constantly collecting data, about its pose (the combined Position and Orientation) of the device, in addition to textual logs of events and information about the state
of the device. Finally, each sampled point is recorded and logged. All StarTrack logs are stored in the location specified in the StarTrack.yaml, under the 'log_dir' parameter.  Under this
directory, an additional directory is created using the date/time stamp that the device was started.  Note that the StarTrack device does not have a Real-Time clock, and its date/time is derived
from a network time service, if a network connection is available (via Wifi, or Ethernet). If no date/time is set, the clock may record logs with old timestamps. When a reset is performed, the
current log is closed and saved, and a new Log folder is created using the current date/time.

Within the timestamped log folder, the following files may exist:
- points.measured : this the ECEF coordinates that were recorded with each sampled point.  Note that these are not *actual* ECEF coordinates, but are relative to the stored reference ECEF
coordinated stored in the RUG3's flash configuration.
- startrack.log : this is a text file which reports all the events, status and information about the "session".  This file is human readable, and is the primary tool to troubleshoot the operation
of the device and the application running on the Raspberry Pi.
- LOG_SN#####_<date_time>_####.dat : these are binary logs from the RUG3 device (not human readable). And provided highly detailed information about the position/orientation/state of the RUG3/IMX
device, suitable to in-depth analysis.  These are the primary tool used to troubleshoot the analysis algorithm.
- stats_*.txt : these are "side-car" files which provide additional metadata for the INS/RUG3 device logs.


### Dependency Installation

First, install the various dependencies:
```bash
sudo apt install avahi-daemon network-manager cmake g++ gcc gdb build-essential
sudo apt install libusb-1.0-0-dev libyaml-cpp-dev libeigen3-dev libgtest-dev libspdlog-dev libfmt-dev
```

In order to support removable media, run the following commands on the PI to install headlessw auto-mount udev rules:
```bash
git clone https://github.com/Ferk/udev-media-automount.git
cd udev-media-automount
sudo make install
```

If you are running this on actual Raspberry Pi hardware, you'll need libpigpiod-if-dev installed.  This provides a user-space mechanism for accessing
the PI GPIO hardware, without requiring root/elevated privileges.  Again, this is only available on the Raspberry PI itself.
```bash
sudo apt install libpigpiod-if-dev
```

Once installed, the default systemd service configuration is invalid and prevents it from starting.
Run the following command on the raspberry pi to remove the errant config, which will allow it to start correctly, and then enable the service to start on bootup:

```bash
sudo rm -rf /etc/systemd/system/pigpiod.service.d/public.conf
sudo systemctl daemon-reload
sudo systemctl enable pigpiod && systemctl start pigpiod.service
```

Next, make sure the requirements for the InertialSense-SDK are met, specifically that the user/application can access the serial port:

```bash
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
sudo systemctl disable ModemManager.service && sudo systemctl stop ModemManager.service
```
Finally, reboot the Pi to make sure the new permissions take effect, and all services are running:

```bash
sudo reboot
````
