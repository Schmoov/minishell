#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	while (1)
	{
		sleep(1);
		printf("%d\n", getpid());
	}
}
