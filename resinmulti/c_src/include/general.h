#include "erl_nif.h"

#define MY_NUMBER 10
ErlNifResourceType* RES_TYPE;
ErlNifResourceType* rt_obj;
ErlNifResourceType* rt_obj2;
ERL_NIF_TERM atom_ok;
ERL_NIF_TERM atom_true;
ERL_NIF_TERM atom_false;

typedef struct
{
  int count;
} Tracker;

typedef struct {
    int gram;
    double volume;
} Object;

typedef struct {
    const char* name;
    int age;
} Inner;
//ラップしたい，入れ子になってる構造体
typedef struct
{
  int id;
  double height;
  Inner profile;
} Example;
