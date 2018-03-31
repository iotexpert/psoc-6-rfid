A "C" library interface to the MFRC522 Arduino RFID class library.

- I do not have hardware so I cannot test at all.  If you get me hardware
  I can play with this.
- The file rfid.h is the "C" interface. It parallels the C++ interface
  on the classes and should be used in the same way.
- There are two options, selected through the preprocessor define
  MULTIPLE_RFID_DEVICES.  If this is defined, you can create multiple
  RFID devices in a project, but malloc must be defined and implemented.
  If this is not defined, a single status RFID device is created and can
  be used.  I recommend the single instance version if at all possible.
- Some more thought may be needed for the pins.  If I can get hardware to