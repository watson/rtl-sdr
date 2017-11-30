'use strict'

const rtlsdr = require('bindings')('rtlsdr.node')

module.exports = rtlsdr

rtlsdr.TUNER_UNKNOWN = 0
rtlsdr.TUNER_E4000 = 1
rtlsdr.TUNER_FC0012 = 2
rtlsdr.TUNER_FC0013 = 3
rtlsdr.TUNER_FC2580 = 4
rtlsdr.TUNER_R820T = 5
rtlsdr.TUNER_R828D = 6

/* eslint-disable camelcase */

const get_tuner_gains = rtlsdr.get_tuner_gains
rtlsdr.get_tuner_gains = function (dev, gains) {
  const buf = Buffer.alloc(gains.length * 4) // TODO: int size is OS dependent
  const result = get_tuner_gains(dev, buf)
  for (let i = 0; i < gains.length; i++) {
    gains[i] = buf.readInt32LE(i * 4) // TODO: Endian is OS dependent
  }
  return result
}

const read_sync = rtlsdr.read_sync
rtlsdr.read_sync = function (dev, buf, len, n_read) {
  len = len || buf.length
  n_read = n_read || Buffer.allocUnsafe(4) // TODO: int size is OS dependent
  const result = read_sync(dev, buf, len, n_read)
  if (result !== 0) {
    throw new Error('Unexpected return value from rtlsdr_read_syn: ' + result)
  }
  return n_read.readInt32LE(n_read) // TODO: Endian is OS dependent
}
