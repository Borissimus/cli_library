#include "serial_port.h"
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>

int serial_port_open(const char *port_name) {
	int fd = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);

	if (fd < 0) {
		perror("Error opening serial port");
	}

	return fd;
}

int serial_port_close(int fd) {
	int result = close(fd);

	if (result < 0) {
		perror("Error closing serial port");
	}

	return result;
}

void serial_port_configure(int fd, uint32_t baud_rate) {
	struct termios options;

	// Configure the serial port settings here (baud rate, parity, etc.)
	tcgetattr(fd, &options);
	cfsetispeed(&options, baud_rate); // Set input baud rate
	cfsetospeed(&options, baud_rate); // Set output baud rate
	options.c_cflag |= (CLOCAL | CREAD); // Enable receiver and set local mode
	options.c_cflag &= ~PARENB;	// Disable parity
	options.c_cflag &= ~CSTOPB;	// Set one stop bit
	options.c_cflag &= ~CSIZE; // Clear data size bits
	options.c_cflag |= CS8; // Set data size to 8 bits
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Set raw input mode
	options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control
	options.c_oflag &= ~OPOST; // Set raw output mode
	tcsetattr(fd, TCSANOW, &options);
}

_weak int serial_port_read(int fd, char *buffer, int buffer_size) {
	int bytes_read = read(fd, buffer, buffer_size - 1);

	if (bytes_read < 0) {
		perror("Error reading from serial port");
	}

	return bytes_read;
}

_weak int serial_port_write(int fd, const char *buffer, int buffer_size) {
	int bytes_written = write(fd, buffer, buffer_size);

	if (bytes_written < 0) {
		perror("Error writing to serial port");
	}

	return bytes_written;
}

int serial_port_listen(port_listen_t *port_ptr) {
	int            result        = SER_PORT_STATE_OK;
	int            select_result = 0;
	int            bytes_read    = 0;
	fd_set         read_fds;
    struct timeval timeout;

	if (port_ptr != NULL) {
		FD_ZERO(&read_fds);
		FD_SET(port_ptr->fd, &read_fds);

		// Set timeout to time_ms; this blocks for that duration or until data arrives
		timeout.tv_sec = port_ptr->time_ms / 1000;
		timeout.tv_usec = (port_ptr->time_ms % 1000) * 1000;

		select_result = select(port_ptr->fd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result > 0) {
			if (FD_ISSET(port_ptr->fd, &read_fds)) {
				// Data is available to read
				bytes_read = serial_port_read(port_ptr->fd, port_ptr->buffer, 
											  port_ptr->buffer_size);
				if (bytes_read > 0) {
                	port_ptr->buffer[bytes_read] = '\0'; // Null-terminate the string
					port_ptr->buffer_size = bytes_read;  // Update buffer size received
                	//printf("Received: %s\n\r", port_ptr->buffer);
				} else if (bytes_read < 0) {
					perror("Error reading from serial port");
				}
			}
			
		} else if (select_result == 0) {
			printf("No data received in %u ms\n\r", port_ptr->time_ms);
		} else {
			perror("Error with select");
			result = SER_PORT_STATE_ERROR;
		}
	} else {
		result = SER_PORT_STATE_ERROR;
		perror("Error! port_ptr is NULL");
	}

	return result;
}

int serial_port_echo(int fd, const char *buffer, int buffer_size) {
	int result = SER_PORT_STATE_OK;

	if (buffer != NULL) {
		// Echo back the received data
		int bytes_written = serial_port_write(fd, buffer, buffer_size);
		if (bytes_written != buffer_size) {
			result = SER_PORT_STATE_ERROR;
		}
	} else {
		result = SER_PORT_STATE_ERROR;
		perror("Error! port_ptr is NULL");
	}

	return result;
}