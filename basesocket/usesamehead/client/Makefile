OBJS = main.cpp

CXX = g++

HEADER_DIR = -I .*h ../Heads/*.h

TARGET = z_client 

$(TARGET): $(OBJS)
	    $(CXX) -std=c++11 -o $(TARGET) $(HEADER_DIR) $(OBJS) -lcrypto

clean:
	rm -rf *gch
	rm -rf z_client
