#include "cell.h"

Napi::FunctionReference Cell::constructor;

Napi::Object Cell::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Cell", {
    InstanceMethod("getVertex", &Cell::GetVertex),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Cell", func);
  return exports;
}

Cell::Cell(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Cell>(info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  string badArgs = "CellId expected.";

  if (length <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isCellId = object.InstanceOf(CellId::constructor.Value());
  if (!isCellId) {
    Napi::TypeError::New(env, badArgs).ThrowAsJavaScriptException();
    return;
  }

  CellId* cellId = CellId::Unwrap(object);
  this->s2Cell = S2Cell(cellId->Get());
}

Napi::Value Cell::GetVertex(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "(vertex: number) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Number vertex = info[0].As<Napi::Number>();
  S2Point point = s2Cell.GetVertex(vertex);

  return Point::constructor.New({
    Napi::External<S2Point>::New(env, &point)
  });
}

S2Cell Cell::Get() {
  return s2Cell;
}
