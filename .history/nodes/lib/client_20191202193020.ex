defmodule Client do
  
  def start do
    pid = spawn(__MODULE__,:receiver,[])
    Ticker.register(pid)
end