CHATCLIENT = chatClient
CXX = g++
CXXFLAGS = -std=c++0x
CXXFLAGS += -Wall
CXXFLAGS += -pedantic-errors
OBJS = ChatClient.o 

$(CHATCLIENT): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(CHATCLIENT)

ChatClient.o: ChatClient.c
	$(CXX) $(CXXFLAGS) -c ChatClient.c

clean:
	rm *.o $(CHATCLIENT)

debug: 
	valgrind --leak-check=full ./$(CHATCLIENT)

run:
	./$(CHATCLIENT)

.PHONY: clean debug run