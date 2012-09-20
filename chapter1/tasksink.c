#include "zhelpers.h"

int main () {
  // Prepare our context and socket
  void* context = zmq_ctx_new();
  void* receiver = zmq_socket(context, ZMQ_PULL);
  zmq_bind(receiver, "tcp://*:5558");

  // Wait for the start of the batch
  char* string = s_recv(receiver);
  free(string);

  // Start our clock now
  int64_t start_time = s_clock();

  // Process 100 confirmations
  int task_nbr;
  for (task_nbr = 0; task_nbr < 100; task_nbr++) {
    char* string = s_recv(receiver);
    free(string);
    if ((task_nbr / 10) * 10 == task_nbr) {
      printf(":");
    } else {
      printf(".");
    }
    fflush(stdout);
  }
  // Calculate and report duration of batch
  printf("\nTotal elapsed time: %d msec\n", (int) (s_clock() - start_time));
  zmq_close(receiver);
  zmq_ctx_destroy(context);
}
