defmodule Execute do
    def run do
        to_process = [37,37,37,37,37,37]
        #プロセス数1~10で処理速度の実験
        Enum.each(1..10,fn(num_processes)->
            {time,result} = :timer.tc(
            Scheduler, :run, [num_processes,Fib, :fib, to_process]
        )

        if num_processes == 1 do
            IO.puts "hello! #{inspect result}"  #計算結果を表示する
            IO.puts "\n # time (ms)"               
        end
        :io.format "~2B   ~.2f~n",[num_processes, time/1000.0]
        end)
    end
end