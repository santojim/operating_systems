////////////////////////////////////////////////////////////////////////////////
////////////////////     W R I T E R     ///////////////////////////////////////
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
////////////////////////////////////////////////////
/*V A R I A B L E S   D E C L A R A T I O N       */
////////////////////////////////////////////////////
int lower_bound,upper_bound;
int period,shmid,semid;
int i;
int retval;
FILE *wOutput;
MyRecord *mem;
sem_t *spna,*spnw,*spc;
int *glbv;
////////////////////////////////////////////////////
/*S T O R E   T H E   V A R I A B L E S */
////////////////////////////////////////////////////
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
	sem_wait(spna);
	sem_post(spnw);
////////////////////////////////////////////////////////////////////////////////
///P R I N T  R E A D E R  I S  I N  C R I T I C A L  S E C T I O N ////////////
////////////////////////////////////////////////////////////////////////////////
printf("\n\t----->Writer with processID: %d ,is ----IN---- Critical section \n",getpid());
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
wOutput=fopen("Writer_Output.txt","a");
fprintf(wOutput,"\n\t---------Writer with id:%d at time : %ld --------- \n ",getpid(),time(NULL));
mem=mem+lower_bound;/*3ekina na diabazeis apo to lower -> upper*/
///////////////// C H A N G E   T H E  R E C O R D S ///////////////////////////
for(i=lower_bound;i<=upper_bound;i++)
{
	mem->ssn=time(NULL);
	mem->income=getpid();
	mem++;
}
////////////////////////////////////////////////////////////////////////////////
//////////////// R E W I N D  T H E  m e m  P O I N T E R  /////////////////////
////////////////////////////////////////////////////////////////////////////////
for(i=lower_bound;i<=upper_bound;i++)
{
	mem--;
}
////////////////////////////////////////////////////////////////////////////////
///////////////////// W R I T E  T O  O U T P U T  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
for(i=lower_bound;i<=upper_bound;i++)
{
	fprintf(wOutput,"%d %s %s %d\n",mem->ssn,mem->LastName,mem->FirstName,mem->income);
	mem++;
}
sleep(period);
/////////////////////////// F C L O S E ////////////////////////////////////////
fclose(wOutput);
printf("\n\t<-----Writer with processID: %d ,is ----OUT---- of the critical section ,rest in peace\n",getpid());
///////////////////  s e m a p h o r e   h a n d l i n g   /////////////////////
sem_post(spna);
return 0;
}

