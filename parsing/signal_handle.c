#include "../minishell.h"

#define SIGINT 2    // Ctrl+C
#define SIGQUIT 3   // Ctrl+D
#define SIGTERM 15  // Termination signal (not Ctrl+D by default)

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void handle_ctrl_c()
{
    printf("\n");
	rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_ctrl_d()
{
    exit(127);
}

void handle_signal()
{
    signal(SIGINT, handle_ctrl_c);
    signal(SIGTERM, handle_ctrl_d);
    
}

