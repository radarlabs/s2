#include "cell_union.h"

Napi::FunctionReference CellUnion::constructor;

Napi::Object CellUnion::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "CellUnion", {
    InstanceMethod("contains", &CellUnion::Contains),
    InstanceMethod("intersects", &CellUnion::Intersects),
    InstanceMethod("union", &CellUnion::Union),
    InstanceMethod("intersection", &CellUnion::Intersection),
    InstanceMethod("difference", &CellUnion::Difference),
    InstanceMethod("cellIds", &CellUnion::CellIds),
    InstanceMethod("tokens", &CellUnion::Tokens),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CellUnion", func);
  return exports;
}

CellUnion::CellUnion(const Napi::CallbackInfo &info) : Napi::ObjectWrap<CellUnion>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  const int length = info.Length();
  const string badArgs = "(cellIds: s2.CellId[]) expected.";

  if (
    length <= 0
    || (!info[0].IsArray() && !info[0].IsExternal())
  ) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  if (info[0].IsExternal()) {
    Napi::External<S2CellUnion> external = info[0].As<Napi::External<S2CellUnion>>();
    if (!external.Data()->IsValid()) {
      Napi::TypeError::New(env, "S2 cell union is invalid.").ThrowAsJavaScriptException();
      return;
    }
    this->s2cellunion = *external.Data();
  } else if (info[0].IsArray()) {
    Napi::Array cellIdArray = info[0].As<Napi::Array>();
    uint32_t arrayLength = cellIdArray.Length();
    if (arrayLength <= 0) {
      Napi::RangeError::New(env, "(cellIds: s2.CellId[]) was empty.").ThrowAsJavaScriptException();
      return;
    }

    std::vector<S2CellId> cellIdVector;
    for (uint32_t i = 0; i < arrayLength; i++) {
      Napi::Value obj = cellIdArray[i];
      if (obj.IsObject()) {
        CellId* cellId = CellId::Unwrap(obj.As<Napi::Object>());
        cellIdVector.push_back(cellId->Get());
      } else if (obj.IsString()) {
        Napi::String token = obj.As<Napi::String>();
        cellIdVector.push_back(S2CellId::FromToken(token.Utf8Value()));
      }
    }

    this->s2cellunion = S2CellUnion(std::move(cellIdVector));
  }
}

S2CellUnion CellUnion::Get() {
  return this->s2cellunion;
}

Napi::Value CellUnion::Contains(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const string badArgs = "(other: s2.CellId | s2.CellUnion) expected.";
  bool contains = false;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellUnion::constructor.Value())) {
      CellUnion* otherCellUnion = CellUnion::Unwrap(object);
      contains = s2cellunion.Contains(otherCellUnion->Get());
    } else if (object.InstanceOf(CellId::constructor.Value())) {
      CellId* otherCellId = CellId::Unwrap(object);
      contains = s2cellunion.Contains(otherCellId->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  } else {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, contains);
}

Napi::Value CellUnion::Intersects(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const string badArgs = "(other: s2.CellId | s2.CellUnion) expected.";
  bool intersects = false;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellUnion::constructor.Value())) {
      CellUnion* otherCellUnion = CellUnion::Unwrap(object);
      intersects = s2cellunion.Intersects(otherCellUnion->Get());
    } else if (object.InstanceOf(CellId::constructor.Value())) {
      CellId* otherCellId = CellId::Unwrap(object);
      intersects = s2cellunion.Intersects(otherCellId->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  } else {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  return Napi::Boolean::New(env, intersects);
}

Napi::Value CellUnion::Union(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const string badArgs = "(other: s2.CellUnion) expected.";
  S2CellUnion s2Union;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellUnion::constructor.Value())) {
      CellUnion* otherCellUnion = CellUnion::Unwrap(object);
      s2Union = s2cellunion.Union(otherCellUnion->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  } else {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  return constructor.New({
    Napi::External<S2CellUnion>::New(env, &s2Union)
  });
}

Napi::Value CellUnion::Intersection(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const string badArgs = "(other: s2.CellUnion) expected.";
  S2CellUnion s2Intersection;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellUnion::constructor.Value())) {
      CellUnion* otherCellUnion = CellUnion::Unwrap(object);
      s2Intersection = s2cellunion.Intersection(otherCellUnion->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  } else {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  return constructor.New({
    Napi::External<S2CellUnion>::New(env, new S2CellUnion(s2Intersection))
  });
}

Napi::Value CellUnion::Difference(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  const string badArgs = "(other: s2.CellUnion) expected.";
  S2CellUnion s2Difference;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellUnion::constructor.Value())) {
      CellUnion* otherCellUnion = CellUnion::Unwrap(object);
      s2Difference = s2cellunion.Difference(otherCellUnion->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return env.Null();
    }
  } else {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return env.Null();
  }

  return constructor.New({
    Napi::External<S2CellUnion>::New(env, &s2Difference)
  });
}

Napi::Value CellUnion::CellIds(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();

  uint32_t size = this->s2cellunion.size();
  Napi::Array returnedIds = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    S2CellId cellId = this->s2cellunion[i];
    returnedIds[i] = CellId::constructor.New({ Napi::External<S2CellId>::New(env, &cellId) });
  }

  return returnedIds;
}

Napi::Value CellUnion::Tokens(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();

  uint32_t size = this->s2cellunion.size();
  Napi::Array returnedTokens = Napi::Array::New(env, size);

  for (uint32_t i = 0; i < size; i++) {
    returnedTokens[i] = Napi::String::New(env, this->s2cellunion[i].ToToken());
  }

  return returnedTokens;
}
