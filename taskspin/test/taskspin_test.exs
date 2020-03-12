defmodule TaskspinTest do
  use ExUnit.Case
  doctest Taskspin

  test "greets the world" do
    assert Taskspin.hello() == :world
  end
end
