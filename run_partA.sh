#!/bin/bash

CSV_FILE="results.csv"
THREADS=(2 4 8)
CPU_CORES=$(sysctl -n hw.ncpu)

if [ ! -f "$CSV_FILE" ]; then
    echo "timestamp,experiment,cpu_cores,n_threads,final_counter,real_time,user_time,sys_time" > "$CSV_FILE"
fi

run_experiment() {
    local prog=$1
    local n=$2
    local timestamp=$(date +"%Y-%m-%d %H:%M:%S")
    
    echo "$prog with $n threads"
    
    tmp_time=$(mktemp)
    output=$( { time -p "./compiled/$prog" "$n"; } 2> "$tmp_time" )
    
    final_counter=$(echo "$output" | grep "Final counter:" | awk '{print $3}')
    
    real_time=$(grep "real" "$tmp_time" | awk '{print $2}')
    user_time=$(grep "user" "$tmp_time" | awk '{print $2}')
    sys_time=$(grep "sys" "$tmp_time" | awk '{print $2}')
    
    echo "$timestamp,$prog,$CPU_CORES,$n,$final_counter,$real_time,$user_time,$sys_time" >> "$CSV_FILE"
    rm "$tmp_time"
}

for n in "${THREADS[@]}"; do
    run_experiment "t1" "$n"
    run_experiment "t2" "$n"
done

echo "Results appended to: $CSV_FILE"