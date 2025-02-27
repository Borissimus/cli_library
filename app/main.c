#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/select.h>
#include "main.h"
#include "cli.h"
#include "log.h"
#include "serial_port.h"

#define SERIAL_PORT_BUF_SIZE 8
static char serial_port_buf[SERIAL_PORT_BUF_SIZE];

static console_api_t logger;

int main(int argc, char* argv[]) {
	// Initialize serial port
	serial_port_t port = {
		.name = (argv[1] != NULL) ? argv[1] : NULL,
		.state = SER_PORT_STATE_OK,
		.exit_requested = false,
		.listen = {
			.fd = 0,
			.buffer = serial_port_buf,
			.buffer_size = SERIAL_PORT_BUF_SIZE,
			.time_ms = 10000
		}
	};

	// Check serial port name and open it
	if ((argc > 1) && port.name != NULL) {
		port.listen.fd = serial_port_open(port.name);
		printf("Serial port name: %s\n\r", port.name);
		if (port.listen.fd < 0) {
			perror("Error opening serial port");
			port.state = SER_PORT_STATE_ERROR;
		}
	} else {
		port.state = SER_PORT_STATE_ERROR;
	}

	// Configure the serial port settings
	serial_port_configure(port.listen.fd, B115200);

	// Initialize the logger
	init_console_instance(&logger);

	logger.error("Error message\n\r");

    while (port.exit_requested == false && port.state == SER_PORT_STATE_OK) {
		// Rewrite serial port buffer size
		port.listen.buffer_size = SERIAL_PORT_BUF_SIZE;
		// Listen for data on the serial port
		port.state = serial_port_listen(&port.listen);

		if(port.state == SER_PORT_STATE_OK) {
			// Check exit condition
			if (port.listen.buffer[0] == 13) {
				printf("Exit\n\r");
				port.exit_requested = true;
			} else {
				// Echo back the received data
				port.state = serial_port_echo(port.listen.fd,
											  port.listen.buffer,
											  port.listen.buffer_size);
			}
		}
	}
	// Close the serial port
    return serial_port_close(port.listen.fd);
}

