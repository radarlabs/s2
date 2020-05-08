#include "polyline.h"

Napi::FunctionReference Polyline::constructor;

Napi::Object Polyline::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Polyline", {
    InstanceMethod("contains", &Polyline::Contains),
    InstanceMethod("nearlyCovers", &Polyline::NearlyCovers),
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

  if(length < 1 || !info[0].IsObject()){
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

  if(length != 2 || !info[0].IsObject() || !info[1].IsNumber()){
    Napi::TypeError::New(env, "expected Polyline and Number").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isPolyline = object.InstanceOf(Polyline::constructor.Value());
  if (!isPolyline) {
    Napi::TypeError::New(env, "Polyline expected.").ThrowAsJavaScriptException();
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


