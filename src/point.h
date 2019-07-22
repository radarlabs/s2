#ifndef RADAR_POINT
#define RADAR_POINT

#include <napi.h>

class Point : public Napi::ObjectWrap<Point> {

  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Point(const Napi::CallbackInfo& info);

  private:
      static Napi::FunctionReference constructor;

      Napi::Value GetValue(const Napi::CallbackInfo &info);
      Napi::Value PlusOne(const Napi::CallbackInfo &info);
      Napi::Value Multiply(const Napi::CallbackInfo &info);

      double value_;
};

#endif
