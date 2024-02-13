#include "timeformat.h"
#include "mystrlib.h"
#include <string.h>
#include <time.h>


void formatTimePeriod(char* buffer,const char* fmt,unsigned long period){
    unsigned int minutes = period/60;
	unsigned int hours = minutes/60;
    minutes -= hours * 60;
	unsigned int seconds = period%60;

    int i=0;
    const char* pfmt=fmt;

    for(;*pfmt;pfmt ++){
        if(*pfmt == '%'){
            pfmt++;
            if(! *pfmt) break;

            if(*pfmt == 'h'){
                // hour
                i += sprintInt(buffer +i, hours);

            }else if(*pfmt == 'm'){
                buffer[i++] = '0' + (minutes /10);
                buffer[i++] = '0' + (minutes %10);                
            }else if(*pfmt == 's'){
                buffer[i++] = '0' + (seconds /10);
                buffer[i++] = '0' + (seconds %10);
            }
        }else{
            buffer[i] = *pfmt;
            i++;
        }
    }
    buffer[i]='\0';

}


void formatTime(char* buffer,const char* fmt,unsigned long epoc){
    
    struct tm *t= localtime(&epoc);

    int i=0;
    const char* pfmt=fmt;

    for(;*pfmt;pfmt ++){
        if(*pfmt == '%'){
            pfmt++;
            if(! *pfmt) break;

            if(*pfmt == 'h'){
                // hour
                buffer[i++] = '0' + (t->tm_hour /10);
                buffer[i++] = '0' + (t->tm_hour %10);                
            }else if(*pfmt == 'm'){
                // minutes
                buffer[i++] = '0' + (t->tm_min /10);
                buffer[i++] = '0' + (t->tm_min %10);                
            }else if(*pfmt == 's'){
                // seconds
                buffer[i++] = '0' + (t->tm_sec /10);
                buffer[i++] = '0' + (t->tm_sec %10);
            }else if(*pfmt == 'd'){
                // month day  tm_mday 1-31
                buffer[i++] = '0' + (t->tm_mday /10);
                buffer[i++] = '0' + (t->tm_mday %10);
            }else if(*pfmt == 'M'){
                // month,tm_mon 0-11
                int mon=t->tm_mon +1;
                buffer[i++] = '0' + (mon /10);
                buffer[i++] = '0' + (mon %10);
            }else if(*pfmt == 'D'){
                // week of the day, 0-6
                static const char* SymbolDaysWeek[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
                strcpy(buffer+i,SymbolDaysWeek[t->tm_wday]);
                i += 3;
            }else if(*pfmt == 'y'){
                // two digit
            }else if(*pfmt == 'Y'){
            }

        }else{
            buffer[i] = *pfmt;
            i++;
        }
    }
    buffer[i]='\0';
}
