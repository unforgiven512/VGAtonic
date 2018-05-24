# VGAtonic Hardware - Revision B #

**NOTE:**  _This is the current version that should be built._ It has the **NTSC** pins and almost the same number of **GPIO** connections broken out as **Rev A** on the _(optional)_ unpopulated header on the back of the PCB.


## Revision Info ##

If using primarily For **VGA**, there is no contest - _build Rev B_.
You will get more serial speeds, better ground isolation, shielding on the SPI signals, and... LEDs!


## Firmware Info ##

By default, you should try the **REGULAR** firmware, not the **WIDESCREEN** firmware.

The resolution `640x480` _at_ `60Hz` is an industry standard, so that's the fallback _(and it allows `320x240`, `160x120` and `80x60` resolution(s), as well, for slower parts or when VGATonic is to be driven over serial)_.

Once your board is working, the **WIDESCREEN** firmware is _much_ nicer, but it only offers `848x480` and `424x240` resolutions. It is only tested on **Rev B**, but it likely works on **Rev A** - just be careful.

Note, too, that the **TERMINAL EMULATOR** only works with the **REGULAR** firmware; it isn't yet updated for the **WIDESCREEN** firmware.


## Directory Contents ##

In this directory, you'll find the design documentation for VGAtonic's second design, which we're naming **v1.25** or "**Rev B**".


## Component Info ##

It is **39%** smaller than the first version and contains **54** components _(counting the headers and a single component each)_.

Returning are the Xilinx **XC95144** CPLD, the ISSI **IS61LV5128AL** memory, and the Linear **LTC6903**.

The Atmel **ATtiny2313a** has been replaced with the **ATtiny4313** _(it's pin compatible)_, and clocked it with a **7.3728 MHz** crystal _(which gives us "magic frequencies" to support many new serial speeds)_.
Additionally, an external clock saves us the optional calibration step in programming, _which could be a bit of a hassle_; now there are only two steps for the microcontroller:
* Burning the EEPROM
* Programming the primary flash (code) of the part

Oh, we also added LEDs! That, _of course_, is the **best** new feature.


## License ##

MIT (see root directory)
