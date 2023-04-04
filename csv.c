#include "fundamental_config.h"
#include "fundamental.h"
#include "csv.h"
#include "utils.h"
#ifdef REAL_HUNTER
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

off_t getfilelen(int fd)
{
   struct stat buf;

   if(fstat(fd,&buf))
      exit(errno);
   return buf.st_size;
}

void parselines(char *buff,off_t len,char ***lines,int *num_csv_lines)
{
   off_t curr,lastline;
   int currline;
   buff[len]=0;
   
   *num_csv_lines=0;
   for(curr=0;curr<len;curr++)
   {
      if(buff[curr]==0xa)
	 (*num_csv_lines)++;
   }
   if(buff[len-1]!=0xa)
      (*num_csv_lines)++;
   if((*lines=(char **)malloc(((*num_csv_lines)+1)*sizeof(char*)))==NULL)
   {
      fprintf(stderr,"parselines: failed to allocate memory\n");
      exit(errno);
   }
   lastline=0;
   currline=0;
   for(curr=0;curr<len;curr++)
   {
      if(buff[curr]==0xa)
      {
	 buff[curr]=0;
	 (*lines)[currline++]=&buff[lastline];
	 lastline=curr+1;
      }
   }
   (*lines)[(*num_csv_lines)-1]=&buff[lastline];
   buff[len]=0;
   (*lines)[*num_csv_lines]=&buff[len];
}

void readreals(char *filename)
{
   int num_csv_lines=0;
   int i,j,k,num_commas0,num_commas,readlen,currlen;
   char **lines,*line,*endptr;
   int *strlens;
   off_t filelen;
   int fd;
   char *buff;
   if((fd=open(filename,O_RDONLY))<0)
   {
      fprintf(stderr,"Couldn't open %s exiting\n",filename);
      exit(errno);
   }
   filelen=getfilelen(fd);
   if((buff=malloc(filelen+1))==NULL)
      exit(errno);
   /* if read doesn't read the whole file in one go this will break */
   currlen=0;
   while(readlen=read(fd,buff,filelen)!=0)
   {
      if(readlen==0)
	 exit_error("Bad file read\n");
      currlen+=readlen;
      if(currlen>=filelen)
	 break;
   }
   close(fd);
   parselines(buff,filelen,&lines,&num_csv_lines);
   num_commas0=0;
   for(i=0;i<num_csv_lines;i++)
   {
      line=lines[i];
      num_commas=0;
      while(*line!=0)
      {
	 if(*line==',')
	 {
	    if(i==0)
	       num_commas0++;
	    else
	       num_commas++;
	    if(*(line+1)==',')
	       exit_error("Too many commas in csv");
	 }
	 line++;
	 
      }
      if(i==0)
      {
	 if(num_commas0<1)
	    exit_error("bad num_commas0 in cvs file\n");
      }
      else
	if(num_commas0!=num_commas)
	  exit_error("bad num_commas in line %d\n",i);
   }
   if(num_commas!=NUM_HUNTER_DIMENSIONS)
      exit_error("num_commas!=NUM_HUNTER_DIMENSIONS");
   strlens=(int *)calloc(num_csv_lines,sizeof(int));
   if(strlens==NULL)
      exit_error("calloc strlens failed");
   for(i=0;i<num_csv_lines;i++)
      strlens[i]=strlen(lines[i]);
   for(i=0;i<num_csv_lines;i++)
   {
      line=lines[i];
      k=strlens[i];
      for(j=0;j<k;j++)
      {
	 if(line[j]==',')
	    line[j]=0;
      }
   }
   coord_and_result_array=calloc((NUM_HUNTER_DIMENSIONS+1)*num_csv_lines,sizeof(number_t));
   if(coord_and_result_array==NULL)
     exit_error("failed to allocate coord_and_result_array");
   for(i=0;i<num_csv_lines;i++)
   {
      line=lines[i];
      for(j=0;j<=NUM_HUNTER_DIMENSIONS;j++)
      {
	while(isspace(*line))
	  line++;
	 coord_and_result_array[((NUM_HUNTER_DIMENSIONS+1)*i)+j]=strtod(line, &endptr);
	 if(endptr==NULL)
	    exit_error("bad_number");
	 while(isspace(*endptr))
	   endptr++;
	 line=endptr+1;
      }
   }
   num_real_coord_idx=num_csv_lines-1;
   free(buff);
   free(lines);
}
#endif
