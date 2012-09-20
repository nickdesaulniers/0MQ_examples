#include "zhelpers.h"

int main () {
  void* context = zmq_ctx_new();

  // Socket to send messages on
  void* sender = zmq_socket(context, ZMQ_PUSH);
  zmq_bind(sender, "tcp://*:5557");

  // Socket to send start of batch message on
  void* sink = zmq_socket(context, ZMQ_PUSH);
  zmq_connect(sink, "tcp://localhost:5558");

  printf("Press Enter when the workers are ready: ");
  getchar();
  printf("Sending tasks to workers...\n");

  // The first message is "0" and signals start of batch
  s_send(sink, "0");

  // Initialize the rng
  srandom((unsigned) time(NULL));

  // Send 100 tasks
  int task_nbr;
  // Total expected cost in msecs
  int total_msec = 0;
  for (task_nbr = 0; task_nbr < 100; task_nbr++) {
    int workload;
    // Random workload from 1 to 100msecs
    workload = randof(100) + 1;
    total_msec += workload;
    char string [10];
    sprintf(string, "%d", workload);
    s_send(sender, string);
  }
  printf("Total expected cost: %d msec\n", total_msec);
  // Give 0MQ time to deliver
  sleep(1);

  zmq_close(sink);
  zmq_close(sender);
  zmq_ctx_destroy(context);
}
