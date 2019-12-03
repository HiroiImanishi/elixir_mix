defmodule ExampleBundlex.BundlexProject do
  use Bundlex.Project
  @moduledoc """
  Documentation for ExampleBundlex.
  """

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
        # native files to be compiled
        # should be located in project_root/c_src/my_app
        sources: ["test.c"],
        # sources, libs, and all the stuff declared in dependencies will be added to this NIF
        deps: [app_name: :nif_name],
        # names of libraries that should be linked via pkg config
        pkg_configs: ["my_lib_with_pkg_config"],
        # names of libraries to link
        libs: ["my_library"],
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
