module Example.Native

callback :load

spec init() :: {:ok :: label, state}

spec foo(target :: pid, state) :: {:ok :: label, answer :: int} | {:error :: label, reason :: atom}

sends {:example_msg :: label, num :: int}