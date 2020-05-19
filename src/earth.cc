#include "earth.h"

Napi::FunctionReference Earth::constructor;

Napi::Object Earth::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Earth", {
    StaticMethod("toMeters", &Earth::ToMeters),
    StaticMethod("toKm", &Earth::ToKm),
    StaticMethod("getRadian", &Earth::GetRadian),
    StaticMethod("getDegree", &Earth::GetDegree),
    StaticMethod("getDistanceKm", &Earth::GetDistanceKm),
    StaticMethod("getDistanceMeters", &Earth::GetDistanceMeters),
    StaticMethod("getInitalBearingDegree", &Earth::GetInitalBearingDegree),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Earth", func);
  return exports;
}

Earth::Earth(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Earth>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
}

bool Earth::PreCheck(const Napi::CallbackInfo &info, S2LatLng &latlng1, S2LatLng &latlng2){
  int length = info.Length();

  if(length < 2 || !info[0].IsObject() || !info[1].IsObject()){
    return false;
  }

  Napi::Object objectA = info[0].As<Napi::Object>();
  bool isLatlng1 = objectA.InstanceOf(LatLng::constructor.Value());
  if (!isLatlng1) {
    return false;
  }

  LatLng* ll1 = LatLng::Unwrap(objectA);
  latlng1 = ll1->Get();

  Napi::Object objectB = info[1].As<Napi::Object>();
  bool isLatlng2 = objectB.InstanceOf(LatLng::constructor.Value());
  if (!isLatlng2) {
    return false;
  }

  LatLng* ll2 = LatLng::Unwrap(objectB);
  latlng2 = ll2->Get();

  return true;
}

Napi::Value Earth::ToMeters(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S1Angle s1Angle = S1Angle(s2LatLngA, s2LatLngB);
  double distance = S2Earth::ToMeters(s1Angle);

  return Napi::Number::New(env, distance);
}

Napi::Value Earth::ToKm(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S1Angle s1Angle = S1Angle(s2LatLngA, s2LatLngB);

  double distance = S2Earth::ToKm(s1Angle);

  return Napi::Number::New(env, distance);
}

Napi::Value Earth::GetRadian(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S1Angle s1Angle = S1Angle(s2LatLngA, s2LatLngB);

  double radians = s1Angle.radians();
  return Napi::Number::New(env, radians);
}

Napi::Value Earth::GetDegree(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S1Angle s1Angle = S1Angle(s2LatLngA, s2LatLngB);

  double degrees = s1Angle.degrees();
  return Napi::Number::New(env, degrees);
}

Napi::Value Earth::GetDistanceKm(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  double distance = S2Earth::GetDistanceKm(s2LatLngA, s2LatLngB);

  return Napi::Number::New(env, distance);
}

Napi::Value Earth::GetDistanceMeters(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  double distance = S2Earth::GetDistanceMeters(s2LatLngA, s2LatLngB);

  return Napi::Number::New(env, distance);
}

Napi::Value Earth::GetInitalBearingDegree(const Napi::CallbackInfo &info){
  Napi::Env env = info.Env();

  S2LatLng s2LatLngA;
  S2LatLng s2LatLngB;

  bool isValid = Earth::PreCheck(info, s2LatLngA, s2LatLngB);
  if(!isValid){
    Napi::TypeError::New(env, "2 Latlng Object expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S1Angle s1Angle = S2Earth::GetInitialBearing(s2LatLngA, s2LatLngB);

  double degrees = s1Angle.degrees();
  return Napi::Number::New(env, degrees);
}

S2Earth Earth::Get() {
  return this->s2earth;
}
