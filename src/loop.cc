#include "loop.h"
#include "s2/s2pointutil.h"

Napi::FunctionReference Loop::constructor;

Napi::Object Loop::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Loop", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Loop", func);
  return exports;
}

Loop::Loop(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Loop>(info)  {
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

  this->s2loop = std::make_shared<S2Loop>(pointVector, S2Debug::ALLOW);

  S2Error error;
  if (this->s2loop->FindValidationError(&error)) {
    Napi::Error::New(env, StringPrintf("Loop is invalid: %s", error.text().c_str())).ThrowAsJavaScriptException();
    return;
  }

  // Report the native allocation to V8 so GC pressure reflects it.
  this->externalMemory = static_cast<int64_t>(this->s2loop->SpaceUsed());
  Napi::MemoryManagement::AdjustExternalMemory(env, this->externalMemory);
}

void Loop::Finalize(Napi::BasicEnv env) {
  if (this->externalMemory > 0) {
    Napi::MemoryManagement::AdjustExternalMemory(env, -this->externalMemory);
    this->externalMemory = 0;
  }
}
