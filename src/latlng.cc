#include "latlng.h"

Napi::FunctionReference LatLng::constructor;

Napi::Object LatLng::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "LatLng", {
    InstanceMethod("toString", &LatLng::ToString),
    InstanceMethod("normalized", &LatLng::Normalized)
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
  if (length <= 0 || !info[0].IsNumber() || !info[1].IsNumber()) {
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
