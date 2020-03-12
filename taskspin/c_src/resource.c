#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "erl_nif.h"
//#include "../include/simple_sum.h"

#define MY_NUMBER 10
ErlNifResourceType* rt_ex;
ErlNifResourceType* rt_obj;
ERL_NIF_TERM atom_ok;
ERL_NIF_TERM atom_true;
ERL_NIF_TERM atom_false;

typedef struct
{
  int count;
} Tracker;

typedef struct {
    int data;
    double volume;
} Object;

typedef struct {
    const char* name;
    int age;
} Inner;
//ラップしたい，入れ子になってる構造体
typedef struct
{
  int data;
  double height;
  Inner profile;
} Example;


//構造体が返り値のラップしたい関数
/*
Example hoge(){
    printf("hoge function\n");
    
        static Example init_pub ={1,170.5,{"Jack",22}};
        return init_pub;
    
    static Unchi katame = {160,20.0};
    static Inner hoy = {"Jack",18};
    Example katamehoy = arg_return_diff(&katame,&hoy);
    return katamehoy;
}
*/

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
    res = enif_alloc_resource(rt_ex,sizeof(Example));
    if(res == NULL) return enif_make_badarg(env);
    *res = hoge();      //ここでもオッケー！
    ret = enif_make_resource(env,res);
   
    enif_release_resource(res);
    
    return ret;
}

//返り値がbool型の関数,引数がconst構造体
bool isInt(const Example* ex){
    int omosa = ex->data;
    printf("data:%d\n",ex->data);
    printf("height:%f\n",ex->height);
    printf("age:%d\n",ex->profile.age);
    printf("name:%s\n",ex->profile.name);
    if(sizeof(double) == sizeof(omosa)){
        return true;
    }
    else return false;
}

ERL_NIF_TERM nif_isInt(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    Example** res;

    if(!enif_get_resource(env, argv[0], rt_ex, (void**) &res))
    {
	    return enif_make_badarg(env);
    }
    printf("1\n");
    bool result;
    result = isInt(*res);
    printf("2\n");
    printf("%d",result);
    if(result == true){
        return enif_make_tuple2(env,atom_ok,atom_true);
    }
    else return enif_make_tuple2(env,atom_ok,atom_false);
}


//引数，返り値ともに構造体
int copy_data(Object* obj,Example* ex){
    obj->data = ex->data;
    obj->volume = ex->height;
    ex->data++;
    return 0;
}

ERL_NIF_TERM nif_copy_data(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    
    Object* res_obj;
    Example* res_ex;
    ERL_NIF_TERM ret_obj,ret_ex;
    int ret = 0;
    enif_get_resource(env,argv[0],rt_obj,(void**)&res_obj);
    enif_get_resource(env,argv[1],rt_ex,(void**)&res_ex);
    printf("hey\n");
    ret = copy_data(res_obj,res_ex);       
    printf("fuga\n");
    ret_obj = enif_make_resource(env,res_obj);
    ret_ex = enif_make_resource(env,res_ex);
    //こいつらがいらんかった
    //enif_release_resource(res_obj);
    //enif_release_resource(res_ex);
    
    printf("nyoo\n");
    return enif_make_tuple3(env,enif_make_int(env,ret),ret_obj,ret_ex);
}

ERL_NIF_TERM nif_test(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    //引数が構造体ポインタの関数をここから実装
    Example* res;
    void* gen_ptr;    //何が来るかわかんねえ
    if(argc != 1)
    {
        printf("hey\n");
        return enif_make_badarg(env);
    }
    printf("1\n");
    //受け取るのはExampleもしくはObjectなのでリソース型を決めつけられない
    //もしくはここに来たstructureによって場合分け
    if(!enif_get_resource(env,argv[0],rt_ex, (void**) &res)){
        printf("2\n");
        return enif_make_badarg(env);
        /*
        ここでエラーするわけだが，argv[1]がenif_make_resourceした構造体だといけて，
        enif_make_stringした文字列だとエラーになる.(bad arg)
        argv[1]にきてるのは(11/11)文字列ではなく文字に変換できる整数のリストである
        */
    }
    
    printf("3\n");
    return enif_make_atom(env,"ok");
}

void free_ex(ErlNifEnv* env, void* resource){
    printf("enter example free\n");
    Example* ex = (Example*) resource;
    enif_free(ex);
    printf("finish example free\n");

}
void free_obj(ErlNifEnv* env, void* resource){
    printf("enter obj free\n");
    Object* obj = (Object*) resource;
    enif_free(obj);
    printf("finish obj free\n");

}

static int
open_resource(ErlNifEnv* env)
{
    printf("open_resource\n");  
    const char* mod = "Elixir.Taskspin";
    const char* name = "Example";
    const char* name2 = "Object";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    //arg...env,module_str,resource type name,destructor,flags,NULL
    rt_ex = enif_open_resource_type(env, mod, name, free_ex, flags, NULL);
    rt_obj = enif_open_resource_type(env,mod,name2,free_obj,flags,NULL);
    if(rt_ex == NULL || rt_obj == NULL) return -1;
    return 0;
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    printf("load\n");
    if(open_resource(env) == -1) return -1;

    atom_ok = enif_make_atom(env, "ok");
    atom_true = enif_make_atom(env,"true");
    atom_false = enif_make_atom(env,"false");

    return 0;
}

// Erlang requires that we re-open resources on re-initialisation.
static int
reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    printf("reload\n");
    if(open_resource(env) == -1) return -1;
    return 0;
}
/*
static int
reload(ErlNifEnv* env, void** priv, void** old_priv, ERL_NIF_TERM load_info)
{
    if(open_resource(env) == -1) return -1;
    return 0;
}
*/

static ERL_NIF_TERM create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    printf("create\n");
    Example* res;
    ERL_NIF_TERM ret;
    unsigned int data;
    double height;
    Tracker* tracker;
    //createの引数がn個ならifの条件節の数値をnに合わせる
    if(argc != 2)
    {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint(env, argv[0], &data))
    {
        return enif_make_badarg(env);
    }
    if(!enif_get_double(env, argv[1], &height))
    {
        return enif_make_badarg(env);
    }
    //rt_exは
    res = enif_alloc_resource(rt_ex, sizeof(Example));
    if(res == NULL) return enif_make_badarg(env);

    ret = enif_make_resource(env, res);
    enif_release_resource(res);

    res->data = data;
    res->height = height;
    tracker = (Tracker*) enif_priv_data(env);
    tracker->count += 1;

    return enif_make_tuple2(env, atom_ok, ret);
}

static ERL_NIF_TERM
read(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    printf("read\n");
    Example* res;

    if(argc != 1)
    {
        return enif_make_badarg(env);
    }

    if(!enif_get_resource(env, argv[0], rt_ex, (void**) &res))
    {
	return enif_make_badarg(env);
    }

    return enif_make_tuple2(env, enif_make_int(env,res->data),enif_make_double(env,res->height));
   

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
    if(!enif_get_resource(env, argv[0], rt_ex, (void**) &res))
    {
	return enif_make_badarg(env);
    }
    return enif_make_tuple4(env, enif_make_int(env,res->data),enif_make_double(env,res->height),
    enif_make_string(env,res->profile.name,ERL_NIF_LATIN1),enif_make_int(env,res->profile.age));

}
void greeting(char* str){
    printf("%s,%d\n",str,MY_NUMBER);
    
}
ERL_NIF_TERM nif_greeting(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
   
    char buf[64];
    (void)memset(&buf,'\0',sizeof(buf));
    if(!enif_get_string(env,argv[0],buf,sizeof(buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
    greeting(buf);  //なんかブレイクしたら表示された...?
    return enif_make_atom(env,"ok");
    //return enif_make_string(env,"hello,world",ERL_NIF_LATIN1);
}

//空のリソースオブジェクトを作ってあとで使う場合
ERL_NIF_TERM nif_cre_empty(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    Object* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_obj,sizeof(Object));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    return ret;
}
//からのオブジェクトに値を入れる
void fillin_obj(Object* obj){
    obj->data = 100;
    obj->volume = 123.4;
}
//fillinobjのNIF関数
ERL_NIF_TERM nif_fillin_obj(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    Object* res;
    ERL_NIF_TERM ret;
    if(!enif_get_resource(env, argv[0], rt_obj, (void**) &res)){
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
    if(!enif_get_resource(env, argv[0], rt_obj, (void**) &res))
    {
	return enif_make_badarg(env);
    }
    return enif_make_tuple2(env, enif_make_int(env,res->data),enif_make_double(env,res->volume));

}
static ErlNifFunc nif_funcs[] = {
    {"create", 2, create},
    {"hoge",0,hoge_nif},
    {"read_hoge",1,read_hogenif},
    {"read", 1, read},
    {"test",1,nif_test},
    {"isInt",1,nif_isInt},
    {"copy_data",2,nif_copy_data},
    {"greeting",1,nif_greeting},
    {"cre_empty",0,nif_cre_empty},
    {"fillin_obj",1,nif_fillin_obj},
    {"read_obj",1,nif_read_obj},
};

ERL_NIF_INIT(Elixir.Taskspin, nif_funcs, &load, &reload, NULL, NULL);
/*
    load()はiex **.exしたあとの@on_loadで呼び出された
    :erlang.load_nif('./resource',0)
    で呼び出される.
    load or upgradeはライブラリを初期化するために呼び出される．
*/