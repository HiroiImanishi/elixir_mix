defmodule Sequence.Server do
  use GenServer

  #GenServerによって呼ばれる．
  #1番目はクライアントが渡した最初の引数の情報
  #2番目はクライアントのPID
  #3番目はサーバーの状態
  def handle_call({:set_number,new_number},_from,current_number) do
    #返すタプルも{:atom,クライアントに返す値，新しい状態}
    {:reply, new_number, new_number}
  end

  def handle_call(:hello,_from,state) do
    IO.puts "hello!"
    {:reply, state, state+10}
  end

  #キャストはクライアントに返す必要がないので，クライアントのpidを知らない
  def handle_cast({:increment_number,delta}, current_number) do
    {:noreply,current_number + delta}
  end
end