#include <nan.h>
#include <rtl-sdr.h>
#include "jshandle.h"
#include "macros.h"

class RTLSDRDevice : public JSHandle<RTLSDRDevice> {
public:
  static const char *jsClassName() { return "RTLSDRDevice"; }
};

using namespace v8;

NAN_METHOD(get_device_count) {
  CALL_RTLSDR_INT(rtlsdr_get_device_count())
}

NAN_METHOD(get_device_name) {
  ASSERT_UINT(info[0], index)
  CALL_RTLSDR_CONST_CHAR(rtlsdr_get_device_name(index))
}

NAN_METHOD(get_device_usb_strings) {
  ASSERT_UINT(info[0], index)
  CDATA_OR_NULL(info[1], manufact)
  CDATA_OR_NULL(info[2], product)
  CDATA_OR_NULL(info[3], serial)
  CALL_RTLSDR_INT(rtlsdr_get_device_usb_strings(index, manufact, product, serial))
}

NAN_METHOD(get_index_by_serial) {
  ASSERT_BUFFER(info[0], serial)
  CALL_RTLSDR_INT(rtlsdr_get_index_by_serial(CDATA(serial)))
}

NAN_METHOD(open) {
  ASSERT_UINT(info[0], index)
  rtlsdr_dev_t *dev;
  int returnValue = rtlsdr_open(&dev, index);
  if (returnValue != 0) {
    Nan::ThrowError("Could not open device");
    return;
  }
  Local<Object> jsdev = RTLSDRDevice::New(dev);
  info.GetReturnValue().Set(jsdev);
}

NAN_METHOD(close) {
  ASSERT_OBJECT(info[0], jsdev)
  rtlsdr_dev_t *dev = (struct rtlsdr_dev *)RTLSDRDevice::Resolve(jsdev);
  int returnValue = rtlsdr_close(dev);
  Nan::SetInternalFieldPointer(jsdev, 0, 0);
  info.GetReturnValue().Set(Nan::New(returnValue));
}

NAN_METHOD(set_xtal_freq) {
  ASSERT_DEV(info[0], dev)
  ASSERT_UINT(info[1], rtl_freq)
  ASSERT_UINT(info[2], tuner_freq)
  CALL_RTLSDR_INT(rtlsdr_set_xtal_freq(dev, rtl_freq, tuner_freq))
}

NAN_METHOD(get_xtal_freq) {
  ASSERT_DEV(info[0], dev)
  ASSERT_BUFFER(info[1], rtl_freq)
  ASSERT_BUFFER(info[2], tuner_freq)
  CALL_RTLSDR_INT(rtlsdr_get_xtal_freq(dev,
        (uint32_t *)CDATA(rtl_freq),
        (uint32_t *)CDATA(tuner_freq)))
}

NAN_METHOD(get_usb_strings) {
  ASSERT_DEV(info[0], dev)
  CDATA_OR_NULL(info[1], manufact)
  CDATA_OR_NULL(info[2], product)
  CDATA_OR_NULL(info[3], serial)
  CALL_RTLSDR_INT(rtlsdr_get_usb_strings(dev, manufact, product, serial))
}

NAN_METHOD(write_eeprom) {
  ASSERT_DEV(info[0], dev)
  ASSERT_BUFFER(info[1], data)
  ASSERT_UINT8(info[2], offset)
  ASSERT_UINT16(info[3], len)
  CALL_RTLSDR_INT(rtlsdr_write_eeprom(dev, (uint8_t *)CDATA(data), offset, len))
}

NAN_METHOD(read_eeprom) {
  ASSERT_DEV(info[0], dev)
  ASSERT_BUFFER(info[1], data)
  ASSERT_UINT8(info[2], offset)
  ASSERT_UINT16(info[3], len)
  CALL_RTLSDR_INT(rtlsdr_write_eeprom(dev, (uint8_t *)CDATA(data), offset, len))
}

NAN_METHOD(set_center_freq) {
  ASSERT_DEV(info[0], dev)
  ASSERT_UINT(info[1], ppm)
  CALL_RTLSDR_INT(rtlsdr_set_center_freq(dev, ppm))
}

NAN_METHOD(get_center_freq) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_UINT(rtlsdr_get_center_freq(dev))
}

NAN_METHOD(set_freq_correction) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], ppm)
  CALL_RTLSDR_INT(rtlsdr_set_freq_correction(dev, ppm))
}

NAN_METHOD(get_freq_correction) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_get_freq_correction(dev))
}

NAN_METHOD(get_tuner_type) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_get_tuner_type(dev))
}

NAN_METHOD(get_tuner_gains) {
  ASSERT_DEV(info[0], dev)
  ASSERT_BUFFER(info[1], gains)
  CALL_RTLSDR_INT(rtlsdr_get_tuner_gains(dev, (int *)CDATA(gains)))
}

NAN_METHOD(set_tuner_gain) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], gain)
  CALL_RTLSDR_INT(rtlsdr_set_tuner_gain(dev, gain))
}

NAN_METHOD(get_tuner_gain) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_get_tuner_gain(dev))
}

NAN_METHOD(set_tuner_if_gain) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], stage)
  ASSERT_INT(info[2], gain)
  CALL_RTLSDR_INT(rtlsdr_set_tuner_if_gain(dev, stage, gain))
}

NAN_METHOD(set_tuner_gain_mode) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], manual)
  CALL_RTLSDR_INT(rtlsdr_set_tuner_gain_mode(dev, manual))
}

NAN_METHOD(set_sample_rate) {
  ASSERT_DEV(info[0], dev)
  ASSERT_UINT(info[1], rate)
  CALL_RTLSDR_INT(rtlsdr_set_sample_rate(dev, rate))
}

NAN_METHOD(get_sample_rate) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_UINT(rtlsdr_get_sample_rate(dev))
}

NAN_METHOD(set_testmode) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], on)
  CALL_RTLSDR_INT(rtlsdr_set_testmode(dev, on))
}

NAN_METHOD(set_agc_mode) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], on)
  CALL_RTLSDR_INT(rtlsdr_set_agc_mode(dev, on))
}

NAN_METHOD(set_direct_sampling) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], on)
  CALL_RTLSDR_INT(rtlsdr_set_direct_sampling(dev, on))
}

NAN_METHOD(get_direct_sampling) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_get_direct_sampling(dev))
}

NAN_METHOD(set_offset_tuning) {
  ASSERT_DEV(info[0], dev)
  ASSERT_INT(info[1], on)
  CALL_RTLSDR_INT(rtlsdr_set_offset_tuning(dev, on))
}

NAN_METHOD(get_offset_tuning) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_get_offset_tuning(dev))
}

NAN_METHOD(reset_buffer) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_reset_buffer(dev))
}

NAN_METHOD(read_sync) {
  ASSERT_DEV(info[0], dev)
  ASSERT_BUFFER(info[1], buf)
  ASSERT_INT(info[2], len)
  ASSERT_BUFFER(info[3], n_read)
  CALL_RTLSDR_INT(rtlsdr_read_sync(dev, CDATA(buf), len, (int *)CDATA(n_read)))
}

// NAN_METHOD(wait_async) {} // deprecated

// Executes in worker thread
void AsyncReadCallback(unsigned char *buf, uint32_t len, void *ctx) {
  // Send message to HandleProgressCallback in main thread with read data
  ((Nan::AsyncProgressWorker::ExecutionProgress *)ctx)->Send(reinterpret_cast<char *>(buf), len);
}

class ReadAsyncWorker : public Nan::AsyncProgressWorker {
public:
  ReadAsyncWorker(
      Nan::Callback *onEnd,
      Nan::Callback *onData,
      rtlsdr_dev_t *dev,
      uint32_t buf_num,
      uint32_t buf_len)
    : Nan::AsyncProgressWorker(onEnd), onData(onData), dev(dev), buf_num(buf_num), buf_len(buf_len) {}

  ~ReadAsyncWorker() {
    delete onData;
  }

  // Called when worker thread is started
  void Execute (const Nan::AsyncProgressWorker::ExecutionProgress& progress) {
    // TODO: Consider doing something with the return value of the call to rtlsdr_read_async
    // If the AsyncReadCallback is never called, check the return value of this
    // function call
    rtlsdr_read_async(
        dev,
        AsyncReadCallback,
        (Nan::AsyncProgressWorker::ExecutionProgress *)&progress,
        buf_num,
        buf_len);
  }

  // Call javascript onData callback (this method executes in main thread)
  void HandleProgressCallback(const char *buf, size_t len) {
    Nan::HandleScope scope;

    Local<Value> arguments[2] = {
      Nan::CopyBuffer(buf, (uint32_t)len).ToLocalChecked(),
      Nan::New((uint32_t)len),
    };

    onData->Call(2, arguments);
  }
private:
  Nan::Callback *onData;
  rtlsdr_dev_t *dev;
  uint32_t buf_num;
  uint32_t buf_len;
};

NAN_METHOD(read_async) {
  ASSERT_DEV(info[0], dev)
  ASSERT_FUNCTION(info[1], onData)
  ASSERT_FUNCTION(info[2], onEnd)
  ASSERT_UINT(info[3], buf_num)
  ASSERT_UINT(info[4], buf_len)
  Nan::AsyncQueueWorker(new ReadAsyncWorker(
        new Nan::Callback(onEnd),
        new Nan::Callback(onData),
        dev,
        buf_num,
        buf_len));
}

NAN_METHOD(cancel_async) {
  ASSERT_DEV(info[0], dev)
  CALL_RTLSDR_INT(rtlsdr_cancel_async(dev))
}

NAN_MODULE_INIT(Init) {
  EXPORT_FUNCTION(get_device_count)
  EXPORT_FUNCTION(get_device_name)
  EXPORT_FUNCTION(get_device_usb_strings)
  EXPORT_FUNCTION(get_index_by_serial)
  EXPORT_FUNCTION(open)
  EXPORT_FUNCTION(close)
  EXPORT_FUNCTION(set_xtal_freq)
  EXPORT_FUNCTION(get_xtal_freq)
  EXPORT_FUNCTION(get_usb_strings)
  EXPORT_FUNCTION(write_eeprom)
  EXPORT_FUNCTION(read_eeprom)
  EXPORT_FUNCTION(set_center_freq)
  EXPORT_FUNCTION(get_center_freq)
  EXPORT_FUNCTION(set_freq_correction)
  EXPORT_FUNCTION(get_freq_correction)
  EXPORT_FUNCTION(get_tuner_type)
  EXPORT_FUNCTION(get_tuner_gains)
  EXPORT_FUNCTION(set_tuner_gain)
  EXPORT_FUNCTION(get_tuner_gain)
  EXPORT_FUNCTION(set_tuner_if_gain)
  EXPORT_FUNCTION(set_tuner_gain_mode)
  EXPORT_FUNCTION(set_sample_rate)
  EXPORT_FUNCTION(get_sample_rate)
  EXPORT_FUNCTION(set_testmode)
  EXPORT_FUNCTION(set_agc_mode)
  EXPORT_FUNCTION(set_direct_sampling)
  EXPORT_FUNCTION(get_direct_sampling)
  EXPORT_FUNCTION(set_offset_tuning)
  EXPORT_FUNCTION(get_offset_tuning)
  EXPORT_FUNCTION(reset_buffer)
  EXPORT_FUNCTION(read_sync)
  // EXPORT_FUNCTION(wait_async) // deprecated
  EXPORT_FUNCTION(read_async)
  EXPORT_FUNCTION(cancel_async)
}

NODE_MODULE(rtlsdr, Init)

#undef CDATA
#undef LOCAL_STRING
#undef LOCAL_FUNCTION
#undef EXPORT_NUMBER
#undef EXPORT_FUNCTION
#undef CALL_RTLSDR_INT
#undef CALL_RTLSDR_UINT
#undef CALL_RTLSDR_CONST_CHAR
#undef CDATA_OR_NULL
#undef ASSERT_UINT8
#undef ASSERT_UINT16
#undef ASSERT_UINT
#undef ASSERT_INT
#undef ASSERT_OBJCET
#undef ASSERT_DEV
#undef ASSERT_BUFFER
#undef ASSERT_STRING
#undef ASSERT_FUNCTION
