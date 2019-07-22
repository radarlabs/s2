#include "polygon.h"

Napi::FunctionReference Polygon::constructor;

Napi::Object Polygon::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Polygon", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Polygon", func);
  return exports;
}

Polygon::Polygon(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Polygon>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length < 1 || !info[0].IsArrayBuffer()) {
    Napi::TypeError::New(env, "(encoded: ArrayBuffer) expected.").ThrowAsJavaScriptException();
    return;
  }

  Napi::ArrayBuffer encoded = info[0].As<Napi::ArrayBuffer>();

  this->s2polygon = new S2Polygon();
  Decoder decoder(encoded.Data(), encoded.ByteLength());
  this->s2polygon->Decode(&decoder);
}

Polygon::~Polygon() {
  delete this->s2polygon;
}

S2Polygon* Polygon::Get() {
  return this->s2polygon;
}
