# rtl-sdr

A low level binding to the
[librtlsdr](https://github.com/steve-m/librtlsdr) library for Node.js.

[![Build status](https://travis-ci.org/watson/rtl-sdr.svg?branch=master)](https://travis-ci.org/watson/rtl-sdr)

## Prerequisites

This module requires that you have
[librtlsdr](https://github.com/steve-m/librtlsdr) installed on your
system. In turn librtlsdr requires [libusb](http://libusb.info/).

You can install librtlsdr with most package managers which will ensure
you have the right dependencies.

Homebrew (macOS):

```
brew install librtlsdr
```

Debian based Linux distros:

```
apt-get install librtlsdr-dev
```

## Installation

```
npm install rtl-sdr --save
```

## Usage

```js
const rtlsdr = require('rtl-sdr')

// Get number of connected RTLSDR devices
const deviceCount = rtlsdr.get_device_count()

if (!deviceCount) {
  console.log('No supported RTLSDR devices found')
  process.exit(1)
}

console.log('Found %d device(s):', deviceCount)
```

For a complete running example, see
[example.js](https://github.com/watson/rtl-sdr/tree/master/example.js).

## API

The API is thin wrapper on top of the librltsdr API as found in the
[rtl-sdr.h](https://github.com/steve-m/librtlsdr/tree/master/include/rtl-sdr.h)
file. All functions exposed by this module are named the same as their
librtlsdr counterparts, except for the `rtlsdr_` prefix.

Only a few changes have been made to the return values and function
arguments in order to make this suitable for JavaScript.

### Tuner Constants

The following constants are exposed on the `rtlsdr` object:

- `TUNER_UNKNOWN`
- `TUNER_E4000`
- `TUNER_FC0012`
- `TUNER_FC0013`
- `TUNER_FC2580`
- `TUNER_R820T`
- `TUNER_R828D`

### Functions

#### `count = get_device_count()`

Return number of devices found.

#### `name = get_device_name(index)`

Return name of device at `index`

#### `error = get_device_usb_strings(index, manufact, product, serial)`

Get manufacturer, product and serial number from device at `index`.

Arguments:

- `index` - Device index
- `manufact` - An zero filled `Buffer` with space for up to 256 bytes to
  which the manufacturer name will be written (may be `null`)
- `product` - An zero filled `Buffer` with space for up to 256 bytes to
  which the product name will be written (may be `null`)
- `serial` - An zero filled `Buffer` with space for up to 256 bytes to
  which the serial number will be written (may be `null`)

Returns `0` on success.

#### `error = get_index_by_serial(serial)`

Get device index by USB serial string descriptor.

Returns `0` on success, `-1` if name is `null`, `-2` if no devices were
found at all, and `-3` if devices were found, but nonee with mathcing
name.

#### `device = open(index)`

Open device at `index`.

Returns a device handle that can be used as an argument to other
functions.

#### `error = close(device)`

Close device.

Arguments:

- `device` - The device handle given by `open()`

Returns 0 on success.

#### `error = set_xtal_freq(device, rtl_freq, tuner_freq)`

Set crystal oscillator frequencies used for the RTL2832 and the tuner
IC.

Usually both ICs use the same clock. Changing the clock may make sense
if you are applying an external clock to the tuner or to compensate the
frequency (and samplerate) error caused by the original (cheap) crystal.

NOTE: Call this function only if you fully understand the implications.

Arguments:

- `device` - The device handle given by `open()`
- `rtl_freq` - Frequency value used to clock the RTL2832 in Hz
- `tuner_freq` - Frequency value used to clock the tuner IC in Hz

Returns `0` on success.

#### `error = get_xtal_freq(device, rtl_freq, tuner_freq)`

Get crystal oscillator frequencies used for the RTL2832 and the tuner
IC.

Usually both ICs use the same clock.

Arguments:

- `device` - The device handle given by `open()`
- `rtl_freq` - A 32 bit `Buffer` object to which a 32 bit unsigned
  integer will be written with the frequency value used to clock the
  RTL2832 in Hz
- `tuner_freq` - A 32 bit `Buffer` object to which a 32 bit unsigned
  integer will be written with the frequency value used to clock the
  tuner IC in Hz

Returns `0` on success.

#### `error = get_usb_strings(device, manufact, product, serial)`

Get USB device strings.

NOTE: The string arguments must provide space for up to 256 bytes.

Arguments:

- `device` - The device handle given by `open()`
- `manufact` - An zero filled `Buffer` with space for up to 256 bytes to
  which the manufacturer name will be written (may be `null`)
- `product` - An zero filled `Buffer` with space for up to 256 bytes to
  which the product name will be written (may be `null`)
- `serial` - An zero filled `Buffer` with space for up to 256 bytes to
  which the serial number will be written (may be `null`)

Returns `0` on success

#### `error = write_eeprom(device, data, offset, len)`

Write the device EEPROM.

Arguments:

- `device` - The device handle given by `open()`
- `data` - Buffer of data to be written
- `offset` - Address where the data should be written
- `len` - Length of the data

Returns `0` on success, `-1` if device handle is invalid, `-2` if EEPROM
size is exceeded, or `-3` if no EEPROM was found.

#### `error = read_eeprom(device, data, offset, len)`

Read the device EEPROM.

Arguments:

- `device` - The device handle given by `open()`
- `data` - Buffer where the data should be written
- `offset` - Address where the data should be read from
- `len` - Length of the data

Returns `0` on success, `-1` if device handle is invalid, `-2` if EEPROM
size is exceeded, or `-3` if no EEPROM was found.

#### `error = set_center_freq(device, freq)`

Tune device to the given frequency.

Arguments:

- `freq` - The center frequency to tune to

Returns `0` on success.

#### `freq = get_center_freq(device)`

Get actual frequency the device is tuned to.

Arguments:

- `device` - The device handle given by `open()`

Returns `0` on error, frequency in Hz otherwise.

#### `error = set_freq_correction(device, ppm)`

Set the frequency correction value for the device.

Arguments:

- `device` - The device handle given by `open()`
- `ppm` - Correction value in parts per million (ppm)

Returns 0 on success.

#### `ppm = get_freq_correction(device)`

Get actual frequency correction value of the device.

Arguments:

- `device` - The device handle given by `open()`

Returns correction value in parts per million (ppm).

#### `type = get_tuner_type(device)`

Get the tuner type.

Arguments:

- `device` - The device handle given by `open()`

Returns `TUNER_UNKNOWN` on error, tuner type otherwise (see list of
tuner constants above).

#### `numGains = get_tuner_gains(device, gains)`

Get a list of gains supported by the tuner.

NOTE: The gains argument must be preallocated by the caller. If `null`
is being given instead, the number of available gain values will be
returned.

Arguments:

- `device` - The device handle given by `open()`
- `gains` - Empty array provided by the caller to which the list of gain
  values will be written. In tenths of a dB, 115 means 11.5 dB. If using
  a TypedArray, use an `Int32Array` with enough room to fill in the
  available gains, e.g. size 100.

Returns <= `0` on error, number of available (returned) gain values
otherwise.

#### `error = set_tuner_gain(device, gain)`

Set the gain for the device.

Manual gain mode must be enabled for this to work.

Valid gain values (in tenths of a dB) for the E4000 tuner: -10, 15, 40,
65, 90, 115, 140, 165, 190, 215, 240, 290, 340, 420, 430, 450, 470, 490.

Valid gain values may be queried with `get_tuner_gains` function.

Arguments:

- `device` - The device handle given by `open()`
- `gain` - In tenths of a dB, 115 means 11.5 dB

Returns `0` on success.

#### `gain = get_tuner_gain(device)`

Get actual gain the device is configured to.

Arguments:

- `device` - The device handle given by `open()`

Returns `0` on error, otherwise gain in tenths of a dB, 115 means 11.5
dB.

#### `error = set_tuner_if_gain(device, stage, gain)`

Set the intermediate frequency gain for the device.

Arguments:

- `device` - The device handle given by `open()`
- `stage` - Intermediate frequency gain stage number (1 to 6 for E4000)
- `gain` - In tenths of a dB, -30 means -3.0 dB

Returns `0` on success.

#### `error = set_tuner_gain_mode(device, stage, gain)`

Set the gain mode (automatic/manual) for the device.
Manual gain mode must be enabled for the gain setter function to work.

Arguments:

- `device` - The device handle given by `open()`
- `manual` - Gain mode, 1 means manual gain mode shall be enabled.

Returns `0` on success.

#### `error = set_sample_rate(device, samp_rate)`

Set the sample rate for the device, also selects the baseband filters
according to the requested sample rate for tuners where this is possible.

Arguments:

- `device` - The device handle given by `open()`
- `samp_rate` - The sample rate to be set, possible values are: 225001 -
  300000 Hz and 900001 - 3200000 Hz. Sample loss is to be expected for
  rates > 2400000

Returns 0 on success, -EINVAL on invalid rate

#### `hz = get_sample_rate(device)`

Get actual sample rate the device is configured to.

Arguments:

- `device` - The device handle given by `open()`

Returns `0` on error, sample rate in Hz otherwise.

#### `error = set_testmode(device, test)`

Enable test mode that returns an 8 bit counter instead of the samples.
The counter is generated inside the RTL2832.

Arguments:

- `device` - The device handle given by `open()`
- `test` - Mode, `1` means enabled, `0` disabled

Returns `0` on success.

#### `error = set_agc_mode(device, digital)`

Enable or disable the internal digital AGC of the RTL2832.

Arguments:

- `device` - The device handle given by `open()`
- `digital` - AGC mode, `1` means enabled, `0` disabled

Returns `0` on success.

#### `error = set_direct_sampling(device, on)`

Enable or disable the direct sampling mode. When enabled, the IF mode
of the RTL2832 is activated, and `set_center_freq()` will control the
IF-frequency of the DDC, which can be used to tune from 0 to 28.8 MHz
(xtal frequency of the RTL2832).

Arguments:

- `device` - The device handle given by `open()`
- `on` - 0 means disabled, 1 I-ADC input enabled, 2 Q-ADC input enabled

Returns `0` on success.

#### `state = get_direct_sampling(device)`

Get state of the direct sampling mode

Arguments:

- `device` - The device handle given by `open()`

Returns `-1` on error, `0` means disabled, `1` I-ADC input enabled, `2`
Q-ADC input enabled.

#### `error = set_offset_tuning(device, on)`

Enable or disable offset tuning for zero-IF tuners, which allows to
avoid problems caused by the DC offset of the ADCs and 1/f noise.

Arguments:

- `device` - The device handle given by `open()`
- `on` - 0 means disabled, 1 enabled

Returns `0` on success.

#### `state = get_offset_tuning(device)`

Get state of the offset tuning mode.

Arguments:

- `device` - The device handle given by `open()`

Returns `-1` on error, `0` means disabled, `1` enabled

#### `error = reset_buffer(device)`

Reset the internal buffer.

Arguments:

- `device` - The device handle given by `open()`

Returns 0 on success.

#### `error = read_sync(device, buf, len, n_read)`

Read samples from the device synchronously.

NOTE: Make sure the buffer is big enough.

Arguments:

- `device` - The device handle given by `open()`
- `buf` - The buffer to read into
- `len` - The size of the buffer
- `n_read` - A `Buffer.alloc(4)` object to which a 32 bit integer will
  be written containing the size of the data that have been written to
  `buf`

Returns 0 on success.

#### `read_async(device, on_data, on_end, buf_num, buf_len)`

Read samples from the device asynchronously. Data will be read until it
is being canceled using `cancel_async()`

Arguments:

- `device` - The device handle given by `open()`
- `on_data` - Callback function to return received samples. The callback
  will be called with a buffer as the first arguemnt and the size of the
  buffer as the second
- `on_end` - Callback function to be called once no more samples will be
  read
- `buf_num` - Optional buffer count, buf_num * buf_len = overall buffer
  size set to `0` for default buffer count (15)
- `buf_len` - Optional buffer length, must be multiple of 512, should be
  a multiple of 16384 (URB size), set to `0` for default buffer length
  (16 * 32 * 512)

Returns 0 on success.

#### `error = cancel_async(device)`

Cancel all pending asynchronous operations on the device.

Arguments:

- `device` - The device handle given by `open()`

Returns `0` on success.

## License

MIT
