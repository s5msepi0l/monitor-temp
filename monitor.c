#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h> //string tokenization
#include <stdint.h> // 8 bit integer
#include <unistd.h>

#define cmd_sz 256

char *excv(const char *cmd)
{
	FILE *pipe = popen(cmd, "r");
	if (!pipe)
		return NULL;

	char buffer[cmd_sz];
	char *tmp;
	char *res = malloc(1);
	res[0] = '\0';
		
	while (!feof(pipe))
	{
		if (fgets(buffer, cmd_sz, pipe) != NULL)
		{
			tmp = realloc(res, strlen(res) + strlen(buffer)	+1);
			if (tmp == NULL)
			{
				free(res);
				pclose(pipe);
				return NULL;
			}
			res = tmp;
			strcat(res, buffer);
		}
	}

	pclose(pipe);
	return res;
}

void c_remove(char *s, char *d)
{
	int j, n = strlen(s);

	for (int i = j = 0; i < n; i++)
	{
		if (s[i] != *d)
			s[j++] = s[i];

		s[j] = '\0';
	}
}

int main(int argc, char **argv)
{
	if (argc < 1)
	{
		fprintf(stderr, "enter max temperature: ");
		return 0;
	}

	float max_temp = atof(argv[1]);

	while(1)
	{	
		char *tmp = excv("sensors");
		char *tok = strtok(tmp, ":");
		char *res;

		for (int i = 0; i<5; i++)
		{
			tok = strtok(NULL, ":");
		}
		res = strtok(tok, "°"); //res = strtok(NULL, "C");
		c_remove(res, " ");
		c_remove(res, "+");
	
		free(tmp);
		float f_tmp = atof(res);
		printf("Current cpu temp: %f\n", f_tmp);
		
		if (f_tmp >= max_temp)
		{
			system("shutdown");
			return 0;
		}
		sleep(1);
	}
	

	return 0;
}
