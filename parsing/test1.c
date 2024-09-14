#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
#include <stdio.h>
#define EXIT_FAILURE 1
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// char **lsh_split_line(char *line)
// {
//   int bufsize = LSH_TOK_BUFSIZE, position = 0;
//   char **tokens = malloc(bufsize * sizeof(char*));
//   char *token;

//   if (!tokens) {
//     fprintf(stderr, "lsh: allocation error\n");
//     exit(EXIT_FAILURE);
//   }

//   token = strtok(line, LSH_TOK_DELIM);
//   while (token != NULL) {
//     tokens[position] = token;
//     position++;

//     if (position >= bufsize) {
//       bufsize += LSH_TOK_BUFSIZE;
//       tokens = realloc(tokens, bufsize * sizeof(char*));
//       if (!tokens)
//       {
//         fprintf(stderr, "lsh: allocation error\n");
//         exit(EXIT_FAILURE);
//       }
//     }

//     token = strtok(NULL, LSH_TOK_DELIM);
//   }
//   tokens[position] = NULL;
//   return tokens;
// }
// int main()
// {
//     int i;
//     char *input = readline("test=> ");
//     char **tokens = lsh_split_line(input);
//     i = 0;
//     while (tokens[i])
//     {
//         printf("data = %s\n", tokens[i]);
//         i++;
//     }
// }

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char *rest = ttyname(2);
    printf("result = %s\n", rest);
}

// The isatty() function is used to determine whether a file descriptor refers to a terminal device (like a keyboard or screen in an interactive session) rather than a regular file, pipe, or other types of files.