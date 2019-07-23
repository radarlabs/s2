#include "builder.h"

using absl::make_unique;
using s2builderutil::S2PolygonLayer;

Napi::FunctionReference Builder::constructor;

Napi::Object Builder::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Builder", {
    InstanceMethod("addLoop", &Builder::AddLoop),
    InstanceMethod("build", &Builder::Build)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Builder", func);
  return exports;
}

Builder::Builder(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Builder>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->s2builderOptions = make_unique<S2Builder::Options>();
  this->s2builder = make_unique<S2Builder>(*this->s2builderOptions);
}

Napi::Value Builder::AddLoop(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (info.Length() <= 0 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "(loop: s2.Loop) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object object = info[0].As<Napi::Object>();
  bool isLoop = object.InstanceOf(Loop::constructor.Value());
  if (!isLoop) {
    Napi::TypeError::New(env, "(loop: s2.Loop) expected.").ThrowAsJavaScriptException();
    return env.Null();
  }

  Loop* loop = Loop::Unwrap(object);

  this->output = std::make_unique<S2Polygon>();
  this->s2builder->StartLayer(
    std::make_unique<S2PolygonLayer>(this->output.get())
  );
  this->s2builder->AddLoop(*loop->s2loop);

  return env.Null();
}

Napi::Value Builder::Build(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (this->output == nullptr) {
    Napi::Error::New(env, "Add a loop first.").ThrowAsJavaScriptException();
    return env.Null();
  }

  S2Error error;
  bool buildOk = this->s2builder->Build(&error);
  if (!buildOk) {
    Napi::Error::New(env, StringPrintf("Build failed: %s", error.text().c_str())).ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!this->output->IsValid()) {
    Napi::Error::New(env, "Output is invalid.").ThrowAsJavaScriptException();
    return env.Null();
  }

  // encode the output and return it as a JS object
  Encoder encoder;
  this->output->Encode(&encoder);

  return Polygon::constructor.New({
    Napi::ArrayBuffer::New(env, (void *)encoder.base(), encoder.length())
  });
}
