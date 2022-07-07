/* -read a file in standard C */
/* -probably according to the first line set a flag for init, something like #RUNIT */
/* -parse the file with regex magic */
/* -pass to a struct system that handles the relevant strings and stufd */
/* -pass to a function 5that reads the *struct and converts to desired init with 50 switch statements */
#include <stdio.h>

struct depend{ // dont know if its good programming but another struct so it could be changed to somehow acommodate recursive dependencies, runit is ignored, s6 is a future problem
	char * type; //before,after;wants=use,requires=need
				 //; provides, wantedBy is eh //TODO
	char * value;
	/* char *target; //TODO its a bit of a pain */
};

struct characteristics{
	char * command; //Exec Command
	char ** command_args; //Arguments, multiple so [][], gotta filter systemd
	char *description; //description, simple 
	int supervisor; //for now int, as openrc uses supervise-daemon and runit LOGGING ENABLE in conf  
	char *pidfile; //Pid file of the process, gonna be NULL(none),RUNIT(if its runnit it means its a symlink, maybe the conversion will be made to just a regular ol' pid) or REGULAR /run/PROGRAM.pid
	struct depend *dependencies; //deps obvs
};

FILE *OpenFile(char *fp);

int GetType(FILE *fp); //for setting Service.Type


int OpenRC_Parser(FILE *fp);

int SystemD_Parser(FILE *fp);

void OpenRC_Writer(struct characteristics);
void SystemD_Writer(struct characteristics);

int main(int argc, char **argv){
	int input,target;
	FILE *fin, *fout;
	struct characteristics Service; 
	fin = OpenFile(argv[1]);
	//PASS TO HANDLER
	switch (input) { 
		case 1:
			OpenRC_Parser(fin);break;
		case 2:
			SystemD_Parser(fin);break;
		default:
			printf("error\n");;break;
	}

	switch (target){
				case 1:
			OpenRC_Writer(Service);break;
		case 2:
			SystemD_Writer(Service);break;
		default:
			printf("error\n");;break;
	}

	fclose(fin);
	fclose(fout);
}

FILE *OpenFile(char *path){
	FILE *fp;
	if ((fp = fopen(path,"r")) == NULL) {
		printf("error opening %s\n",path);
		return NULL;
	}
	return fp;
}
