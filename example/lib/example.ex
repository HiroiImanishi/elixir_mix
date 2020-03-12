defmodule Res do
  @on_load :load_nifs
  def load_nifs do
      IO.puts "load_nifs"
      :erlang.load_nif('/home/imanishi/Desktop/rakugaki/elixir_mix/example/c_src/lib/resource',0)
  end
  use Agent
  def count do
      raise "NIF count/0 not implemented"
  end
  def read(_a) do
      raise "NIF read/1 not implemented"
  end
  def create(_a,_b) do
      raise "NIF create/2 not implemented"
  end

  defmodule Object do
    @on_load :load_nifs
    def load_nifs do
      IO.puts "load_nif_obj"
      :erlang.load_nif('/home/imanishi/Desktop/rakugaki/elixir_mix/example/c_src/lib/object',0)
    end
      def cre_empty do
      raise "sorry"
    end
    def fillin_obj(_a) do
        raise "sorry"
    end
    def read_obj(_a) do
        raise "sorry"
    end
  end

  def test(_a,_b) do
      raise "NIF test/1 not implemented"
  end
  def isInt(_a) do
      raise "NIF isInt/1 not implemented"
  end
  def cre_str do
      raise "NIF crestr/0 not implemented"
  end
  def receive_list(_a) do
      raise "NIF receivelist/1 not implemented"
  end
  def change_age(_a) do
      raise "NIF change_age/1 not implemented"
  end
  def greeting(_a) do
      raise "Sorry"
  end
  def ret_strptr do
      raise "Sorry"
  end
  def delegate_agent(profile) do
     Agent.start_link(fn -> profile end,name: :agent1)
  end

  def agent_value(agent) do
      IO.puts "entered"
      #Agent.get(agent,fn(n) -> n end)
      #Agent.get(agent,Res.Hoge.read_hoge(agent))
  end

  def agent_update(agent) do
      Agent.update(agent,fn -> Res.change_age(agent) end)
  end

  def simple_sum(_a) do
      raise "NIF simple_sum/1 is not implemented"
  end
  
  #------------空のリソースオブジェクトを作って値を入れてちゃんと変わってるか調べる-------------
  
end