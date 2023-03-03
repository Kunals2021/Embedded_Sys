#include "queue.h"
#define MAX_COMMAND_LEN QUEUE_SIZE

typedef struct command {
  char * cmd_string;
  void (*cmd_function)(char * arg);
} command_t;

int get_command(uint8_t *);
int delspace(uint8_t *);
int execute_command(uint8_t *);
int parse_command (uint8_t *, uint8_t **, uint8_t **);
void prompt(void);
