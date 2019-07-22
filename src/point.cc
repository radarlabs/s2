#include "point.h"

Napi::FunctionReference Point::constructor;

Napi::Object Point::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Point", {
    InstanceMethod("plusOne", &Point::PlusOne),
    InstanceMethod("value", &Point::GetValue),
    InstanceMethod("multiply", &Point::Multiply)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Point", func);
  return exports;
}

Point::Point(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Point>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value = info[0].As<Napi::Number>();
  this->value_ = value.DoubleValue();
}

Napi::Value Point::GetValue(const Napi::CallbackInfo& info) {
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}

Napi::Value Point::PlusOne(const Napi::CallbackInfo& info) {
  this->value_ = this->value_ + 1;

  return Point::GetValue(info);
}

Napi::Value Point::Multiply(const Napi::CallbackInfo& info) {
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    multiple = Napi::Number::New(info.Env(), 1);
  } else {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New({ Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue()) });

  return obj;
}
