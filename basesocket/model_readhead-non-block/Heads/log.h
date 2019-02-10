#include <stdio.h>

#ifndef LOG_H_
#define LOG_H_

#define log_msg(fmt, log_arg...)  \
	    printf("[%s:%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##log_arg)

#endif
