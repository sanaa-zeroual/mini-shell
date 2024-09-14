#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	char *arr;
	getopt(argc, &argv[1], arr);
	printf("%s\n", argv[1]);
}