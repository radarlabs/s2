#ifndef RADAR_LOOP
#define RADAR_LOOP

#include <napi.h>
#include "latlng.h"
#include "s2/s2loop.h"
#include "s2/s2error.h"
#include "s2/base/stringprintf.h"

class Loop : public Napi::ObjectWrap<Loop> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    Loop(const Napi::CallbackInfo& info);

    std::shared_ptr<S2Loop> s2loop;
};

#endif
