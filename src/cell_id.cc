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
    InstanceMethod("level", &CellId::Level),
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
  string badArgs = "(id: number) | (ll: LatLng) expected.";

  if (length <= 0 || (!info[0].IsNumber() && !info[0].IsObject())) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  if (info[0].IsNumber()) { // id
    Napi::Number id = info[0].As<Napi::Number>();
    this->s2cellid = S2CellId(id.DoubleValue());
  } else if (info[0].IsObject()) { // ll
    Napi::Object object = info[0].As<Napi::Object>();
    bool isLL = object.InstanceOf(LatLng::constructor.Value());
    if (isLL) {
      LatLng* ll = LatLng::Unwrap(object);
      this->s2cellid = S2CellId(ll->Get());
    } else {
      Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
      return;
    }
  }
}

Napi::Value CellId::Id(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2cellid.id());
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
  Napi::Number parentId;
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsNumber()) {
    parentId =
        s2cellid.level() <= 0 ? Napi::Number::New(env, s2cellid.id())
                              : Napi::Number::New(env, s2cellid.parent().id());
  } else {
    int level = info[0].As<Napi::Number>().Int32Value();
    int finalLevel = level <= 0 ? 0 : level;
    parentId = Napi::Number::New(env, s2cellid.parent(finalLevel).id());
  }
  return constructor.New({ parentId });
}

Napi::Value CellId::Child(const Napi::CallbackInfo &info) {
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(info.Env(), "(position: number) expected.").ThrowAsJavaScriptException();
    return info.Env().Null();
  }

  Napi::Number position = info[0].As<Napi::Number>();
  return constructor.New({
    Napi::Number::New(
      info.Env(),
      s2cellid.child(position.Int32Value()).id()
    )
  });
}

Napi::Value CellId::Level(const Napi::CallbackInfo &info) {
  return Napi::Number::New(info.Env(), s2cellid.level());
}

S2CellId CellId::Get() {
  return s2cellid;
}
