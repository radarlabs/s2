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

  this->s2polygon = std::make_shared<S2Polygon>();
  Decoder decoder(encoded.Data(), encoded.ByteLength());
  if (!this->s2polygon->Decode(&decoder)) {
    Napi::TypeError::New(env, "malformed ArrayBuffer for S2Polygon.").ThrowAsJavaScriptException();
    return;
  }

  // Report the native allocation to V8 so GC pressure reflects it.
  this->externalMemory = static_cast<int64_t>(this->s2polygon->SpaceUsed());
  Napi::MemoryManagement::AdjustExternalMemory(env, this->externalMemory);
}

void Polygon::Finalize(Napi::BasicEnv env) {
  if (this->externalMemory > 0) {
    Napi::MemoryManagement::AdjustExternalMemory(env, -this->externalMemory);
    this->externalMemory = 0;
  }
}
