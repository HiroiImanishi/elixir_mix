defmodule Notification.Handler do
  #イベントハンドラの定義だよ〜
  @behaviour :gen_event
  require Logger

  #:gen_event.add_handler(Notification, Notification.Handler, name: "handler1")で入る
  def init(args) do
    name = get_in(args,[:name])
    Logger.info("#{name} initialized")
    {:ok,%{name: name}}
  end
  #:gen_event.call(Notification, Notification.Handler, "hello")で入る
  def handle_call(request,state) do
    Logger.info("#{state.name} called with #{request}")
    {:ok,{:ok,request},state}
  end
  #:gen_event.notify(Notification, "hello")で入る
  def handle_event(event,state) do
    Logger.info("#{state.name} received #{event}") # handler1 received hello
    {:ok,state}
  end

end
