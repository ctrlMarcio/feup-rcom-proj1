# C compiler
CC = gcc

# Directory naming
BIN_DIR = bin
SRC_DIR = src

# Executables naming
SENDER_EXECUTABLE = sender
RECEIVER_EXECUTABLE = receiver

# Compiling extensions
COMPILATION_EXTENSIONS = -Wall -lm

# The names of the .c source files
COMMON_SRC = ${SRC_DIR}/data_link/data_link.c ${SRC_DIR}/data_link/sender/writenoncanonical.c \
			${SRC_DIR}/data_link/receiver/readnoncanonical.c ${SRC_DIR}/util/util.c \
			${SRC_DIR}/data_link/util/state_machine.c ${SRC_DIR}/data_link/common.c \
			${SRC_DIR}/data_link/util/serial_port.c ${SRC_DIR}/application/application_receiver.c ${SRC_DIR}/application/util/util.c \
			${SRC_DIR}/application/application_sender.c ${SRC_DIR}/error/error.c 
SENDER_SRC = ${SRC_DIR}/sender.c ${COMMON_SRC}
RECEIVER_SRC = ${SRC_DIR}/receiver.c ${COMMON_SRC}
SRC_FILES = ${SENDER_SRC} ${RECEIVER_SRC}

make: ${SRC_FILES}
	@mkdir -p $(BIN_DIR)
	@$(CC) -o $(BIN_DIR)/${SENDER_EXECUTABLE} ${SENDER_SRC} ${COMPILATION_EXTENSIONS};
	@$(CC) -o $(BIN_DIR)/${RECEIVER_EXECUTABLE} ${RECEIVER_SRC} ${COMPILATION_EXTENSIONS}

clean:
	@rm -f {BIN_DIR}/*