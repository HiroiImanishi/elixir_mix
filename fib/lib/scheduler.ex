defmodule Scheduler do
    @doc """
        runはExecuteモジュールから呼び出される．
        num_processes数のプロセスを生成、
        for example: num_process = 3,module=Fib,func=fib,to_calculate = [31,32,33,34,35...]
    """
    def run(num_processes,module,func,to_calculate) do
        (1..num_processes)
        |> Enum.map(fn(_) -> spawn(module,func,[self()]) end)   
        #Fibモジュールのfib関数を行うプロセス(計算部隊)を複数生成
        |> schedule_processes(to_calculate, [])                 
        #返された計算部隊のpidリストと計算すべきリストを元にスケジューリングを呼び出す
    end
    #計算部隊（サーバプロセス）のpidリスト,計算リスト,結果を格納するリスト
    defp schedule_processes(processes,queue,results) do
        receive do
            #まだ計算するフィボナッチすうが残っている(when節)
            {:ready, pid} when length(queue) > 0 ->
                [next|tail] = queue #nextとtailに分割
                # であればサーバプロセスに計算要求
                send(pid,{:fib,next,self()})
                schedule_processes(processes, tail, results)
            {:ready,pid} -> 
                send(pid,{:shutdown})
                #計算すべきフィボナッチ数はないが、サーバプロセスが残っている場合
                if length(processes) > 1 do
                    #IO.puts "shutdown and rests :#{inspect Enum.at(processes,0)},#{inspect Enum.at(processes,1)}"
                    schedule_processes(List.delete(processes,pid),queue,results)
                    #シャットダウンするよう指令を送って、Listから削除する
                else
                    #全てサーバプロセスを削除したら、結果をソートする
                    Enum.sort(results,fn({n1,_},{n2,_}) -> n1 <= n2 end)
                end
                #サーバプロセスが計算し終わった値を結果リストに格納する
            {:answer,number,result,_pid} -> 
                schedule_processes(processes,queue,[{number,result}|results])
        end
    end
end