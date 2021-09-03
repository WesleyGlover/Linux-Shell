#include "major2.h"

char *alias_name[50];
char *real_name[50];

int cc=0;

void aliasNew(char* argv[])
{
	char String1[522];	//String1 is used to process
	char String2[522];
//alias -c
	if(argv[1]==NULL){
		int i=0;
		printf("all active aliases:\n");
		while(i<50){
			if(alias_name[i]!=NULL)
				printf("%s\t\t%s\n",alias_name[i],real_name[i]);
			i++;
		}

	}
	else if (strcmp(argv[1],"-c") ==0)				//argv[1] =="-c".	"strcmp" returns 0 on SUCCESS.  
	{
		int i=0;
		while(i<50){
			if(alias_name[i]!=NULL){
				alias_name[i]=NULL;
				real_name[i]=NULL;
			}
			i++;
		}
	}
//alias -r alias_name
	else if(strcmp(argv[1],"-r") ==0) 			//argv[1] =="-r".      
	{
	    							//"unalias alias_name" syntax generation from "alias -r alias_name".
	    if(argv[2] == NULL) 							//NULL ptr for "alias_name" in "alias -r alias_name" syntax.  
	    {
	        printf("Null pointer reference. \"alias_name\" argument in \"alias -r alias_name\" is missing\n");
	    }
	    else{
		    int i=0;
		    while(i<50){
			    if(strcmp(argv[2],alias_name[i])==0){
				    alias_name[i]=NULL;
				    real_name[i]=NULL;
				    break;
			    }
			    i++;
		    }
	    }
	}
//alias alias_name='command';
	else if(strcmp(argv[0],"alias") ==0 && argv[1] !=NULL)	        
	{
		char *pch;
	    sprintf(String1,"%s %s",argv[1],argv[2]);	    //alias alias_name='command' generation.
	    pch = strtok(String1,"='");
	    alias_name[cc] = pch;
	    pch = strtok(NULL,"='");
	    real_name[cc] = pch;
	    cc++;
	}
}
