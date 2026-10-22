cc := gcc

CFLAGS := -std=c11 -g -O0 -Wall -Wextra

TARGET := main

SRCS := $(wildcard *.c)

OBJS := $(patsubst %.c,%.o,$(SRCS))

DEPS := $(patsubst %.o,%.d,$(OBJS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(cc) $^ -o $@

%.o: %.c
	$(cc) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)
