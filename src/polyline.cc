#include "polyline.h"

Napi::FunctionReference Polyline::constructor;

Napi::Object Polyline::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Polyline", {
    InstanceMethod("contains", &Polyline::Contains),
    InstanceMethod("nearlyCovers", &Polyline::NearlyCovers),
    InstanceMethod("getLength", &Polyline::GetLength),
    InstanceMethod("getCentroid", &Polyline::GetCentroid),
    InstanceMethod("interpolate", &Polyline::Interpolate),
    InstanceMethod("project", &Polyline::Project),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Polyline", func);
  return exports;
}

Polyline::Polyline(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Polyline>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsArray()) {
    Napi::TypeError::New(env, "(latlngs: s2.LatLng[]) expected.").ThrowAsJavaScriptException();
    return;
  }

  Napi::Array llArray = info[0].As<Napi::Array>();
  uint32_t arrayLength = llArray.Length();
  if (arrayLength <= 0) {
    Napi::RangeError::New(env, "(latlngs: s2.LatLng[]) was empty.").ThrowAsJavaScriptException();
    return;
  }

  std::vector<S2Point> pointVector;
  for (uint32_t i = 0; i < arrayLength; i++) {
    Napi::Value obj = llArray[i];
    LatLng* ll = LatLng::Unwrap(obj.As<Napi::Object>());
    S2Point point = ll->Get().Normalized().ToPoint().Normalize();
    pointVector.push_back(point);
  }

  this->s2polyline = S2Polyline(pointVector, S2Debug::ALLOW);

  S2Error error;
  if (this->s2polyline.FindValidationError(&error)) {
    Napi::Error::New(env, StringPrintf("Polyline is invalid: %s", error.text().c_str())).ThrowAsJavaScriptException();
    return;
  }
}

Napi::Value Polyline::Contains(const Napi::CallbackInfo& info){
  bool contains = false;

  Napi::Env env = info.Env();
  int length = info.Length();

  if (length < 1 || !info[0].IsObject()){
    Napi::TypeError::New(env, "expected cell").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isCell = object.InstanceOf(Cell::constructor.Value());
  if (!isCell) {
    Napi::TypeError::New(env, "Cell expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Cell* cell = Cell::Unwrap(object);
  S2Cell s2cell = cell->Get();

  contains = this->s2polyline.Contains(s2cell);

  return Napi::Boolean::New(info.Env(), contains);
}

Napi::Value Polyline::NearlyCovers(const Napi::CallbackInfo& info){
  bool isCover = false;

  Napi::Env env = info.Env();
  int length = info.Length();

  if (length != 2 || !info[0].IsObject() || !info[1].IsNumber()){
    Napi::TypeError::New(env, "(polyline: Polyline, margin: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isPolyline = object.InstanceOf(Polyline::constructor.Value());
  if (!isPolyline) {
    Napi::TypeError::New(env, "(polyline: Polyline, margin: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  double max_error_degree = info[1].As<Napi::Number>().DoubleValue();
  S1Angle max_error = S1Angle::Degrees(max_error_degree);

  Polyline* polyline = Polyline::Unwrap(object);
  S2Polyline* s2polyline = polyline->Get();

  isCover = this->s2polyline.NearlyCovers(*s2polyline, max_error);

  return Napi::Boolean::New(info.Env(), isCover);
}

S2Polyline* Polyline::Get() {
  return this->s2polyline.Clone();
}

Napi::Value Polyline::GetLength(const Napi::CallbackInfo& info){
  double length = 1;
  Napi::Env env = info.Env();
  S1Angle s1Angle = this->s2polyline.GetLength();
  length = S2Earth::ToMeters(s1Angle);

  return Napi::Number::New(env, length);
}

Napi::Value Polyline::GetCentroid(const Napi::CallbackInfo& info){
  S2Point s2point = this->s2polyline.GetCentroid();
  S2LatLng s2latlng = S2LatLng(s2point);
  return LatLng::NewInstance(
    Napi::Number::New(info.Env(), s2latlng.lat().degrees()),
    Napi::Number::New(info.Env(), s2latlng.lng().degrees())
  );
}

Napi::Value Polyline::Interpolate(const Napi::CallbackInfo& info){
   Napi::Env env = info.Env();
  int length = info.Length();

  if (length != 1 || !info[0].IsNumber()){
    Napi::TypeError::New(env, "expected fraction").ThrowAsJavaScriptException();
    return env.Null();
  }

  double fraction = info[0].As<Napi::Number>().DoubleValue();
  S2Point s2point = this->s2polyline.Interpolate(fraction);
  S2LatLng s2latlng = S2LatLng(s2point);
  return LatLng::NewInstance(
    Napi::Number::New(info.Env(), s2latlng.lat().degrees()),
    Napi::Number::New(info.Env(), s2latlng.lng().degrees())
  );
}

Napi::Value Polyline::Project(const Napi::CallbackInfo& info){
   Napi::Env env = info.Env();
  int length = info.Length();

  if (length != 1 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "(ll: LatLng) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isLatlng = object.InstanceOf(LatLng::constructor.Value());
  if (!isLatlng) {
    Napi::TypeError::New(env, "(ll: LatLng) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  LatLng* ll = LatLng::Unwrap(object);
  S2LatLng sourceLatLng = ll->Get();

  int index;
  S2Point s2point = this->s2polyline.Project(sourceLatLng.ToPoint(), &index);
  S2LatLng s2latlng = S2LatLng(s2point);

  Napi::Object newPoint = LatLng::NewInstance(
    Napi::Number::New(info.Env(), s2latlng.lat().degrees()),
    Napi::Number::New(info.Env(), s2latlng.lng().degrees())
  );

  Napi::Object returnObj = Napi::Object::New(env);
  returnObj.Set("point", newPoint);
  returnObj.Set("index", index);
  return returnObj;
}
