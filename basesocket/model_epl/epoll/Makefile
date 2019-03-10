OBJS = main.cpp

HEADER_DIR = -I .*h 

TARGET = z_server

$(TARGET): $(OBJS)
	    $(CXX) -o $(TARGET) $(HEADER_DIR) $(OBJS) -lcrypto -g

clean:
	rm -rf *gch
	rm -rf z_server
