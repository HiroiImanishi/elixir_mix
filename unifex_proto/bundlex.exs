defmodule Example.BundlexProject do
  use Bundlex.Project

  def project() do
    [
      nifs: nifs(Bundlex.platform())
    ]
  end

  def nifs(_platform) do
    [
      example: [
        deps: [unifex: :unifex],
        src_base: "example",
        sources: ["_generated/example.c","example.c"],
        lib_dirs: [__DIR__, "c_src/example/c_libs"],
        libs:    ["test"]
      ]
    ]
  end
end