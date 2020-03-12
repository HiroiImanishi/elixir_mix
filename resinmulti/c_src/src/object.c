
#include "erl_nif.h"
#include "../include/general.h"

void free_res_obj(ErlNifEnv* env, void* obj){
    printf("enter obj free\n");
    Object* objptr = (Object*) enif_priv_data(env);
}

//空のリソースオブジェクトを作ってあとで使う場合
ERL_NIF_TERM nif_cre_empty(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    Object* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_obj2,sizeof(Object));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    return ret;
}
//からのオブジェクトに値を入れる
void fillin_obj(Object* obj){
    obj->gram = 100;
    obj->volume = 123.4;
}
//fillinobjのNIF関数
ERL_NIF_TERM nif_fillin_obj(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    Object* res;
    ERL_NIF_TERM ret;
    if(!enif_get_resource(env, argv[0], rt_obj2, (void**) &res)){
	    return enif_make_badarg(env);
    }
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    fillin_obj(res);
    return atom_ok;
}
//Objectに入れた値を調べる関数
ERL_NIF_TERM
nif_read_obj(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    printf("nif_read_obj\n");
    Object* res;
    
    if(argc != 1)
    {
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[0], rt_obj2, (void**) &res))
    {
	  return enif_make_badarg(env);
    }
    return enif_make_tuple2(env, enif_make_int(env,res->gram),enif_make_double(env,res->volume));

}

static ErlNifFunc nif_funcs_obj[]={
  {"cre_empty",0,nif_cre_empty},
  {"fillin_obj",1,nif_fillin_obj},
  {"read_obj",1,nif_read_obj},
}; 
/*
void
free_res(ErlNifEnv* env, void* obj)
{
    printf("enter free_res\n");
    //Object* object = (Object*) enif_priv_data(env);
}
*/
/*
void free_res_obj(ErlNifEnv* env, void* obj){
    printf("enter obj free\n");
    Object* objptr = (Object*) enif_priv_data(env);
}
*/

static int
open_resource(ErlNifEnv* env){
    printf("open_resource_obj\n");  
    //const char* mod0 = "Elixir.Res";
    const char* mod = "Elixir.Res.Object";
    const char* name = "Object";
    //const char* name2 = "Object";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    //arg...env,module_str,resource type name,destructor,flags,NULL
    rt_obj2 = enif_open_resource_type(env, mod, name, NULL, flags, NULL);
    //rt_obj = enif_open_resource_type(env,mod,name2,free_res_obj,flags,NULL);
    if(rt_obj2 == NULL) return -1;
    return 0;
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info){
    printf("loadobj\n");
    if(open_resource(env) == -1) return -1;

    return 0;
}

static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    printf("reloadobj\n");
    if(open_resource(env) == -1) return -1;
    return 0;
}
ERL_NIF_INIT(Elixir.Resinmulti.Object, nif_funcs_obj, &load, &reload, NULL, NULL);