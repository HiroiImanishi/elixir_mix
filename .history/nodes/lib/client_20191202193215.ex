defmodule Client do
  
  def start do
    pid = spawn(__MODULE__,:receiver,[])
    Ticker.register(pid)
  end
  #通知を受け取りつづけるプロセス
  def receiver do
    receive do
      {:tick} -> IO.puts "yahoo!"
    end
    receiver
  end
end