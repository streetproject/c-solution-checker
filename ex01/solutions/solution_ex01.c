#include <unistd.h>

void	MQ_MyFunction(char ch)
{
	write(1, &ch, 1);
}


int	main()
{
	MQ_MyFunction('Z');

	return (0);
}
