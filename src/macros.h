#ifndef __RTLSDR_MACROS_H
#define __RTLSDR_MACROS_H

#define CDATA(buf) (char *) node::Buffer::Data(buf)
#define LOCAL_STRING(str) Nan::New<String>(str).ToLocalChecked()
#define LOCAL_FUNCTION(fn) Nan::GetFunction(Nan::New<FunctionTemplate>(fn)).ToLocalChecked()
#define EXPORT_NUMBER(name) Nan::Set(target, LOCAL_STRING(#name), Nan::New<Number>(name));
#define EXPORT_FUNCTION(name) Nan::Set(target, LOCAL_STRING(#name), LOCAL_FUNCTION(name));

#define CALL_RTLSDR_INT(fn) \
  int ret = fn; \
  info.GetReturnValue().Set(Nan::New(ret));

#define CALL_RTLSDR_UINT(fn) \
  uint32_t ret = fn; \
  info.GetReturnValue().Set(Nan::New(ret));

#define CALL_RTLSDR_CONST_CHAR(fn) \
  const char *ret = fn; \
  info.GetReturnValue().Set(Nan::New(ret).ToLocalChecked());

#define CDATA_OR_NULL(name, var) \
  char *var = NULL; \
  if (name->IsObject()) { \
    Local<Object> buf = name->ToObject(); \
    var = CDATA(buf); \
  }

#define ASSERT_UINT8(name, var) \
  if (!name->IsNumber()) { \
    Nan::ThrowError(#var " must be a number"); \
    return; \
  } \
  uint8_t var = name->IntegerValue();

#define ASSERT_UINT16(name, var) \
  if (!name->IsNumber()) { \
    Nan::ThrowError(#var " must be a number"); \
    return; \
  } \
  uint16_t var = name->IntegerValue();

#define ASSERT_UINT(name, var) \
  if (!name->IsNumber()) { \
    Nan::ThrowError(#var " must be a number"); \
    return; \
  } \
  uint32_t var = name->IntegerValue();

#define ASSERT_INT(name, var) \
  if (!name->IsNumber()) { \
    Nan::ThrowError(#var " must be a number"); \
    return; \
  } \
  int var = name->IntegerValue();

#define ASSERT_OBJECT(name, var) \
  if (!name->IsObject()) { \
    Nan::ThrowError(#var " must be an object"); \
    return; \
  } \
  Local<Object> var = name->ToObject();

#define ASSERT_DEV(name, var) \
  if (!name->IsObject()) { \
    Nan::ThrowError(#var " must be an object"); \
    return; \
  } \
  Local<Object> jsdev = name->ToObject(); \
  rtlsdr_dev_t *var = (struct rtlsdr_dev *)RTLSDRDevice::Resolve(jsdev);

#define ASSERT_BUFFER(name, var) \
  if (!name->IsObject()) { \
    Nan::ThrowError(#var " must be a buffer"); \
    return; \
  } \
  Local<Object> var = name->ToObject();

#define ASSERT_STRING(name, var) \
  if (!name->IsObject()) { \
    Nan::ThrowError(#var " must be a string"); \
    return; \
  } \
  Local<String> var = name->ToString();

#define ASSERT_FUNCTION(name, var) \
  if (!name->IsFunction()) { \
    Nan::ThrowError(#var " must be a function"); \
    return; \
  } \
  Local<Function> var = name.As<Function>();

#endif
