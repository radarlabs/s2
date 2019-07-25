#include "region_coverer.h"

using absl::make_unique;

Napi::FunctionReference RegionCoverer::constructor;

Napi::Object RegionCoverer::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "RegionCoverer", {
    InstanceMethod("getCovering", &RegionCoverer::GetCovering),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("RegionCoverer", func);
  return exports;
}

RegionCoverer::RegionCoverer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<RegionCoverer>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->options = make_unique<S2RegionCoverer::Options>();

  int length = info.Length();
  if (length >= 1 && info[0].IsObject()) {
    Napi::Object optionsObject = info[0].As<Napi::Object>();

    // TODO(jeffk): support more options
    Napi::Value minLevelRaw = optionsObject["min"];
    Napi::Value maxLevelRaw = optionsObject["max"];
    Napi::Value maxCellsRaw = optionsObject["max_cells"];

    if (minLevelRaw.IsNumber()) {
      this->options->set_min_level(minLevelRaw.As<Napi::Number>().Uint32Value());
    }
    if (maxLevelRaw.IsNumber()) {
      this->options->set_max_level(maxLevelRaw.As<Napi::Number>().Uint32Value());
    }
    if (maxCellsRaw.IsNumber()) {
      this->options->set_max_cells(maxCellsRaw.As<Napi::Number>().Uint32Value());
    }

    this->s2regioncoverer = make_unique<S2RegionCoverer>(*options);
  }
}

Napi::Value RegionCoverer::GetCovering(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();
  string badArgs = "(polygon: s2.Polygon) expected.";

  if (length <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isPolygon = object.InstanceOf(Polygon::constructor.Value());
  if (!isPolygon) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Polygon* polygon = Polygon::Unwrap(object);

  std::vector<S2CellId> cellIds;
  this->s2regioncoverer->GetCovering(*polygon->s2polygon, &cellIds);

  uint32_t size = cellIds.size();
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = CellId::FromTokenString(env, cellIds[i].ToToken());
  }

  return returnedIds;
}
