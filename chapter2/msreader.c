// Funky way of doing non blocking reads
#include "../chapter1/zhelpers.h"

int main () {
  // Prepare our context and sockets
  void* context = zmq_ctx_new();

  // Connect to task ventilator
  void* receiver = zmq_socket(context, ZMQ_PULL);
  zmq_connect(receiver, "tcp://localhost:5557");

  // Connect to weather server
  void* subscriber = zmq_socket(context, ZMQ_SUB);
  zmq_connect(subscriber, "tcp://localhost:5556");
  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "10001", 6);

  // Process messages from both sockets
  // We prioritize message from the task ventilator
  while (1) {
    // Process any waiting tasks
    int rc;
    for (rc = 0; !rc;) {
      zmq_msg_t task;
      zmq_msg_init(&task);
      if ((rc = zmq_msg_recv(&task, receiver, ZMQ_DONTWAIT)) != -1) {
        // Process task
      }
      zmq_msg_close(&task);
    }
    // Process any waiting weather updates
    for (rc = 0; !rc;) {
      zmq_msg_t update;
      zmq_msg_init(&update);
      if ((rc = zmq_msg_recv(&update, subscriber, ZMQ_DONTWAIT)) != -1) {
        // Process weather update
      }
      zmq_msg_close(&update);
    }
    // No activity, so sleep for 1ms
    s_sleep(1);
  }
  // We never get here but clean up anyhow
  zmq_close(receiver);
  zmq_close(subscriber);
  zmq_ctx_destroy(context);
}
