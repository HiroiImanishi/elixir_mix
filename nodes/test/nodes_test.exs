defmodule NodesTest do
  use ExUnit.Case
  doctest Nodes

  test "greets the world" do
    assert Nodes.hello() == :world
  end
end
