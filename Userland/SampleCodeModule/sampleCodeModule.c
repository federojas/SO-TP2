/* sampleCodeModule.c */
#include <shell.h>
#include <userSyscalls.h>
#include <libc.h>

int main(){
	char *argv[] = {"Shell"};
    newProcess(shellExecute, 1, argv, BACKGROUND, NULL);
	return 0;
}
