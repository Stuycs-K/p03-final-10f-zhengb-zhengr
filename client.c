#include "networking.h"


struct message {
  char msg[1025];
  char username[33];
  char time[9];
};

struct message messages[100];
