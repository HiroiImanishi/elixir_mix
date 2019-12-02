defmodule Client do
  
  def register do
    self()
    |> Ticker.register()
end