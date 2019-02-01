1, how to write the data from databuffer?
TCPConnection: writeData()
	ret = _socket.write(_output.getData(), _output.getDataLen());
Socket: write(const void *data, int len)
	res = write(_socketHandle, data, len);

2, recv side: how to read data from databuferr?
TCPConnection: readData()
	ret = read(_input.getFree(), _input.getFreeLen());
Socket: read(void *data, int len)
	res = read(_socketHandle, data, len);


Tasklist:
1, readHead has't read the whole headinfo till the databuffer ends	【done】
2, better to make test easier
3, add the glog	【abeyance】
4, modify Makefile for two module use the same .h file	【done】
5, add the api of "transfer head only", "transfer head and file"	【abeyance】
6, add the working directory	【done】
7, Two-way communication	【working】
8, server side check the files to sync
