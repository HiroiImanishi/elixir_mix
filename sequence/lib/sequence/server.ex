defmodule Sequence.Server do
  @moduledoc """
    サーバを開始するときにサーバーに数値を渡して現在の状態とする．
    :next_numberというリクエストともに呼び出すと現在の状態を呼び出し側に返す
    同時に状態の数値を1増やして次の呼び出しに備える．
    サーバーを呼び出すたびに更新済みの連続した数を受け取ることができる
  """

  use GenServer

  #クライアントがサーバを呼び出すとGenServerはhandle_call関数を呼び出す．
  #引数について解説
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
  def handle_call(:spin,_from,current_number) do
    spin(current_number)
  end
  #キャストはクライアントに返す必要がないので，クライアントのpidを知らない
  def handle_cast({:increment_number,delta}, current_number) do
    {:noreply,current_number + delta}
  end
  #GenServerでrecursive loop
  def spin(number) do
      IO.puts "current_number:#{number}"
      number = number + 1
      :timer.sleep(1000)
      spin(number)
  end
  #GenServer.startlinkの引数とか打つんめんどいし，pidわかっとく必要があったし，それを省くために外部APIを用意する
  #初期値だけユーザーが入力
  #GenServer.start_link(module,init_arg,options)
  #GenServer.call(serverpid,atom for request)
  def start_link(current_number) do
    GenServer.start_link(__MODULE__,current_number, name: __MODULE__)
  end

  def next_number do
    GenServer.call(__MODULE__,:next_number)
  end

  def increment_number(delta) do
    GenServer.call(__MODULE__,{:increment,delta})
  end
  def start_spin do
    GenServer.call(__MODULE__,:spin)
  end
end
