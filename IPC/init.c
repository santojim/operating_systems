////////////////////////////////////////////////////////////////////////////////
//////////////////////////// I N I T I A L I Z E  //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "myRecordDef.h"
#include <string.h>
#include <semaphore.h>


int main(int argc,char *argv[])
{
	int err=0;
	FILE *fpb;
	int id=0;
	long lSize;
	MyRecord rec;
	MyRecord *mem;
	int numOfrecords=0;
	int i;
	FILE *rOutput,*wOutput;
	int semid=0;
	sem_t *spnw,*spna,*spc;
	int retval;
	int *glbv;
	char option;
////////////////////////////////////////////////////////////////////////////////
/////////////////    F I L E  I N I T I A L I Z E     //////////////////////////
////////////////////////////////////////////////////////////////////////////////
rOutput=fopen("Reader_Output.txt","w");
wOutput=fopen("Writer_Output.txt","w");
////////////////////////////////////////////////////////////////////////////////
///////////////      O P E N  B I N A R Y  F I L E    //////////////////////////
////////////////////////////////////////////////////////////////////////////////
	if (argc!=2)
	{
		printf("Correct syntax is: %s BinaryFile\n", argv[0]);
      	return(1);
   	}
    fpb = fopen (argv[1],"rb");
    if (fpb==NULL) {
      	printf("Cannot open binary file\n");
      	return 1;
   	}
	else
		printf("\nFILE SUCCESSFULLY OPENED\n");

/////////////////// CHECK NUMBER OF RECORDs ////////////////////////////////////
    fseek (fpb , 0 , SEEK_END);
    lSize = ftell (fpb);
    rewind (fpb);
    numOfrecords = (int) lSize/sizeof(rec);
    printf("Records found in file %d \n\n", numOfrecords);

////////////////////////////////////////////////////////////////////////////////
/////////////// M A K E  S H A R E D  M E M O R Y  S E G M E N T  //////////////
////////////////////////////////////////////////////////////////////////////////
	id = shmget(IPC_PRIVATE,numOfrecords*sizeof( MyRecord ),0666|IPC_CREAT);
	if (id == -1)
		perror ("Creation");
	/* Attach the segment */
	mem = shmat(id, (void*)0, 0);
////////////////////////////////////////////////////////////////////////////////
///////////////S E M A P H O R E S  S H A R E D  M E M O R Y  S E G ////////////
////////////////////////////////////////////////////////////////////////////////
	semid = shmget(IPC_PRIVATE,3*sizeof(sem_t)+sizeof(int),0666);
	if (semid == -1)
		perror ("Creation");
////////////////////////////////////////////////////////////////////////////////
///////////// A T A C H  T H E  S E M A P H O R E S  S E G M E N T /////////////
////////////////////////////////////////////////////////////////////////////////
	spna  = (sem_t *) shmat(semid,(void*) 0, 0);
	spnw  = (sem_t *) (spna+sizeof(sem_t));
	spc  = (sem_t *) (spnw+sizeof(sem_t));
	glbv = (int *)	 (spc+sizeof(int));
////////////////////////////////////////////////////////////////////////////////
///////////// I N I T I A L I Z E  T H E  S E M A P H O R E  ///////////////////
////////////////////////////////////////////////////////////////////////////////
	retval = sem_init(spnw,1,1);
	retval = sem_init(spna,1,1);
	retval = sem_init(spc,1,1);
	*glbv   = 0;
////////////////////////////////////////////////////////////////////////////////
////W R I T E  T H E  R E C O R D S  I N  T H E  S H A R E D  M E M O R Y  /////
////////////////////////////////////////////////////////////////////////////////
   for (i=0; i<numOfrecords ; i++)
	{
      	fread(&rec, sizeof(rec), 1, fpb);
		(mem+i)->ssn=rec.ssn;
		strcpy((mem+i)->LastName,rec.LastName);
		strcpy((mem+i)->FirstName,rec.FirstName);
		(mem+i)->income=rec.income;
   	}
////////////////////////////////////////////////////////////////////////////////
////////////  P R I N T  F R O M  T H E  S H A R E D  M E M O R Y //////////////
////////////////////////////////////////////////////////////////////////////////
	printf("Do you want to print the records ? ['y' for yes 'n' for no] \n");
	scanf("%c",&option);
	if(option=='y')
		for (i=0; i<numOfrecords ; i++)
		{
			printf("%d \t",mem->ssn);
			printf("%s \t",mem->LastName);
			printf("%s \t",mem->FirstName);
			printf("%d ",mem->income);
			printf("\n");
			mem++;
		}
printf("\n\nMemory Successfully Allocated, MEMID= %d\n",(int)id);
printf("Semaphore Memory Successfully Allocated, SEM_MEM_ID= %d\n",(int)semid);
printf("Use this part for copy and paste : -p %d -s %d \n",(int)semid,(int)id);
////////////////////////// C L O S E  T H E  F I L E ///////////////////////////
fclose(fpb);


/* E N D   O F   M A I N */
	return 0;
}
