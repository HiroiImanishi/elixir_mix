#include "../include/example.h"
#include "erl_nif.h"

//外部で宣言してる関数が入ってる場合
Example hoge(){
    printf("hoge function\n");
    
    static Example ex ={5,170.5,{"Jack",35}};
    return ex;
}

//hoge()をラップしたやつ
static ERL_NIF_TERM hoge_nif(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    Example* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(RES_TYPE,sizeof(Example));
    if(res == NULL) return enif_make_badarg(env);
    *res = hoge();      //ここでもオッケー！
    ret = enif_make_resource(env,res);
   
    enif_release_resource(res);
    
    return ret;
}

//hoge()で入れた構造体の中身を調べるElixir関数
static ERL_NIF_TERM
read_hogenif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    printf("read_hogenif\n");
    Example* res;
    
    if(argc != 1)
    {
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[0], RES_TYPE, (void**) &res))
    {
	return enif_make_badarg(env);
    }
    return enif_make_tuple4(env, enif_make_int(env,res->id),enif_make_double(env,res->height),
    enif_make_string(env,res->profile.name,ERL_NIF_LATIN1),enif_make_int(env,res->profile.age));

}

void
free_res(ErlNifEnv* env, void* obj){
    printf("enter free_res\n");
    Example* example = (Example*) enif_priv_data(env);
}
static int
open_resource(ErlNifEnv* env){
    printf("open_resource_hoge\n");  
    const char* mod2 = "Elixir.Res.hoge";
    const char* name = "Example";
    const char* name2 = "Object";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    //arg...env,module_str,resource type name,destructor,flags,NULL
    RES_TYPE = enif_open_resource_type(env, mod, name, free_res, flags, NULL);
    rt_obj = enif_open_resource_type(env,mod,name2,free_res_obj,flags,NULL);
    if(RES_TYPE == NULL || rt_obj == NULL) return -1;
    return 0;
}
static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    printf("load_hoge\n");
    if(open_resource(env) == -1) return -1;
    return 0;
}

static ErlNifFunc nif_funcs_hoge[] = {
  {"hoge",0,hoge_nif},
  {"read_hoge",1,read_hogenif}
};

ERL_NIF_INIT(Elixir.Res.Hoge,nif_funcs_hoge,&load,&reload,NULL,NULL);