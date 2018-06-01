#include<stdio.h>
#include<sys/types.h>
#include <sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"header.h"
#include <sys/times.h>
#include <time.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
int i;
int verbose;
int temp=0;

clock_t start,end;
struct tms tb1,tb2;
double ticspersec;

ticspersec=(double) sysconf(_SC_CLK_TCK);
start=(double) times(&tb1);
if (argc<3 || argc>4)
{
	printf("Wrong arguments ! \n");
	return 0;
}
verbose=0;
for(i=1;i<argc;i++)
	if(!strcmp(argv[i],"-v"))
	{
		verbose=1;
		temp=i;
	}
counter=0;
lSize=0;
copied_counter=0;
if (temp==1)
{
	Synchronize(argv[2],argv[3],verbose);
	CheckSource(argv[2],argv[3],verbose);
	Ecount(argv[2]);
}
else
{
	Synchronize(argv[1],argv[2],verbose);
	CheckSource(argv[1],argv[2],verbose);
	Ecount(argv[1]);
}
printf("--------------------------Number of bytes read : %ld \n",lSize);
printf("--------------------------Number of entities : %d \n",counter-1);
printf("--------------------------Number of entities copied : %d \n",copied_counter);
end=(double) times(&tb2);
printf("--------------------------in time %lf , ",(end-start)/ticspersec);
printf("at %lf bytes/sec \n",lSize/((end-start)/ticspersec));
return 0;
}



