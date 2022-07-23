#include<stdio.h>
#include<sys/ioctl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include <signal.h>

#define MAGIC 255
#define WRITE_VALUE _IOW(MAGIC,'a',int32_t*)
#define READ_VALUE _IOR(MAGIC,'b',int32_t*)
#define SIGETX 44

#define REG_CURRENT_TASK _IOW(MAGIC,'c',int32_t*)
#define CREATE_TASKLET _IOW(MAGIC,'d',int32_t*)

#define STAR '*'
#define HASH '#'


// Function Declaration
int main(void);
void printMenu(void);
void printChar(int,char);
void newLine(void);
void pre_exit_cleanup(void);

static int done = 0;
int check = 0;
 
void ctrl_c_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGINT) {
        printf("\nrecieved ctrl-c\n");
        done = 1;
    }
}
 
void sig_event_handler(int n, siginfo_t *info, void *unused)
{
    if (n == SIGETX) {
        check = info->si_int;
        printf ("Received signal from kernel : Value =  %u\n", check);
    }
}
int main(void){
	int32_t value, number;
	int selection,fd;
	struct sigaction act;

	printf("Initializing  of the User Space App.\n");
	printf("\nOpening Driver\n");
        fd = open("/dev/etx_device", O_RDWR);

	act.sa_flags = (SA_SIGINFO | SA_RESETHAND);
	act.sa_sigaction = ctrl_c_handler;
	sigaction (SIGINT, &act, NULL);
 
    /* install custom signal handler*/
    	sigemptyset(&act.sa_mask);
   	act.sa_flags = (SA_SIGINFO | SA_RESTART);
    	act.sa_sigaction = sig_event_handler;
    	sigaction(SIGETX, &act, NULL);
	printf("Installed signal handler for SIGETX = %d\n", SIGETX);

        //fd = open("/dev/etx_device", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }

	while(1){
	
		printMenu();
		scanf("%d" , &selection);
		
		switch(selection){
			case 1:
				
				printf("Writing to kernel \n");
				printf("\nInsert value to be written\n");
				scanf("%d" , &value);
				ioctl(fd, WRITE_VALUE , (int32_t*)&value);
				printf("Value read from kernal:%d\n", value);
				break;
			case 2: 
				printf("Reading Value from Driver\n");
			        ioctl(fd, READ_VALUE, (int32_t*) &value);
			        printf("Value is %d\n", value);
				break;
			case 4:
                                ioctl(fd,CREATE_TASKLET , (int32_t*)&value);
				break;

			case 8:
				pre_exit_cleanup();
				return 0;	
			default:
				printf("Invalid argument,Please select again");
				break;
		}
	
	}	
	pre_exit_cleanup();
	return 0;

}

void printMenu(void){
	newLine();
	printChar(60, HASH);newLine();
	printChar(60, STAR);newLine();
	printChar(17, STAR);
	printf("Welcome To UserSpace App");
	printChar(20, STAR);newLine();


        printf("\nPlease select choise from Menu items\n");
        printf("1. Write Values\n");
        printf("2. Read  Value\n");
        printf("3. List usb devices\n");
	printf("4. Create tasklate\n");
	printf("8. Exit\n");
	printChar(60, STAR);
	newLine();
}


//To generate slight Delay in UserSpace.
void pre_exit_cleanup(){
}

void printChar(int n , char c){
	while(n){
		printf("%c", c);
		n--;
	}	
}
void newLine(){
	printf("\n");
}


