This Repo Aims at implementing the flip hash algorithm in c.
-------------------------------------------------------------------------------------------------------
Process:

1) It takes the number of resources as input from the user.
2) Begins a parallel processing of taking inputs and executing them.
  2.1) The inputs are taken in packets and stored into a queue.
  2.2) At the same time, if the queue is not empty, the execution part deques packets and hashes them.
  2.3) The outputs of the hashes are being printed to stdout, at the moment.
-------------------------------------------------------------------------------------------------------
Usage:
1) Make the executable file (using make).
2) Enter the total number of resources.
3) Give your inputs and the program does it's job.
-------------------------------------------------------------------------------------------------------
