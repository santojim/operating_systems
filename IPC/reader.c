////////////////////////////////////////////////////////////////////////////////
////////////////////   R E A D E R       ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "myRecordDef.h"
#include <time.h>
#include <semaphore.h>
#include <sys/ipc.h>

int main(int argc,char *argv[])
{
////////////////////////////////////////////////////////////////////////////////
////////// V A R I A B L E S   D E C L A R A T I O N    ////////////////////////
////////////////////////////////////////////////////////////////////////////////
int lower_bound,upper_bound;
int period,shmid,semid;
int i;
int retval,prev,current;
MyRecord *mem;
FILE *rOutput;
sem_t *spna,*spnw,*spc;
int *glbv;

////////////////////////////////////////////////////////////////////////////////
//////////////    S T O R E   T H E   V A R I A B L E S           //////////////
////////////////////////////////////////////////////////////////////////////////
if(argc==10)
{
	for(i=1;i<argc;i++)
	{
		if(!strcmp(argv[i],"-r"))
		{
			lower_bound=atoi(argv[i+1]);
			upper_bound=atoi(argv[i+2]);
		}
		else if(!strcmp(argv[i],"-d"))
		{
			period=atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-s"))
		{
			shmid=atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-p"))
		{
			semid=atoi(argv[i+1]);
		}
	}
}
else
{	if (argc>10)
		printf("Error: you gave too many arguments,please try again\n");
	else
		printf("Error: you gave less arguments,please try again\n");
	return(1);
}
//////////////// C H E C K  F O R  C O R R E C T  B O U N D S //////////////////
if (lower_bound>upper_bound)
	{
		printf("\nERROR : incorrect bounds given\n");
		return 1;
	}
////////////////////////////////////////////////////////////////////////////////
///////////// A T A C H  T H E  S E M A P H O R E S  S E G M E N T /////////////
////////////////////////////////////////////////////////////////////////////////
	spna  = (sem_t *) shmat(semid,(void*) 0, 0);
	spnw  = (sem_t *) (spna+sizeof(sem_t));
	spc  = (sem_t *) (spnw+sizeof(sem_t));
	glbv = (int *)	 (spc+sizeof(int));
///////////////////  s e m a p h o r e   h a n d l i n g   /////////////////////
	sem_wait(spnw);
	sem_wait(spc);
	prev=*glbv;
	*glbv=*glbv+1;
	sem_post(spc);
	if (prev==0)
		sem_wait(spna);
	sem_post(spnw);
////////////////////////////////////////////////////////////////////////////////
///P R I N T  R E A D E R  I S  I N  C R I T I C A L  S E C T I O N ////////////
////////////////////////////////////////////////////////////////////////////////
printf("\n\t----->Reader with processID: %d ,is --IN-- Critical section \n",getpid());
////////////////////////////////////////////////////////////////////////////////
/////// R E A D  T H E  R E C O R D S  F R O M  M E M O R Y ////////////////////
////////////////////////////////////////////////////////////////////////////////
mem = shmat(shmid, (void*)0, 0);
////////////////////   C A S E  O F  W R O N G  M E M  I D    //////////////////
if (mem==(void *) -1)
{
	printf("\nERROR! : wrong memory id given\n");
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
///////////////////// F I L E  O U T P U T /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
rOutput=fopen("Reader_Output.txt","a");
fprintf(rOutput,"\n\t---------Reader with id:%d at time : %ld ---------\n ",getpid(),time(NULL));
mem=mem+lower_bound;/*3ekina na diabazeis apo to lower -> upper*/
for(i=lower_bound;i<=upper_bound;i++)
{
	fprintf(rOutput,"%d %s %s %d\n",mem->ssn,mem->LastName,mem->FirstName,mem->income);
	mem++;
}
///////////////////  s e m a p h o r e   h a n d l i n g   /////////////////////

sleep(period);
/////////////////////////// F C L O S E ////////////////////////////////////////
fclose(rOutput);

printf("\n\t<-----Reader with processID: %d ,is --OUT-- of the critical section ,rest in peace\n",getpid());
sem_wait(spc);
*glbv=*glbv-1;
current=*glbv;
sem_post(spc);
if (current==0)
	sem_post(spna);
return 0;
}
