#ifndef RADAR_CELL_UNION
#define RADAR_CELL_UNION

#include <napi.h>
#include "cell_id.h"
#include "s2/s2cell_union.h"

class CellUnion : public Napi::ObjectWrap<CellUnion> {

  public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    CellUnion(const Napi::CallbackInfo& info);

    S2CellUnion Get();

  private:
    S2CellUnion s2cellunion;

    Napi::Value Contains(const Napi::CallbackInfo &info);
    Napi::Value Intersects(const Napi::CallbackInfo &info);

    Napi::Value Union(const Napi::CallbackInfo &info);
    Napi::Value Intersection(const Napi::CallbackInfo &info);
    Napi::Value Difference(const Napi::CallbackInfo &info);

    Napi::Value Ids(const Napi::CallbackInfo &info);
    Napi::Value CellIds(const Napi::CallbackInfo &info);
    Napi::Value Tokens(const Napi::CallbackInfo &info);

};

#endif
