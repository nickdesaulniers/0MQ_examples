#include "../chapter1/zhelpers.h"

int s_interrupted = 0;

void s_signalhandler (int signal_value) {
  s_interrupted = 1;
}

void s_catch_signals () {
  struct sigaction action = {
    .sa_handler = s_signalhandler,
    .sa_flags = 0
  };
  sigemptyset(&action.sa_mask);
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);
}

int main () {
  void* context = zmq_ctx_new();
  void* socket = zmq_socket(context, ZMQ_REP);
  zmq_bind(socket, "tcp://*:5555");

  s_catch_signals();
  while(1) {
    // Blocking read will exit on a signal
    zmq_msg_t message;
    zmq_msg_init(&message);
    zmq_msg_recv(&message, socket, 0);

    if (s_interrupted) {
      printf("W: interrupt recived, killing server...\n");
      break;
    }
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}
