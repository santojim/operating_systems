#!/bin/bash
if [ ! -f init ]; then
   echo "Run make first"
else
  init_output=$(yes | ./init input_files/b100 | awk '{for (I=1;I<=NF;I++) if ($I == "-p" || $I == "-s") {print $(I+1)};}')

  var_p=$(echo $init_output | cut -d' ' -f1)
  var_s=$(echo $init_output | cut -d' ' -f2)

  ./reader -r 1 9 -d 5 -p $var_p -s $var_s
  ./reader -r 15 39 -d 5 -p $var_p -s $var_s
  ./writer -r 10 100 -d 3 -p $var_p -s $var_s
  ./writer -r 1 20 -d 3 -p $var_p -s $var_s
  ./detacher -p $var_p -s $var_s
fi
