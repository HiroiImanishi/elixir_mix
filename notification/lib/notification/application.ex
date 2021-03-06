defmodule Notification.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    children = [
      %{
        id: :gen_event,
        start: {
          :gen_event,
          :start_link,
          [{:local, Notification}]
        }
      }
      # Starts a worker by calling: Notification.Worker.start_link(arg)
      # {Notification.Worker, arg}
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: Notification.Supervisor]
    Supervisor.start_link(children, opts)
  end
end
