#include "region_coverer.h"

using absl::make_unique;

Napi::FunctionReference RegionCoverer::constructor;

Napi::Object RegionCoverer::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "RegionCoverer", {
    StaticMethod("getCoveringTokens", &RegionCoverer::GetCoveringTokens),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("RegionCoverer", func);
  return exports;
}

RegionCoverer::RegionCoverer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<RegionCoverer>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
}

Napi::Value RegionCoverer::GetCoveringTokens(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();
  string badArgs = "(latlngs: s2.LatLng[], options: { min?: number, max?: number, max_cells?: number }) expected.";

  if (length <= 1 || !info[0].IsArray() || !info[1].IsObject()) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Array llArray = info[0].As<Napi::Array>();
  uint32_t arrayLength = llArray.Length();
  if (arrayLength <= 0) {
    Napi::TypeError::New(env, "(latlngs: s2.LatLng[]) was empty.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object optionsObject = info[1].As<Napi::Object>();

  Napi::Value minLevelRaw = optionsObject["min"];
  Napi::Value maxLevelRaw = optionsObject["max"];
  Napi::Value maxCellsRaw = optionsObject["max_cells"];

  S2RegionCoverer::Options options;
  if (minLevelRaw.IsNumber()) {
    options.set_min_level(minLevelRaw.As<Napi::Number>().Uint32Value());
  }
  if (maxLevelRaw.IsNumber()) {
    options.set_max_level(maxLevelRaw.As<Napi::Number>().Uint32Value());
  }
  if (maxCellsRaw.IsNumber()) {
    options.set_max_cells(maxCellsRaw.As<Napi::Number>().Uint32Value());
  }

  std::vector<S2Point> vertices;
  for (uint32_t i = 0; i < arrayLength; i++) {
    Napi::Value obj = llArray[i];
    if (obj.IsObject()) {
      LatLng* ll = LatLng::Unwrap(obj.As<Napi::Object>());
      S2Point point = ll->Get().Normalized().ToPoint().Normalize();
      vertices.push_back(point);
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  S2Loop loop(vertices, S2Debug::ALLOW);
  S2Error loopError;
  if (loop.FindValidationError(&loopError)) {
    Napi::Error::New(
      env,
      StringPrintf("Loop is invalid: %s", loopError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  S2Builder::Options builderOptions;
  S2Builder builder(builderOptions);

  S2Polygon output;
  builder.StartLayer(std::make_unique<s2builderutil::S2PolygonLayer>(&output));
  builder.AddLoop(loop);

  S2Error error;
  bool buildOk = builder.Build(&error);
  if (!buildOk) {
    Napi::Error::New(env, StringPrintf("Build failed: %s", error.text().c_str())).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!output.IsValid()) {
    Napi::Error::New(env, "Output is invalid.").ThrowAsJavaScriptException();
    return env.Null();
  }

  std::vector<S2CellId> covering;
  S2RegionCoverer coverer(options);
  coverer.GetCovering(output, &covering);

  uint32_t size = covering.size();
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = Napi::String::New(env, covering[i].ToToken());
  }

  return returnedIds;
}
