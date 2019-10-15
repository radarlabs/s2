#include "region_coverer.h"

using absl::make_unique;

Napi::FunctionReference RegionCoverer::constructor;

Napi::Object RegionCoverer::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "RegionCoverer", {
    StaticMethod("getCovering", &RegionCoverer::GetCoveringCellUnion),
    StaticMethod("getCoveringIds", &RegionCoverer::GetCoveringIds),
    StaticMethod("getCoveringTokens", &RegionCoverer::GetCoveringTokens),

    StaticMethod("getRadiusCovering", &RegionCoverer::GetRadiusCoveringCellUnion),
    StaticMethod("getRadiusCoveringIds", &RegionCoverer::GetRadiusCoveringIds),
    StaticMethod("getRadiusCoveringTokens", &RegionCoverer::GetRadiusCoveringTokens),
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

Napi::Value RegionCoverer::GetCoveringIds(const Napi::CallbackInfo &info) {
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
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

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

  S2Error loopError;
  S2Error buildError;
  S2Error outputError;

  S2CellUnion covering = GetCovering(
    env,
    vertices,
    options,
    loopError,
    buildError,
    outputError
  );

  if (!loopError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Loop is invalid: %d %s", loopError.code(), loopError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!buildError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Build failed: %d %s", buildError.code(), buildError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!outputError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Output is invalid: %d %s", outputError.code(), outputError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!covering.IsValid()) {
    Napi::Error::New(env, "Covering is invalid").ThrowAsJavaScriptException();
    return env.Null();
  }

  uint32_t size = covering.size();
  Napi::BigUint64Array returnedIds = Napi::BigUint64Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = covering[i].id();
  }

  return returnedIds;
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
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

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

  S2Error loopError;
  S2Error buildError;
  S2Error outputError;

  S2CellUnion covering = GetCovering(
    env,
    vertices,
    options,
    loopError,
    buildError,
    outputError
  );

  if (!loopError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Loop is invalid: %d %s", loopError.code(), loopError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!buildError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Build failed: %d %s", buildError.code(), buildError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!outputError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Output is invalid: %d %s", outputError.code(), outputError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!covering.IsValid()) {
    Napi::Error::New(env, "Covering is invalid").ThrowAsJavaScriptException();
    return env.Null();
  }

  uint32_t size = covering.size();
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = Napi::String::New(env, covering[i].ToToken());
  }

  return returnedIds;
}

Napi::Value RegionCoverer::GetCoveringCellUnion(const Napi::CallbackInfo &info) {
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
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

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

  S2Error loopError;
  S2Error buildError;
  S2Error outputError;

  S2CellUnion covering = GetCovering(
    env,
    vertices,
    options,
    loopError,
    buildError,
    outputError
  );

  if (!loopError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Loop is invalid: %d %s", loopError.code(), loopError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!buildError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Build failed: %d %s", buildError.code(), buildError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!outputError.ok()) {
    Napi::Error::New(
      env,
      StringPrintf("Output is invalid: %d %s", outputError.code(), outputError.text().c_str())
    ).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!covering.IsValid()) {
    Napi::Error::New(env, "Covering is invalid").ThrowAsJavaScriptException();
    return env.Null();
  }

  return CellUnion::constructor.New({
    Napi::External<S2CellUnion>::New(env, &covering)
  });
}

Napi::Value RegionCoverer::GetRadiusCoveringTokens(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();
  string badArgs = "(latlng: s2.LatLng, radiusM: number, options: { min?: number, max?: number, max_cells?: number }) expected.";

  if (
    length < 3
    || !info[0].IsObject()
    || !info[1].IsNumber()
    || !info[2].IsObject()
  ) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object optionsObject = info[2].As<Napi::Object>();
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

  LatLng* ll = LatLng::Unwrap(info[0].As<Napi::Object>());
  double radiusM = info[1].As<Napi::Number>().DoubleValue();

  S2CellUnion covering = GetRadiusCovering(ll, radiusM, options);

  uint32_t size = covering.size();
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = Napi::String::New(env, covering[i].ToToken());
  }

  return returnedIds;
}

Napi::Value RegionCoverer::GetRadiusCoveringIds(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();
  string badArgs = "(latlng: s2.LatLng, radiusM: number, options: { min?: number, max?: number, max_cells?: number }) expected.";

  if (
    length < 3
    || !info[0].IsObject()
    || !info[1].IsNumber()
    || !info[2].IsObject()
  ) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object optionsObject = info[2].As<Napi::Object>();
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

  LatLng* ll = LatLng::Unwrap(info[0].As<Napi::Object>());
  double radiusM = info[1].As<Napi::Number>().DoubleValue();

  S2CellUnion covering = GetRadiusCovering(ll, radiusM, options);

  uint32_t size = covering.size();
  Napi::BigUint64Array returnedIds = Napi::BigUint64Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedIds[i] = covering[i].id();
  }

  return returnedIds;
}

Napi::Value RegionCoverer::GetRadiusCoveringCellUnion(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  int length = info.Length();
  string badArgs = "(latlng: s2.LatLng, radiusM: number, options: { min?: number, max?: number, max_cells?: number }) expected.";

  if (
    length < 3
    || !info[0].IsObject()
    || !info[1].IsNumber()
    || !info[2].IsObject()
  ) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object optionsObject = info[2].As<Napi::Object>();
  S2RegionCoverer::Options options;
  GetS2Options(optionsObject, options);

  LatLng* ll = LatLng::Unwrap(info[0].As<Napi::Object>());
  double radiusM = info[1].As<Napi::Number>().DoubleValue();

  S2CellUnion covering = GetRadiusCovering(ll, radiusM, options);
  return CellUnion::constructor.New({
    Napi::External<S2CellUnion>::New(env, &covering)
  });
}

void RegionCoverer::GetS2Options(
  Napi::Object optionsObject,
  S2RegionCoverer::Options &options
) {
  Napi::Value minLevelRaw = optionsObject["min"];
  Napi::Value maxLevelRaw = optionsObject["max"];
  Napi::Value maxCellsRaw = optionsObject["max_cells"];

  if (minLevelRaw.IsNumber()) {
    options.set_min_level(minLevelRaw.As<Napi::Number>().Uint32Value());
  }
  if (maxLevelRaw.IsNumber()) {
    options.set_max_level(maxLevelRaw.As<Napi::Number>().Uint32Value());
  }
  if (maxCellsRaw.IsNumber()) {
    options.set_max_cells(maxCellsRaw.As<Napi::Number>().Uint32Value());
  }
}

S2CellUnion RegionCoverer::GetCovering(
  Napi::Env env,
  std::vector<S2Point> &vertices,
  S2RegionCoverer::Options &coverOptions,
  S2Error &loopError,
  S2Error &buildError,
  S2Error &outputError
) {
  S2Loop loop(vertices, S2Debug::ALLOW);
  loop.Normalize();

  bool loopOk = !loop.FindValidationError(&loopError);
  if (!loopOk) { return S2CellUnion(); }

  S2Builder::Options builderOptions;
  S2Builder builder(builderOptions);

  S2Polygon output;
  builder.StartLayer(std::make_unique<s2builderutil::S2PolygonLayer>(&output));
  builder.AddLoop(loop);

  bool buildOk = builder.Build(&buildError);
  if (!buildOk) { return S2CellUnion(); }

  bool outputOk = !output.FindValidationError(&outputError);
  if (!outputOk) { return S2CellUnion(); }

  S2RegionCoverer coverer(coverOptions);
  return coverer.GetCovering(output);
}

S2CellUnion RegionCoverer::GetRadiusCovering(
  LatLng* ll,
  double radiusM,
  S2RegionCoverer::Options &coverOptions
) {
  S2Point point = ll->Get().Normalized().ToPoint().Normalize();
  S1Angle angle = S1Angle::Radians(S2Earth::MetersToRadians(radiusM));
  S2Cap s2cap(point, angle);
  if (!s2cap.is_valid()) {
    return S2CellUnion();
  }
  S2RegionCoverer coverer(coverOptions);
  return coverer.GetCovering(s2cap);
}
