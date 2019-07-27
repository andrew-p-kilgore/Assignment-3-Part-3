#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
static int timer_expired = 0;

static void alarm_handler(int sig)
{
timer_expired=30;
}

int main()
{
sigaction(SIGALRM, &(struct sigaction){.sa_handler = alarm_handler}, NULL);

time_t t;
srand((unsigned) time(&t));

timer_expired = 0;

alarm(1);

while (!timer_expired) {
	int s=(rand()%4)+1;
	printf("%d", s);
}
return 0;
}
