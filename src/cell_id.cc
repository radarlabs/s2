#include "cell_id.h"

Napi::FunctionReference CellId::constructor;

Napi::Object CellId::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "CellId", {
    InstanceMethod("id", &CellId::Id),
    InstanceMethod("token", &CellId::Token),
    InstanceMethod("contains", &CellId::Contains),
    InstanceMethod("intersects", &CellId::Intersects),
    InstanceMethod("parent", &CellId::Parent),
    InstanceMethod("child", &CellId::Child),
    InstanceMethod("next", &CellId::Next),
    InstanceMethod("level", &CellId::Level),
    InstanceMethod("isLeaf", &CellId::IsLeaf),
    StaticMethod("fromToken", &CellId::FromToken),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("CellId", func);
  return exports;
}

CellId::CellId(const Napi::CallbackInfo& info) : Napi::ObjectWrap<CellId>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  string badArgs = "(id: string) | (ll: LatLng) expected.";

  if (
    length <= 0
    || (!info[0].IsString() && !info[0].IsObject() && !info[0].IsExternal())
  ) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  if (info[0].IsString()) {         // id: string
    Napi::String idString = info[0].As<Napi::String>();
    uint64_t id;
    std::istringstream iss(idString.Utf8Value());
    iss >> id;
    this->s2cellid = S2CellId(id);
  } else if (info[0].IsObject()) {  // ll: s2.LatLng
    Napi::Object object = info[0].As<Napi::Object>();
    bool isLL = object.InstanceOf(LatLng::constructor.Value());
    if (isLL) {
      LatLng* ll = LatLng::Unwrap(object);
      this->s2cellid = S2CellId(ll->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return;
    }
  } else if (info[0].IsExternal()) { // S2CellId C++ class
    Napi::External<S2CellId> external = info[0].As<Napi::External<S2CellId>>();
    this->s2cellid = *external.Data();
  }
}

Napi::Value CellId::Id(const Napi::CallbackInfo &info) {
  std::ostringstream idStr;
  idStr << s2cellid.id();
  return Napi::String::New(info.Env(), idStr.str());
}

Napi::Value CellId::Token(const Napi::CallbackInfo &info) {
  return Napi::String::New(info.Env(), s2cellid.ToToken());
}

Napi::Value CellId::Contains(const Napi::CallbackInfo &info) {
  bool contains = false;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellId::constructor.Value())) {
      CellId* otherCellId = CellId::Unwrap(object);
      contains = s2cellid.contains(otherCellId->Get());
    }
  }

  return Napi::Boolean::New(info.Env(), contains);
}

Napi::Value CellId::Intersects(const Napi::CallbackInfo &info) {
  bool intersects = false;

  if (info.Length() > 0 && info[0].IsObject()) {
    Napi::Object object = info[0].As<Napi::Object>();
    if (object.InstanceOf(CellId::constructor.Value())) {
      CellId* otherCellId = CellId::Unwrap(object);
      intersects = s2cellid.intersects(otherCellId->Get());
    }
  }

  return Napi::Boolean::New(info.Env(), intersects);
}

Napi::Value CellId::Parent(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (s2cellid.is_face()) {
    return info.This();
  }

  if (info.Length() <= 0 || !info[0].IsNumber()) {
    S2CellId parent = s2cellid.parent();
    return constructor.New({ Napi::External<S2CellId>::New(env, &parent) });
  }

  int level = info[0].As<Napi::Number>().Int32Value();
  int finalLevel = level <= 0 ? 0 : level;

  S2CellId parent = s2cellid.parent(finalLevel);
  return constructor.New({ Napi::External<S2CellId>::New(env, &parent) });
}

Napi::Value CellId::Child(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(info.Env(), "(position: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Number position = info[0].As<Napi::Number>();
  S2CellId child = s2cellid.child(position.Int32Value());

  if (!child.is_valid()) {
    return info.This();
  }
  return constructor.New({ Napi::External<S2CellId>::New(env, &child) });
}

Napi::Value CellId::Next(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  S2CellId next = s2cellid.next();

  if (!next.is_valid()) {
    return info.This();
  }
  return constructor.New({ Napi::External<S2CellId>::New(env, &next) });
}

Napi::Value CellId::Level(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2cellid.level());
}

Napi::Value CellId::IsLeaf(const Napi::CallbackInfo &info) {
  return Napi::Boolean::New(info.Env(), s2cellid.is_leaf());
}

Napi::Value CellId::FromToken(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0 && info[0].IsString()) {
    Napi::String token = info[0].As<Napi::String>();
    S2CellId cellId = S2CellId::FromToken(token);
    if (cellId.is_valid()) {
      return constructor.New({ Napi::External<S2CellId>::New(env, &cellId) });
    }
  }
  Napi::TypeError::New(env, "(token: string) expected.").ThrowAsJavaScriptException();
  return env.Null();
}

S2CellId CellId::Get() {
  return s2cellid;
}
