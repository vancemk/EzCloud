OBJS = main.cpp

HEADER_DIR = -I .*h

TARGET = client 

$(TARGET): $(OBJS)
	    $(CXX) -o $(TARGET) $(HEADER_DIR) $(OBJS) -lcrypto

clean:
	rm -rf *gch
