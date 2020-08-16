# vitaUSBtty
Provides kernel console output and crashdump info via serial usb

## Supported Firmwares
3.60 only

## Usage
Copy vitaUsbtty.sprx over to your Henkaku enabled PS Vita.

Add it to ux:tai/config.txt.

Reboot your Vita.

Hook up your USB cable to Vita and your PC. Your PC should now detect your Vita as serial USB device.

Use your favorite terminal software to connect to your Vita.

Enjoy.

## Porting
All firmware version specific offsets should be located in koffsets.h. Feel free to add others.

## Notes
As far as i now, it is just Windows 7 which does not support the vita usb serial device out of the box. The Win7 driver can be found as part of the PSM Suite.
