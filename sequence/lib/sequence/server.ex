defmodule Sequence.Server do
  use GenServer

  #startlinkの引数とか打つんめんどいし，pidわかっとく必要があったし，それを省くために外部APIを用意する
  #初期値だけユーザーが入力
  def start_link(current_number) do
    GenServer.start_link(__MODULE__,current_number, name: __MODULE__)
  end

  def next_number do
    GenServer.call(__MODULE__,:next_number)
  end

  def increment_number(delta) do
    GenServer.call(__MODULE__,{:increment,delta})
  end

  #GenServerによって呼ばれる．
  #1番目はクライアントが渡した最初の引数の情報
  #2番目はクライアントのPID
  #3番目はサーバーの状態
  def handle_call({:set_number,new_number},_from,current_number) do
    #返すタプルも{:atom,クライアントに返す値，新しい状態}
    {:reply, new_number, new_number}
  end

  def handle_call(:next_number,_from,current_number) do
    #返すタプルも{:atom,クライアントに返す値，新しい状態}
    {:reply, current_number, current_number + 1}
  end
  def handle_call({:increment,delta},_from,current_number) do
    IO.puts "hello!"
    {:reply, current_number, current_number+delta}
  end

  #キャストはクライアントに返す必要がないので，クライアントのpidを知らない
  def handle_cast({:increment_number,delta}, current_number) do
    {:noreply,current_number + delta}
  end
end