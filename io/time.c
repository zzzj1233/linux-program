#include<time.h>
#include "stdio.h"

int main() {
    time_t t;
    time(&t);

    struct tm *pt = localtime(&t);

    printf("%d-%d-%d %d:%d:%d \n ", pt->tm_year, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
}