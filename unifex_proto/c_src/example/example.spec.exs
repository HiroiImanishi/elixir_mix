module Example.Native

callback :load

spec init() :: {:ok :: label, state}

spec foo(target :: pid, state) :: {:ok :: label, answer :: int} | {:error :: label, reason :: atom}

spec add(num1 :: int,num2 :: int) :: {:ok :: label,answer :: int}
spec greeting() :: {:ok ::label}
sends {:example_msg :: label, num :: int}