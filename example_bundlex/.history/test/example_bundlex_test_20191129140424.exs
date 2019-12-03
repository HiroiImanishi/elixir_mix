defmodule ExampleBundlexTest do
  use ExUnit.Case
  doctest ExampleBundlex

  test "greets the world" do
    assert ExampleBundlex.hello() == :oops
  end
end
