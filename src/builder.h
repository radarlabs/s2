#ifndef RADAR_BUILDER
#define RADAR_BUILDER

#include <napi.h>
#include "loop.h"
#include "polygon.h"
#include "s2/s2builder.h"
#include "s2/s2builderutil_s2polygon_layer.h"
#include "s2/s2polygon.h"
#include "s2/third_party/absl/memory/memory.h"

class Builder : public Napi::ObjectWrap<Builder> {

  public:
    Builder(const Napi::CallbackInfo& info);
    ~Builder();

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    S2Builder* s2builder;
    S2Builder::Options* s2builderOptions;
    S2Polygon* output;

    Napi::Value AddLoop(const Napi::CallbackInfo &info);
    Napi::Value Build(const Napi::CallbackInfo &info);

};

#endif
