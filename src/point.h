#ifndef RADAR_POINT
#define RADAR_POINT

#include <napi.h>
#include <sstream>
#include "s2/s2point.h"

class Point : public Napi::ObjectWrap<Point> {
  public:
    Point(const Napi::CallbackInfo& info);
    S2Point Get();

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    Napi::Value X(const Napi::CallbackInfo &info);
    Napi::Value Y(const Napi::CallbackInfo &info);
    Napi::Value Z(const Napi::CallbackInfo &info);
    S2Point s2Point;
};

#endif
