#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "erl_nif.h"
#include "../include/general.h"
//#include "../include/simple_sum.h"


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

//返り値がbool型の関数,これが記述されていればプログラムは通るが，
//共有ライブラリと宣言だけでも通るようにしたい(11/15)

//intのポインタ型が引数になってるとき
int simple_sum(int* a){
    int sum = 0;
    sum = *a + 1;
    return sum;
}


ERL_NIF_TERM nif_simple_sum(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    int* a; 
    if(!enif_get_int(env, argv[0],a)){
	    return enif_make_badarg(env);
    }
   
    int result = simple_sum(a);
    return enif_make_int(env,result);
}

//返り値がbool型の関数
bool isInt(const Example* ex){
    int omosa = ex->id;
    if(sizeof(double) == sizeof(omosa)){
        return true;
    }
    else return false;
}


ERL_NIF_TERM nif_isInt(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    Example* res;
    if(argc != 1)
    {
        return enif_make_badarg(env);
    }

    if(!enif_get_resource(env, argv[0], RES_TYPE, (void**) &res))
    {
	    return enif_make_badarg(env);
    }
    printf("1\n");
    bool result;
    result = isInt(res);
    printf("2\n");
    printf("%d",result);
    if(result == true){
        return enif_make_tuple2(env,atom_ok,atom_true);
    }
    else return enif_make_tuple2(env,atom_ok,atom_false);
}


//引数が構造体
void change_age(Example* ex){
    ex->height = 185.0;
    ex->profile.age = 21;
}
ERL_NIF_TERM nif_change_age(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    Example* res;
    ERL_NIF_TERM ret;
    enif_get_resource(env,argv[0],RES_TYPE,(void**)&res);
    change_age(res);
    ret = enif_make_resource(env, res);
    enif_release_resource(res);
    //Agent.updateで値がタプルなどに変えられてしまうと，
    //もう一度getすることができなくなるのでERL_NIF_TERMで返す必要がある
    return ret;
    
}
//const char*にすることでwarningが出る．
//constはとりあえず最初はつけずにやってってええと思う
char* cre_str(){
    char* str = "hello,new world!";
    return str;
}
ERL_NIF_TERM nif_crestr(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc!=0){
        return enif_make_badarg(env);
    }
    //文字列に入力のc_func.cをもっかい見る

    char* res;
    /*
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(RES_TYPE, 30);
    if(res == NULL) return enif_make_badarg(env);
    */
    //ret = enif_make_resource(env, res);
    //enif_release_resource(res);
    res = cre_str();
    return enif_make_string(env,res,ERL_NIF_LATIN1);
    //return enif_make_tuple2(env,atom_ok,ret);
    //return enif_make_tuple2(env,str,ERL_NIF_LATIN1);
}
//引数がExample,汎用ポインタの関数
//リストを受け取ってvoid*に入れられるか
char* test(const Example* ex_struct,void* gen_ptr){
    char* str = (char*)gen_ptr; //汎用ポインタを使うときは任意の型にキャストする
    return str;  
    //return ex_struct->profile.name;
}

ERL_NIF_TERM nif_test(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    //引数が構造体ポインタの関数をここから実装
    Example* res;
    void* gen_ptr;    //何が来るかわかんねえ
    if(argc != 2)
    {
        return enif_make_badarg(env);
    }
    printf("1\n");
    if(!enif_get_resource(env, argv[0], RES_TYPE, (void**) &res)){
	    return enif_make_badarg(env);
    }
    printf("2\n");
    if(!enif_get_resource(env,argv[1],RES_TYPE, (void**) &gen_ptr)){
        return enif_make_badarg(env);
        /*
        ここでエラーするわけだが，argv[1]がenif_make_resourceした構造体だといけて，
        enif_make_stringした文字列だとエラーになる.(bad arg)
        argv[1]にきてるのは(11/11)文字列ではなく文字に変換できる整数のリストである
        */
    }
    
    printf("3\n");
    char* test_result;
    test_result = test(res,gen_ptr);
    return enif_make_string(env,test_result,ERL_NIF_LATIN1);
}

//charが引数の関数
const char* char_ex(const char* str){
    printf("enter char_ex\n");
    return str;
}


ERL_NIF_TERM nif_charex(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }

}
//引数にvoid*があってそこにリストを入れられるのか
void receive_list(void* arg){
    char* str = (char*) arg;

    printf("%s:success!!\n",str);
}

ERL_NIF_TERM nif_receive_list(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        enif_make_badarg(env);
    }
    int num = 0;
    void* gen_ptr;
    gen_ptr = &num;
    ERL_NIF_TERM a=0;
    printf("0\n");
    if(!enif_get_int(env,argv[0],(int*)gen_ptr)){
        return enif_make_badarg(env);
    }
    /*
    ERL_NIF_TERM head;
    ERL_NIF_TERM tail;
    if(!enif_get_list_cell(env,argv[0],&head,&tail)){
        return enif_make_badarg(env);
    } 
    */
    printf("1\n");
    receive_list(gen_ptr);
    
    //receive_list(gen_ptr);
    //return tail;    //'ello'が返る
    //return head;  //104が返る 
    return enif_make_int(env,1);
}
void
free_res(ErlNifEnv* env, void* obj)
{
    printf("enter free_res\n");
    Example* example = (Example*) enif_priv_data(env);
}
/*
void free_res_obj(ErlNifEnv* env, void* obj){
    printf("enter obj free\n");
    Object* objptr = (Object*) enif_priv_data(env);
}
*/
static int
open_resource(ErlNifEnv* env){
    printf("open_resource\n");  
    const char* mod = "Elixir.Res";
    const char* name = "Example";
    const char* name2 = "Object";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;
    Tracker* tracker;

    //arg...env,module_str,resource type name,destructor,flags,NULL
    RES_TYPE = enif_open_resource_type(env, mod, name, free_res, flags, NULL);
    rt_obj = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    if(RES_TYPE == NULL) return -1;
    return 0;
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info){
    printf("load\n");
    if(open_resource(env) == -1) return -1;

    atom_ok = enif_make_atom(env, "ok");
    atom_true = enif_make_atom(env,"true");
    atom_false = enif_make_atom(env,"false");
    Tracker* tracker;
    tracker = (Tracker*) enif_alloc(sizeof(Tracker));
    tracker->count = 0;
    *priv = (void*) tracker;

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

static ERL_NIF_TERM
count(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    printf("count\n");
    Tracker* tracker;

    if(argc != 0)
    {
        return enif_make_badarg(env);
    }

    tracker = (Tracker*) enif_priv_data(env);
    return enif_make_int(env, tracker->count);
}

static ERL_NIF_TERM create(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    printf("create\n");
    Example* res;
    ERL_NIF_TERM ret;
    unsigned int id;
    double height;
    Tracker* tracker;
    //createの引数がn個ならifの条件節の数値をnに合わせる
    if(argc != 2)
    {
        return enif_make_badarg(env);
    }

    if(!enif_get_uint(env, argv[0], &id))
    {
        return enif_make_badarg(env);
    }
    if(!enif_get_double(env, argv[1], &height))
    {
        return enif_make_badarg(env);
    }
    //RES_TYPEは
    res = enif_alloc_resource(RES_TYPE, sizeof(Example));
    if(res == NULL) return enif_make_badarg(env);

    ret = enif_make_resource(env, res);
    enif_release_resource(res);

    res->id = id;
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

    if(!enif_get_resource(env, argv[0], RES_TYPE, (void**) &res))
    {
	return enif_make_badarg(env);
    }

    return enif_make_tuple2(env, enif_make_int(env,res->id),enif_make_double(env,res->height));
   

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
//返り値が構造体のポインタ
Example* ret_strptr(){
    Example* ex;
    
    ex->height = 200.0;
    ex->id = -10;
    ex->profile.age = 200;
    ex->profile.name = "Jonny";
    return ex;
}
ERL_NIF_TERM nif_ret_strptr(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    Example* res_ex;
    ERL_NIF_TERM ret;
    res_ex = enif_alloc_resource(RES_TYPE,sizeof(Example));
    if(res_ex == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res_ex);
    enif_release_resource(res_ex);
    printf("uuu\n");
    res_ex = ret_strptr();

    return ret;
}

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

/*
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
    return enif_make_tuple2(env, enif_make_int(env,res->gram),enif_make_double(env,res->volume));

}
*/
static ErlNifFunc nif_funcs[] = {
    {"count", 0, count},
    {"create", 2, create},
    {"test",2,nif_test},
    {"hoge",0,hoge_nif},
    {"read_hoge",1,read_hogenif},
    {"isInt",1,nif_isInt},
    {"read", 1, read},
    {"cre_str",0,nif_crestr},
    {"receive_list",1,nif_receive_list},
    {"change_age",1,nif_change_age},
    {"greeting",1,nif_greeting},
    {"ret_strptr",0,nif_ret_strptr},
    {"simple_sum",1,nif_simple_sum},
};

ERL_NIF_INIT(Elixir.Resinmulti, nif_funcs, &load, &reload, NULL, NULL);

/*
    load()はiex **.exしたあとの@on_loadで呼び出された
    :erlang.load_nif('./resource',0)
    で呼び出される.
    load or upgradeはライブラリを初期化するために呼び出される．
*/