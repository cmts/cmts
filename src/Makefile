include ./config

NODE_OBJS	+= \
	   $(TOPDIR)/node/node.o \
	   $(TOPDIR)/node/kill_process.o

NODE_TARGET	= $(TOPDIR)/bin/node

SCHEDULER_OBJS	+= \
		   $(TOPDIR)/scheduler/scheduler.o

SCHEDULER_TARGET	= $(TOPDIR)/bin/scheduler

all:node sched
	echo "OK"

sched:$(SCHEDULER_OBJS)
	mkdir -p $(TOPDIR)/bin
	$(CC) $(CFLAGS) -o $(SCHEDULER_TARGET) $(SCHEDULER_OBJS) $(LDFLAGS)

node:$(NODE_OBJS)
	mkdir -p $(TOPDIR)/bin
	$(CC) $(CFLAGS) -o $(NODE_TARGET) $(NODE_OBJS) $(LDFLAGS)


clean:
	$(DELETE) $(NODE_OBJS) $(NODE_TARGET)
	$(DELETE) $(SCHEDULER_OBJS) $(SCHEDULER_TARGET)
