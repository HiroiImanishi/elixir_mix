defmodule Taskspin do
  @on_load :load_nifs
  def load_nifs do
      IO.puts "load_nifs"
      :erlang.load_nif('/home/imanishi/Desktop/rakugaki/elixir_mix/taskspin/c_src/lib/resource',0)
  end
  require IEx
  def count do
    
  end
  def create(_a,_b) do
    "Sorry create/2"
  end
  def hoge do
    "Sorry hoge/0"
  end
  def read_hoge(_a) do
    "Sorry read_hoge/1"
  end
  def read(_a) do
    "Sorry read/1"
  end
  def test(_a) do
    "Sorry test/1"
  end
  def isInt(_a) do
    "Sorry isInt/1"
  end
  def greeting(_a) do
    "Sorry greeting/1"
  end
  def cre_empty do
    "Sorry cre_empty/0"
  end
  def fillin_obj(_a) do
    "Sorry fillin_obj/1"
  end
  def read_obj(_a) do
    "Sorry read_obj/1"
  end
  def copy_data(_a,_b) do
    "NIF copy_data/1 is not implemented"
  end

  def spin(obj,example) do
    IO.puts "start spin"
    case copy_data(obj,example) do
      {0,_,_} -> IO.puts "ok"
      {1,_,_} -> IO.puts "error"
      _ -> IO.puts "others"
    end
    :timer.sleep(500)
    spin(obj,example)
  end
  #2,3回spinを回すとセグフォ
  def sub_start(obj,example) do
    worker = Task.async(fn -> spin(obj,example) end)
    {:ok,worker}
  end
  def current_value(worker) do
    result = Task.await(worker)
    IO.inspect(result)
  end

  #返り値を最後に書けばおｋ
  def returntest do
    a = 1
    b = 2
    {:ok,a,b}
  end
end
