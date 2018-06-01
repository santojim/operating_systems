#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"header.h"
#include<sys/stat.h>
#include<unistd.h>


////////////////////////////////////////////////////////////////////////////////
///////////////////////// E N T I T Y  C O U N T ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void Ecount( char dirname [])
{
	char newdirname[256];
	DIR * dir_ptr ;
	struct dirent * direntp ;
	if ( ( dir_ptr = opendir ( dirname ) ) == NULL )
		fprintf (stderr , " cannot open %s \n " , dirname ) ;
	else
	{
		while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
		{	counter++;
//////////////////C A S E  I S  D I R E C T O R Y //////////////////////////////
			if (direntp->d_type==DT_DIR)
			{

/////////////////G O  I N  T H E  D I R E C T O R Y ////////////////////////////
				if(strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0)
				{
					strcpy(newdirname,dirname);
					strcat(newdirname,"/");
					strcat(newdirname,direntp->d_name);
/////////////////C A L L  R E C U R S I V E ////////////////////////////////////
					Ecount(newdirname);
					counter--;
				}
			}
			else;
		}
		closedir (dir_ptr);
		counter--;
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////// S Y N C H R O N I Z E ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void Synchronize(char dirname [],char *dest,int verbose)/*dwse san orismata arxiko katalogo k teliko gia antigrafh*/
{

struct stat source_statbuf ;

struct stat target_statbuf ;

int flag=0;
DIR * dir_ptr ;
struct dirent * direntp ;
char newdirname[256];

DIR * target_dir_ptr ;
struct dirent * target_direntp ;
char target_dirname[256];

FILE *fd,*targetfd;
int md;
char buff[50];


////////////////// O P E N  D I R E C T O R I E S //////////////////////////////
target_dir_ptr = opendir(dest);
dir_ptr=opendir(dirname);

if (dir_ptr==NULL)
	fprintf (stderr , " cannot open %s \n" , dirname ) ;
else if (target_dir_ptr==NULL)
	fprintf (stderr , " cannot open %s \n" , dest) ;
else
	{
/////////////// F O R  E A C H  E L E M E N T  I N  S O U R C E ////////////////
	while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
	{
////////////// S E A R C H  I N  T A R G E T ///////////////////////////////////
		while ( ( (target_direntp=readdir(target_dir_ptr))!= NULL ) )
		{
///////////////////// S A M E  N A ME  /////////////////////////////////////////
			if(strcmp(target_direntp->d_name,direntp->d_name)==0)
			{
////////////////// C A S E  A R E  D I R E C T O R I E S ///////////////////////
				if(target_direntp->d_type==DT_DIR && direntp->d_type==DT_DIR)
				{
					if(strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0 && strcmp((target_direntp->d_name),".")!=0  && strcmp((target_direntp->d_name),"..")!=0)
//////////////////// G O  I N  D I R E C T O R I E S ///////////////////////////
					{
						strcpy(newdirname,dirname);
						strcat(newdirname,"/");
						strcat(newdirname,direntp->d_name);
//////------------------------------------------------------////////////////////
						strcpy(target_dirname,dest);
						strcat(target_dirname,"/");
						strcat(target_dirname,target_direntp->d_name);
///////////////// C A L L  R E C U R S I V E ///////////////////////////////////
						Synchronize(newdirname,target_dirname,verbose);
					}
				}
////////// C A S E  A R E  F I L E S  W I T H  S A M E  N A M E ////////////////
				else
				{
					strcpy(newdirname,dirname);
					strcat(newdirname,"/");
					strcat(newdirname,direntp->d_name);
//////------------------------------------------------------////////////////////
					strcpy(target_dirname,dest);
					strcat(target_dirname,"/");
					strcat(target_dirname,target_direntp->d_name);
		//			printf("eimai ston fakelo %s \n",newdirname);
		//			printf("eimai ston fakelo %s \n",target_direntp->d_name);
					if (stat(newdirname,&source_statbuf)==-1 )
					{
						perror ( " Failed to get file status " );
						exit (2) ;
					}
					if (stat(target_dirname,&target_statbuf)==-1)
					{
						perror ( " Failed to get file status " );
						exit (2) ;
					}
		//			printf("\n\neimai to 1arxeio %s  + size =%lu \n",direntp->d_name,source_statbuf.st_size);
		//			printf("eimai to 2arxeio %s + size =%lu \n",target_direntp->d_name,target_statbuf.st_size);
//////////////// I F  D I F F R E N T  S I Z E  O R  D A T E /////////////////

					if (((source_statbuf.st_size)!=(target_statbuf.st_size)) || ((source_statbuf.st_mtime)>(target_statbuf.st_mtime)))
////////////////////// M A K E  A  F R E S H  C O P Y //////////////////////////
							{	copied_counter++;
								if (verbose==1)
									printf("\n file: %s is being replaced \n",direntp->d_name);
								fd=fopen(newdirname,"r");/*anoi3e to arxeio gia diabasma*/
								targetfd=fopen(target_dirname,"w");/*anoi3e ena arxeio me idio onoma*/
								int numBytes;
								while (numBytes=fread(buff, 1, 50, fd))
								{
									fwrite(buff, 1, numBytes, targetfd);
									//printf("..writing..\n");
								}
    							rewind (fd);
								fseek (fd , 0 , SEEK_END);
   								lSize =lSize+ftell(fd);

								fclose(targetfd);
								fclose(fd);
							}
				}
				flag=1;
				break;
			}
			else
			{
				flag=0;
			}
		}/*edw kleinei h eswterikh while*/
///////////////////// F L A G == 1 -> B R E 8 H K E /////////////////////////////
		if (flag==1);
		else
////////////////////////// N O T  F O U N D /////////////////////////////////////
		{
			ReadAndCopy(dirname,dest,verbose);
		}
		rewinddir(target_dir_ptr);
	}
	closedir(target_dir_ptr);
	closedir (dir_ptr);
	}
return;
}



//--psaxnei na brei ena arxeio ap to target sto source , an to brei epistrefei 1 , alliws to diagrafei-//
////////////////////////////////////////////////////////////////////////////////
///////////////////////// C H E C K S O U R C E ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void CheckSource(char dirname [],char *dest,int verbose)/*dwse san orismata arxiko katalogo k teliko gia antigrafh*/
{
int flag=0;
DIR * dir_ptr ;
struct dirent * direntp ;
char newdirname[256];

DIR * target_dir_ptr ;
struct dirent * target_direntp ;
char target_dirname[256];
////////////////// O P E N  D I R E C T O R I E S //////////////////////////////
target_dir_ptr = opendir(dest);
dir_ptr=opendir(dirname);

if (dir_ptr==NULL)
	fprintf (stderr , " cannot open %s \n" , dirname ) ;
else if (target_dir_ptr==NULL)
	fprintf (stderr , " cannot open %s \n" , dest) ;
else
	{
/////////////// F O R  E A C H  E L E M E N T  I N  T A R G E T ////////////////
	while ((target_direntp=readdir(target_dir_ptr))!= NULL)
	{
////////////// S E A R C H  I N  S O U R C E ///////////////////////////////////

			flag=0;
		while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
		{
			if(strcmp(target_direntp->d_name,direntp->d_name)==0)
			{
///////////////////// C A S E  A R E  D I R E C T O R I E S ////////////////////
				if(strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0 && strcmp((target_direntp->d_name),".")!=0  && strcmp((target_direntp->d_name),"..")!=0)
					if(target_direntp->d_type==DT_DIR && direntp->d_type==DT_DIR)
					{
						strcpy(newdirname,dirname);
						strcat(newdirname,"/");
						strcat(newdirname,direntp->d_name);
//////------------------------------------------------------////////////////////
						strcpy(target_dirname,dest);
						strcat(target_dirname,"/");
						strcat(target_dirname,target_direntp->d_name);

/////////////////C A L L  R E C U R S I V E ////////////////////////////////////
						CheckSource(newdirname,target_dirname,verbose);
					}
				flag=1;
				break;
			}
			else
			{
				flag=0;
			}
		}
		if (flag==1);
		else
		{
			if (verbose==1)
				printf("\n--------------- the %s was not found------------- \n ",target_direntp->d_name);
			strcpy(newdirname,dest);
			strcat(newdirname,"/");
			strcat(newdirname,target_direntp->d_name);
			DeleteTarget(newdirname,verbose);
		}
		rewinddir(dir_ptr);
	}
	closedir (dir_ptr);
	closedir(target_dir_ptr);
	}
return;
}




////////////////////////////////////////////////////////////////////////////////
///////////////////////// D E L E T E  T A R G E T /////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void DeleteTarget(char dirname[],int verbose)
{
	char newdirname[256];
	DIR * dir_ptr ;
	struct dirent * direntp ;
//////////////////  O P E N D I R //////////////////////////////////////////////
	if ( ( dir_ptr = opendir ( dirname ) ) == NULL )
	{
////////////// C A S E  O N L Y  F I L E  T O  B E  D E L E T E D //////////////
		if(remove(dirname)==0)
			printf("file %s successfully removed !\n",dirname);
		//fprintf (stderr , "opendir cannot open %s \n " , dirname ) ;
	}
	else
	{
		while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
		{
//////////////////C A S E  I S  D I R E C T O R Y //////////////////////////////
			if (direntp->d_type==DT_DIR)
			{
///////////////// G O  I N  T H E  D I R E C T O R Y ///////////////////////////
				if(strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0)
				{
					strcpy(newdirname,dirname);
					strcat(newdirname,"/");
					strcat(newdirname,direntp->d_name);
///////////////// C A L L  R E C U R S I V E ///////////////////////////////////
					DeleteTarget(newdirname,verbose);
//////// W H E N  R E C U R S E  F I N I S H E S  C A L L  R M D I R ///////////
					if(remove(newdirname)==0)
						if (verbose==1)
							printf("folder %s , successfully removed ! \n",newdirname);
				}
			}
///////////////////////// C A S E  O F  F I L E ////////////////////////////////
			else
			{
				strcpy(newdirname,dirname);
				strcat(newdirname,"/");
				strcat(newdirname,direntp->d_name);
				if (remove(newdirname)==0)
					if (verbose==1)
						printf("file %s , successfully removed \n",newdirname);
			}
		}
		if(remove(dirname)==0)
			if (verbose==1)
				printf("folder %s , successfully removed ! \n",dirname);
		closedir (dir_ptr);
		return;
	}
}



/*AUTH 8a EPISTREFEI MIA TIMH EAN DEN MPOREi NA DIMIOURHSEI FAKELO POU UPARXEI HDH*/
////////////////////////////////////////////////////////////////////////////////
/////////////////////////R E A D  A N D  C O P Y ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////


void ReadAndCopy(char dirname [],char *dest,int verbose)/*dwse san orismata arxiko katalogo k teliko gia antigrafh*/
{
FILE *fd,*targetfd;
int md;
char buff[50];

DIR * dir_ptr ;
struct dirent * direntp ;
char newdirname[256];

DIR * target_dir_ptr ;
struct dirent * target_direntp ;
char target_dirname[256];
////////////////// O P E N  D I R E C T O R I E S //////////////////////////////
target_dir_ptr = opendir(dest);
dir_ptr=opendir(dirname);

if (dir_ptr==NULL)
	{
		fprintf (stderr , " cannot open %s \n" , dirname ) ;
		return;
	}
else if (target_dir_ptr==NULL)
	{
		fprintf (stderr , " cannot open %s \n" , dest) ;
		return;
	}
else
	{
////////////// R E A D  F R O M  S O U R C E ///////////////////////////////////
		while ( ( direntp = readdir ( dir_ptr ) ) != NULL )
		{
//////////////////C A S E  I S  D I R E C T O R Y //////////////////////////////
		if (direntp->d_type==DT_DIR)
		{
///////  M A K E  D E S T I N A T I O N  D I R E C T O R Y  ////////////////////
			if( strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0 )
			{
				strcpy(target_dirname,dest);/*copy the destination path to target_dirname array*/
				strcat(target_dirname,"/");/*add to the path the "/" symbol*/
				strcat(target_dirname,direntp->d_name);/*add to the path the name of folder to be copied*/
				md=mkdir(target_dirname,0777);/*call the mkdir with the complete path*/
				if (md==0)
				{	copied_counter++;/*count+1 for new folders*/
					if (verbose==1)
						printf("\n folder: %s successfully created \n",target_dirname);
				}
			}
/////////////////G O  I N  T H E  D I R E C T O R Y ////////////////////////////
			if(strcmp((direntp->d_name),".")!=0  && strcmp((direntp->d_name),"..")!=0)
			{
				strcpy(newdirname,dirname);
				strcat(newdirname,"/");
				strcat(newdirname,direntp->d_name);
/////////////////   C A L L  R E C U R S I V E /////////////////////////////////
				ReadAndCopy(newdirname,target_dirname,verbose);
			}
		}
//////////////////// C A S E  I S  F I L E /////////////////////////////////////
		else
		{
				strcpy(newdirname,dirname);/*copy the destination path to target_dirname array*/
				strcat(newdirname,"/");/*add to the path the "/" symbol*/
				strcat(newdirname,direntp->d_name);/*add to the path the name of folder to be copied*/
				fd=fopen(newdirname,"r");/*anoi3e to arxeio gia diabasma*/

				strcpy(target_dirname,dest);/*copy the destination path to target_dirname array*/
				strcat(target_dirname,"/");/*add to the path the "/" symbol*/
				strcat(target_dirname,direntp->d_name);/*add to the path the name of folder to be copied*/
			if(access(target_dirname,F_OK)==0);/*if a file exist do nothing*/
			else/*else create it*/
			{	copied_counter++;/*count+1 for new files*/
				if (verbose==1)
					printf("\nfile: %s is being copied \n",direntp->d_name);
				targetfd=fopen(target_dirname,"w");/*anoi3e ena arxeio me idio onoma*/
				int numBytes;
				while (numBytes=fread(buff, 1, 50, fd))
				{
					fwrite(buff, 1, numBytes, targetfd);
					//printf("..writing..\n");
				}
				rewind (fd);
				fseek (fd , 0 , SEEK_END);
   				lSize =lSize+ftell(fd);

				fclose(targetfd);
				fclose(fd);
			}
		}
//////////////// C L O S E  D I R E C T O R I E S  /////////////////////////////
		}
		closedir (dir_ptr);
		closedir (target_dir_ptr);
	}
return;
}


