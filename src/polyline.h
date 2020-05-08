#ifndef RADAR_POLYLINE
#define RADAR_POLYLINE

#include <napi.h>

#include "latlng.h"
#include "cell.h"
#include "s2/s1angle.h"
#include "s2/s2pointutil.h"
#include "s2/s2polyline.h"
#include "s2/base/stringprintf.h"

class Polyline : public Napi::ObjectWrap<Polyline> {
  public:
    Polyline(const Napi::CallbackInfo& info);
    S2Polyline* Get();

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    Napi::Value Contains(const Napi::CallbackInfo &info);
    Napi::Value NearlyCovers(const Napi::CallbackInfo &info);

    S2Polyline s2polyline;
};

#endif