#include "cli.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    const char *command;
    void (*handler)(void);
} Command;

static Command command_list[10];
static int command_count = 0;

void cli_init(void) {
    cli_register_command("hello", hello_command);
}

void cli_register_command(const char *command, void (*handler)(void)) {
    if (command_count < 10) {
        command_list[command_count].command = command;
        command_list[command_count].handler = handler;
        command_count++;
    }
}

void cli_execute_command(const char *command) {
    for (int i = 0; i < command_count; i++) {
        if (strcmp(command_list[i].command, command) == 0) {
            command_list[i].handler();
            return;
        }
    }
    //log_message(LOG_LEVEL_WARN, "Command not found: %s", command);
}

void hello_command(void) {
    //log_message(LOG_LEVEL_INFO, "Hello command executed!");
}
