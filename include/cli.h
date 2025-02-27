#ifndef CLI_H
#define CLI_H

void cli_init(void);
void cli_execute_command(const char *command);
void cli_register_command(const char *command, void (*handler)(void));

void hello_command(void);

#endif // CLI_H
