#ifndef RADAR_CELL
#define RADAR_CELL

#include <napi.h>
#include <sstream>

#include "cell_id.h"
#include "latlng.h"
#include "point.h"
#include "s2/s2cell.h"

class Cell : public Napi::ObjectWrap<Cell> {
  public:
    Cell(const Napi::CallbackInfo& info);
    S2Cell Get();

    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

  private:
    Napi::Value GetVertex(const Napi::CallbackInfo &info);
    Napi::Value GetCenter(const Napi::CallbackInfo &info);
    S2Cell s2Cell;
};

#endif
