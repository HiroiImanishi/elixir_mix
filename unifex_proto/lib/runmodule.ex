defmodule Resource do
  @on_load :load_nifs
  def load_nifs do
    IO.puts "load_nifs"
    :erlang.load_nif('/home/imanishi/mix_proj/unifex_proto/c_lib/resource',0)
  end

  def count do
      raise "NIF count/0 not implemented"
  end
  def read(_a) do
      raise "NIF read/1 not implemented"
  end
  def create(_a) do
      raise "NIF create/1 not implemented"
  end
end