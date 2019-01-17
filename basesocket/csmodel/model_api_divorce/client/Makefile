OBJS = main.cpp

CXX = g++

HEADER_DIR = -I .*h

TARGET = client 

$(TARGET): $(OBJS)
	    $(CXX) -std=c++11 -o $(TARGET) $(HEADER_DIR) $(OBJS) -lcrypto

clean:
	rm -rf *gch
