// compiled with:
// clang -Wall hwserver.c -o hwserver -lzmq
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (void) {
  void* context = zmq_ctx_new();
  
  // socket to talk to clients
  void* responder = zmq_socket(context, ZMQ_REP);
  zmq_bind(responder, "tcp://*:5555");
  
  while (1) {
    // wait for next request from client
    zmq_msg_t request;
    zmq_msg_init(&request);
    zmq_msg_recv(&request, responder, 0);
    printf("Received Hello\n");
    zmq_msg_close(&request);
    
    // do some work
    sleep(1);
    
    // send reply back to client
    zmq_msg_t reply;
    zmq_msg_init_size(&reply, 5);
    memcpy(zmq_msg_data(&reply), "World", 5);
    zmq_msg_send(&reply, responder, 0);
    zmq_msg_close(&reply);
  }
  
  // We never get here, but if we did, this would be how we end
  zmq_close(responder);
  zmq_term(context);
  return 0;
}
