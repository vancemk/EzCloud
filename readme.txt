1, how to write the data from databuffer?
TCPConnection: writeData()
	ret = _socket.write(_output.getData(), _output.getDataLen());
Socket: write(const void *data, int len)
	res = write(_socketHandle, data, len);

2,recv side: how to read data from databuferr?
TCPConnection: readData()
	ret = read(_input.getFree(), _input.getFreeLen());
Socket: read(void *data, int len)
	res = read(_socketHandle, data, len);
