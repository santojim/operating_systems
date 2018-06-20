# interprocess communication ( Shared Memory )

This is a project from a course in university to get into ipc undertanding. ``init`` reads given input file in binary form and stores it in shared memory. Creates and initializes semaphores, and two files (Reader_Output.txt & Writer_Output.txt) where the ``reader`` and ``writer`` outputs are writen accordingly. ``reader`` flags ``-r lower_bound upper_bound``  ``-d sleep_time`` ``-s records_memory`` ``-p semaphore_memory`` writes in Reader_Output.txt the time that process began reading from shared mem and which fields read. After that it sleeps for given time. ``writer`` changes the value of income with the process id. Flags are exactly the same as ``reader``, writes in Writer_Output.txt the time that process began writing and the above changed value. ``detacher `` takes two parameters ``-p semaphore_memory`` and ``-s records_memory``, and destroys shared memory and semaphores used by ``init``
## build
Folder contains Makefile by default builds all. Inside folder IPC there is a Makefile. Make sure you are in this folder. Then run:
```bash
make
```

## execute from bash script
You can edit the script by adding more readers/writers and also change values in flags (-r -d).
```bash
./exec.sh
```

## manual execution
First should execute ``init`` to create the record in shared mem.(example execution: ``./init input_files/b100``). After the records are in memory, execute either ``writer`` or ``reader`` (example execution: ``./reader -r 1 9 -d 5 -s 123456 -p 123456`` )(example execution: ``./writer -r 1 9 -d 5 -s 123456 -p 123456`` ). In order to remove the shared memory and semaphore you should execute ``detacher`` and pass the parameters from init (--->Use this part for copy and paste : -p xxxxxxx -s xxxxxxx)

## clean
Removes executables and output of reader and writer
```bash
make clean
```
