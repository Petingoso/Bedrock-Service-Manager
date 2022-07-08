/*
 -read a file in standard C
 -probably according to the first line set a flag for init, something like #RUNIT
 -parse the file with regex magic
 -pass to a struct system that handles the relevant strings and stufd
 -pass to a function 5that reads the *struct and converts to desired init with 50 switch statements
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct depend{ // dont know if its good programming but another struct so it could be changed to somehow acommodate recursive dependencies, runit is ignored, s6 is a future problem
	char * type; //before,after;wants=use,requires=need
				 //; provides, wantedBy is eh //TODO
	char * value;
	/* char *target; //TODO its a bit of a pain */
};

struct characteristics{
	char * name; //Self Explains
	char * command; //Exec Command
	char * command_args; //Arguments, multiple so [][], gotta filter systemd nvm just paste the whole string lol
	char *description; //description, simple
	int supervisor; //for now int, as openrc uses supervise-daemon and runit LOGGING ENABLE in conf
	char *pidfile; //path to pid file
	struct depend *dependencies; //deps obvs
};

FILE *OpenFile(char *fp);

int GetType(FILE *fp); //for setting Service.Type

char * FilterBefore(char *name,char letter);
char * FilterAfter(char *name,char letter);

void GetPidFile();

int OpenRC_Parser(FILE *fp);

int SystemD_Parser(FILE *fp);

void OpenRC_Writer(struct characteristics);
void SystemD_Writer(struct characteristics);

struct characteristics Service;

int main(int argc, char **argv){
	int input,target;
	FILE *fin, *fout;
	fin = OpenFile(argv[1]);
	//PASS TO HANDLER
	Service.name=FilterBefore(argv[1],'.');
	switch (input) {
		case 1:
			OpenRC_Parser(fin);break;
		case 2:
			SystemD_Parser(fin);break;
		default:
			printf("Error\n");;break;
	}

	switch (target){
				case 1:
			OpenRC_Writer(Service);break;
		case 2:
			SystemD_Writer(Service);break;
		default:
			printf("Error\n");break;
	}

	fclose(fin);
	fclose(fout);
}

FILE *OpenFile(char *path){
	FILE *fp;
	if ((fp = fopen(path,"r")) == NULL) {
		printf("Error opening %s\n",path);
		return NULL;
	}
	return fp;
}

int GetType(FILE *fp){
	char line[31];
	fgets(line,30,fp);
	printf("line = %s\n",line);
	if (strstr(line,"openrc-run")){
		printf("It's OpenRC\n");
		return 1;
	}

	if(strstr(line,"Unit")){
		printf("It's systemd\n"); //systemd isn't capped lol
		return 2;
	}
	return 0;
}

char * FilterBefore(char *name,char letter){
	int i;
	for(i=0;name[i]!=letter & name[i]!='\0';i++){
		; 
	}
	name[i]='\0';
	return name;
}


char * FilterAfter(char *name,char letter){
	int i,count=0;
	for(i=strlen(name)-1 ;name[i]!=letter & name[i]!='\0';i--){
	}
	name=&name[i+1];
	return name;
}


void GetPidFile(){
	snprintf(Service.pidfile,50,"/run/%s.pid",Service.name);
}


int OpenRC_Parser(FILE *fp){
	char line[151];
	Service.supervisor=1;
	GetPidFile();
	while (fgets(line,150,fp) != NULL) {
		if (strstr(line,"command=")){
			Service.command=FilterAfter(line,'=');
		}
		if (strstr(line,"command_args=")){
			Service.command_args=FilterAfter(line,'=');
		}
		if (strstr(line,"description=")){
			Service.description=FilterAfter(line,'=');
		}
	}
	/* read a string 
	 * check if its command,command args,pidfile*/
	/* also check if its depend,start,stop or restart (pain)*/ 
	/* parse accordingly and set Service values*/ 
	return 0;
}


