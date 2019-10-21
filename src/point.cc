#include "point.h"

Napi::FunctionReference Point::constructor;

Napi::Object Point::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Point", {
    InstanceMethod("x", &Point::X),
    InstanceMethod("y", &Point::Y),
    InstanceMethod("z", &Point::Z),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Point", func);
  return exports;
}

Point::Point(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Point>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  string badArgs = "(number, number, number) expected.";

  if (length != 1 && length != 3) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  if (length == 1) { // S2 Point
    if (!info[0].IsExternal()) {
      Napi::TypeError::New(env, "S2Point expected.").ThrowAsJavaScriptException();
      return;
    }
    Napi::External<S2Point> external = info[0].As<Napi::External<S2Point>>();
    this->s2Point = *external.Data();

  } else { // (x, y, z)
    if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber()) {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return;
    }

    this->s2Point = S2Point(
      info[0].As<Napi::Number>().DoubleValue(),
      info[1].As<Napi::Number>().DoubleValue(),
      info[2].As<Napi::Number>().DoubleValue()
    );
  }
}

Napi::Value Point::X(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2Point.x());
}

Napi::Value Point::Y(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2Point.y());
}

Napi::Value Point::Z(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2Point.z());
}

S2Point Point::Get() {
  return s2Point;
}
