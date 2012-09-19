#include "zhelpers.h"

int main (int argc, char* argv[]) {
  void* context = zmq_ctx_new();

  // Socket to talk to server
  puts("Collecting updates from weather server...");
  void* subscriber = zmq_socket(context, ZMQ_SUB);
  int rc = zmq_connect(subscriber, "tcp://localhost:5556");
  assert(rc == 0);

  // Subscribe to zipcode, default is NYC, 10001
  char* filter = (argc > 1) ? argv[1] : "10001 ";
  rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
  assert(rc == 0);

  // Process 100 updates
  long total_temp = 0;
  int update_nbr;
  for (update_nbr = 0; update_nbr < 100; update_nbr++) {
    char* string = s_recv(subscriber);

    int zipcode, temperature, relhumidity;
    sscanf(string, "%d %d %d", &zipcode, &temperature, &relhumidity);
    total_temp += temperature;
    free(string);
  }
  printf("Average temperature for zipcode '%s' was %dF\n", filter, 
    (int) (total_temp / update_nbr));

  zmq_close(subscriber);
  zmq_ctx_destroy(context);
}
