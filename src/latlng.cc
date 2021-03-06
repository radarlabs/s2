#include "latlng.h"

Napi::FunctionReference LatLng::constructor;

Napi::Object LatLng::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LatLng", {
    InstanceMethod("latitude", &LatLng::Latitude),
    InstanceMethod("longitude", &LatLng::Longitude),
    InstanceMethod("toString", &LatLng::ToString),
    InstanceMethod("normalized", &LatLng::Normalized),
    InstanceMethod("approxEquals", &LatLng::ApproxEquals)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("LatLng", func);
  return exports;
}

Napi::Object LatLng::NewInstance(Napi::Value lat, Napi::Value lng) {
  return constructor.New({ lat, lng });
}

LatLng::LatLng(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LatLng>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || length > 2) {
    Napi::TypeError::New(env, "(lat: number, lng: number) | Point expected.").ThrowAsJavaScriptException();
    return;
  }

  if (length == 1) { // Point
    if (!info[0].IsObject()) {
      Napi::TypeError::New(env, "Point expected.").ThrowAsJavaScriptException();
      return;
    }
    Napi::Object object = info[0].As<Napi::Object>();
    bool isPoint = object.InstanceOf(Point::constructor.Value());
    if (!isPoint) {
      Napi::TypeError::New(env, "Point expected.").ThrowAsJavaScriptException();
      return;
    }

    Point* point = Point::Unwrap(object);
    this->s2latlng = S2LatLng(point->Get());
  } else { // lat, lng
    if (!info[0].IsNumber() || !info[1].IsNumber()) {
      Napi::TypeError::New(env, "(lat: number, lng: number) expected.").ThrowAsJavaScriptException();
      return;
    }

    Napi::Number lat = info[0].As<Napi::Number>();
    Napi::Number lng = info[1].As<Napi::Number>();

    this->s2latlng = S2LatLng::FromDegrees(
      lat.DoubleValue(),
      lng.DoubleValue()
    );
  }
}


S2LatLng LatLng::Get() {
  return s2latlng;
}

Napi::Value LatLng::ToString(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env(), this->s2latlng.ToStringInDegrees());
}

Napi::Value LatLng::Normalized(const Napi::CallbackInfo& info) {
  S2LatLng normalized = this->s2latlng.Normalized();

  return LatLng::NewInstance(
    Napi::Number::New(info.Env(), normalized.lat().degrees()),
    Napi::Number::New(info.Env(), normalized.lng().degrees())
  );
}

Napi::Value LatLng::Latitude(const Napi::CallbackInfo& info) {
  return Napi::Number::New(
    info.Env(),
    this->s2latlng.lat().degrees()
  );
}

Napi::Value LatLng::Longitude(const Napi::CallbackInfo& info) {
  return Napi::Number::New(
    info.Env(),
    this->s2latlng.lng().degrees()
  );
}

Napi::Value LatLng::ApproxEquals(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  int length = info.Length();
  S1Angle maxError = S1Angle::Radians(1e-15);

  if (length < 1 || length > 2) {
    Napi::TypeError::New(env, "(ll: LatLng, maxErrorRadians?: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isLatlng = object.InstanceOf(LatLng::constructor.Value());
  if (!isLatlng) {
    Napi::TypeError::New(env, "(ll: LatLng, maxErrorRadians?: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  LatLng* ll = LatLng::Unwrap(object);
  S2LatLng s2LatLng = ll->Get();

  if (length == 2) {
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "(ll: LatLng, maxErrorRadians?: number) expected.").ThrowAsJavaScriptException();
      return env.Null();
    }

    double radians = info[1].As<Napi::Number>().DoubleValue();
    maxError = S1Angle::Radians(radians);
  }

  bool isEqual = this->s2latlng.ApproxEquals(s2LatLng, maxError);
  return Napi::Boolean::New(info.Env(), isEqual);
}
