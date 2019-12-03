defmodule ExampleBundlex do
  @moduledoc """
  Documentation for ExampleBundlex.
  """

  @doc """
  Hello world.

  ## Examples

      iex> ExampleBundlex.hello()
      :world

  """
  def hello do
    :world
  end

  def project() do
    [
      nifs: nifs(Bundlex.platform),
      cnodes: cnodes(),
      libs: libs()
    ]
  end

  defp nifs(:linux) do
    [
      my_nif: [
        sources: ["something.c", "linux_specific.c"]
      ],
      my_other_nif: [
        "..."
      ]
    ]
  end

  defp nifs(_platform) do
    [
      my_nif: [
        sources: ["something.c","multiplatform.c"]
      ],
      my_other_nif: [
        #...
      ]
    ]
  end

  defp cnodes() do
    [
      my_cnode: [
        sources: ["something.c","something_other.c"]
      ],
      my_other_cnode: [
        #...
      ]
    ]
  end
  defp libs(_platform) do
    [
      my_lib: [
        sources: ["something.c", "something_other.c"]
      ],
      my_other_lib: [
        # ...
      ]
    ]
  end
  end
end
