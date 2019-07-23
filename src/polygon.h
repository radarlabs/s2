#ifndef RADAR_POLYGON
#define RADAR_POLYGON

#include <napi.h>
#include "s2/s2polygon.h"
#include "s2/s2error.h"
#include "s2/base/stringprintf.h"

class Polygon : public Napi::ObjectWrap<Polygon> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Polygon(const Napi::CallbackInfo& info);

    std::shared_ptr<S2Polygon> s2polygon;
};

#endif
