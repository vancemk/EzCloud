#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>



int standard_to_stamp(const char *str_time)  
{  
        struct tm stm;  
        int iY, iM, iD, iH, iMin, iS;  

        memset(&stm,0,sizeof(stm));  
        iY = atoi(str_time);  
        iM = atoi(str_time+5);  
        iD = atoi(str_time+8);  
        iH = atoi(str_time+11);  
        iMin = atoi(str_time+14);  
        iS = atoi(str_time+17);  

        stm.tm_year=iY-1900;  
        stm.tm_mon=iM-1;  
        stm.tm_mday=iD;  
        stm.tm_hour=iH;  
        stm.tm_min=iMin;  
        stm.tm_sec=iS;  
		
		/* 2016:08:02 12:12:30" */
        /* printf("%d-%0d-%0d %0d:%0d:%0d\n", iY, iM, iD, iH, iMin, iS);*/   
        return (int)mktime(&stm);  
}  

typedef struct times
{
        int Year;
        int Mon;
        int Day;
        int Hour;
        int Min;
        int Second;
}Times;

Times stamp_to_standard(int stampTime)
{
        time_t tick = (time_t)stampTime;
        struct tm tm; 
        char s[100];
        Times standard;

        //tick = time(NULL);
        tm = *localtime(&tick);
        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
        printf("%d: %s\n", (int)tick, s); 


        standard.Year = atoi(s);
        standard.Mon = atoi(s+5);
        standard.Day = atoi(s+8);
        standard.Hour = atoi(s+11);
        standard.Min = atoi(s+14);
        standard.Second = atoi(s+17); 
        return standard;
}


int main(){
	int iStampTime =  standard_to_stamp("2018:08:02 12:12:30");
	printf("201810111222 convert timestamp: %d\n", iStampTime);
	return 0;
}







