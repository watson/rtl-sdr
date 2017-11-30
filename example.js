'use strict'

var rtlsdr = require('rtl-sdr')

// Index of the device we want to use
const deviceIndex = 0

// You can either set the gain manually, choose to have the RTLSDR
// device select the right gain for you, or probe it to find the maxium
// gain and then use that:
//
// A) Set to true to ask RTLSDR device to automatically set gain
const autoGain = false
// B) Or probe RTLSDR device to find the maximum possible gain
const maxGain = true
// C) Or just manually set a gain value (in tenths of a dB: e.g. 115 == 11.b dB)
let gain = 0

// Correction value in parts per million (ppm) to use in frequency
// correction
const ppmCorrection = 0

// Set to false to disable digital AGC mode
const agcMode = true

// Set center frequency to tune to
const freq = 1090e6

// Set center frequency to tune to
const sampleRate = 2e6

// Output buffers used with get_device_usb_strings
const vendor = Buffer.alloc(256)
const product = Buffer.alloc(256)
const serial = Buffer.alloc(256)

const deviceCount = rtlsdr.get_device_count()
if (!deviceCount) {
  console.log('No supported RTLSDR devices found.')
  process.exit(1)
}

console.log('Found %d device(s):', deviceCount)
for (let i = 0; i < deviceCount; i++) {
  rtlsdr.get_device_usb_strings(i, vendor, product, serial)
  console.log('%d: %s, %s, SN: %s %s', i, vendor, product, serial,
      i === deviceIndex ? '(currently selected)' : '')
}

const dev = rtlsdr.open(deviceIndex)
if (typeof dev === 'number') {
  console.log('Error opening the RTLSDR device: %s', dev)
  process.exit(1)
}

// Set gain mode to max
rtlsdr.set_tuner_gain_mode(dev, 1)

if (!autoGain) {
  if (maxGain) {
    // Find the maximum gain available

    // Prepare output array for rtl-sdr to write out possible gain
    // values for the device. We set it a large size so it should be
    // possible to accomondate all types of devices:
    const gains = new Int32Array(100)

    // Populate the gains array and get the actual number of different
    // gains available. This number will be less than the actual size of
    // the array:
    const numgains = rtlsdr.get_tuner_gains(dev, gains)

    // Get the highest gain possible
    gain = gains[numgains - 1]

    console.log('Max available gain is: %d', gain / 10)
  }

  // Set the tuner to the selected gain
  console.log('Setting gain to: %d', gain / 10)
  rtlsdr.set_tuner_gain(dev, gain)
} else {
  console.log('Using automatic gain control')
}

// Set the frequency correction value for the device
rtlsdr.set_freq_correction(dev, ppmCorrection)

// Enable or disable the internal digital AGC of the RTL2822
rtlsdr.set_agc_mode(dev, agcMode ? 1 : 0)

// Tune center frequency
rtlsdr.set_center_freq(dev, freq)

// Select sample rate
rtlsdr.set_sample_rate(dev, sampleRate)

// Reset the internal buffer
rtlsdr.reset_buffer(dev)

console.log('Gain reported by device: %d', rtlsdr.get_tuner_gain(dev) / 10)

// Start reading data from the device:
//
// bufNum: optional buffer count, bufNum * bufLen = overall buffer size
//         set to 0 for default buffer count (15)
//
// bufLen: optional buffer length, must be multiple of 512, should be a
//         multiple of 16384 (URB size), set to 0 for default buffer
//         length (2^18)
const bufNum = 12
const bufLen = 2 ** 18 // 2^18 == 256k
rtlsdr.read_async(dev, onData, onEnd, bufNum, bufLen)

function onData (data, size) {
  console.log('onData[%d]:', size, data)
}

function onEnd () {
  console.log('onEnd')
}
