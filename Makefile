# Define compiler and compiler flags
CC = gcc
CFLAGS = -Wall -g

# Define the source directory and output directory
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Define the target executable name
TARGET = poker_ranks

# List the source files and object files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Default target (this will build your project)
all: $(BINDIR)/$(TARGET)

# Rule to build the executable
$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to compile each source file into an object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove object files and executable
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Optional rule to run the program
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)