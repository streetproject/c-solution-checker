#include <unistd.h>

void	MQ_MyFunction(char *str)
{
	int index;

	index = 0;
	while (str[index] != '\0')
	{
		write(1, &str[index], 1);
		index++;
	}
	write(1, "\n", 1);
}

int	main()
{
	MQ_MyFunction("C Solution Checker");

	return (0);
}
