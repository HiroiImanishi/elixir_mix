defmodule Notification do
  @moduledoc """
  Documentation for Notification.
  """

  @doc """
  Hello world.

  ## Examples

      iex> Notification.hello()
      :world

  """
  def add_handler(sub_list) do
    :gen_event.add_handler(__MODULE__, Notification.Handler, name: name)
  end
  def notify(event) do
    :gen_event.notify(__MODULE__,event)
  end
end
