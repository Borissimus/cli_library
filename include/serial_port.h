#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <stdint.h>
#include <stdbool.h>

#define _weak __attribute__((weak))

typedef enum serial_port_state_t{
	SER_PORT_STATE_OK = 0,
	SER_PORT_STATE_BUSY	= 1,
	SER_PORT_STATE_ERROR = -1
} port_state_t;

typedef struct serial_port_listen_t {
	int          fd;
	char*        buffer;
	int          buffer_size;
	uint16_t     time_ms;
} port_listen_t;

typedef struct serial_port_t {
	char*         name;	
	port_state_t  state;
	bool		  exit_requested;
	port_listen_t listen;
} serial_port_t;

/**
 * @brief Opens a serial port.
 *
 * @param port_name The name of the serial port to open.
 * @return The file descriptor of the opened serial port, or -1 if an error occurred.
 */
int serial_port_open(const char *port_name);

/** @brief Closes the serial port connection.
 *
 * This function closes the serial port connection associated with the given file descriptor.
 *
 * @return Returns 0 on success, or -1 on failure.
 * @param fd The file descriptor of the serial port.
 * @param baud_rate The baud rate to be set.
 */
void serial_port_configure(int fd, uint32_t baud_rate);

/** @brief Closes the serial port.
**
 *
 * This function closes the serial port associated with the given file descriptor.
 *
 * @param fd The file descriptor of the serial port to be closed.
 * @return Returns 0 on success, or -1 on failure.
 *
 * @brief 
 * 
 */
int serial_port_close(int fd);

/**
 * @brief Reads data from the serial port into the specified buffer.
 *
 * This function reads data from the serial port into the specified buffer.
 *
 * @param fd The file descriptor of the serial port.
 * @param buffer The buffer to store the read data.
 * @param buffer_size The size of the buffer.
 * @return The number of bytes read, or -1 if an error occurred.
 */
int serial_port_read(int fd, char *buffer, int buffer_size);

/**
 * @brief 
 * 
 * This function writes data from the specified buffer to the serial port.
 *
 * @param fd The file descriptor of the serial port.
 * @param buffer The buffer containing the data to be written.
 * @param buffer_size The size of the buffer.
 * @return The number of bytes written, or -1 if an error occurred.
 */
int serial_port_write(int fd, const char *buffer, int buffer_size);

/**
 * @brief 
 * 
 * This function listens for data on the serial port.
 *
 * @param port_listen The serial port listen configuration.
 * @return serial port state.
 */
int serial_port_listen(port_listen_t *port_listen);

/**
 * @brief 
 * 
 * This function echos data back to the serial port.
 *
 * @param fd The file descriptor of the serial port.
 * @param buffer The buffer containing the data to be echoed.
 * @param buffer_size The size of the buffer.
 * @return The number of bytes echoed, or -1 if an error occurred.
 */
int serial_port_echo(int fd, const char *buffer, int buffer_size);

#endif // SERIAL_PORT_H
