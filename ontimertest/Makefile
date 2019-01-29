CC=gcc
CFLAGS= -g  \
      -I../base/include
LIBS= -lpthread -lstdc++
ALL:ontime
ontime:ontimertest.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
