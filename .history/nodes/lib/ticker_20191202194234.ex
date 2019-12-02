defmodule Ticker do
  @interval 2000 #通知周期
  @name :ticker  #サーバプロセスの名前

  #サーバプロセスを生成するstart関数
  def start do
    pid = spawn(__MODULE__,:generator,[[]]) #生成時には空のリストを渡す
    :global.register_name(@name,pid)
  end

  def generator(clients) do
    receive do
      {:register,client_pid} ->
        IO.puts "register #{inspect client_pid}"
        #clientsリストに新しくpidを追加しgeneratorを続行
        generator([client_pid|clients])
    after 
      @interval ->
        IO.puts "tick"
        #登録しているクライアント全員に通知を送信
        Enum.each(clients,fn(client) -> send(client,{:tick,counter}) end)
        generator(clients)
    end
  end

  def register(client_pid) do
    send(:global.whereis_name(@name),{:register,client_pid})
  end
end