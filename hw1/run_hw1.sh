rm *.csv

gcc multithread_int.c -lpthread -o multithread_int.out
gcc multithread_string.c -lpthread -o multithread_string.out
gcc multithread_struct.c -lpthread -o multithread_struct.out
gcc multiprocess_int.c -o multiprocess_int.out
gcc multiprocess_string.c -o multiprocess_string.out
gcc multiprocess_struct.c -o multiprocess_struct.out

echo "generate csv"
for ((i=1;i<100000000;i*=2))
do
  for ((j=1;j<13;j++))
  do
    ./multithread_int.out $i $j  > /dev/null
    ./multithread_string.out $i $j > /dev/null
    ./multithread_struct.out $i $j > /dev/null
    ./multiprocess_int.out $i $j > /dev/null
    ./multiprocess_string.out $i $j > /dev/null
    ./multiprocess_struct.out $i $j > /dev/null



    if (( $j == 1 ))
    then
      echo "$i"
    fi

  done
done

echo "plot"
python3 plot.py
