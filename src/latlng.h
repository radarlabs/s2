#ifndef RADAR_LATLNG
#define RADAR_LATLNG

#include <napi.h>
#include "s2/s2latlng.h"

class LatLng : public Napi::ObjectWrap<LatLng> {

  public:
    LatLng(const Napi::CallbackInfo& info);

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Value lat, Napi::Value lng);

    S2LatLng Get();


  private:
    Napi::Value ToString(const Napi::CallbackInfo &info);
    Napi::Value Normalized(const Napi::CallbackInfo &info);
    S2LatLng s2latlng;
};

#endif
