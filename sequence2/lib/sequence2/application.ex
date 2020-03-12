defmodule Sequence2.Application do
  # See https://hexdocs.pm/elixir/Application.html
  # for more information on OTP Applications
  @moduledoc false

  use Application

  def start(_type, _args) do
    children = [
      # Starts a worker by calling: Sequence2.Worker.start_link(arg)
      # {Sequence2.Worker, arg}
      worker(Sequence2.Server, [123])
    ]

    # See https://hexdocs.pm/elixir/Supervisor.html
    # for other strategies and supported options
    opts = [strategy: :one_for_one, name: Sequence2.Supervisor]
    {:ok, _pid} = Supervisor.start_link(children, opts)
  end
end
