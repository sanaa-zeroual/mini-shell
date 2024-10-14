#include "minishell.h"
t_envi *create_env_var(const char *name, const char *value)
{
    t_envi *new_env = (t_envi *)malloc(sizeof(t_envi));
    if (!new_env) exit(EXIT_FAILURE);
    new_env->name = strdup(name);
    new_env->vale = strdup(value);
    new_env->next = NULL;
    return new_env;
}

t_envi *int_env(char **envp)
{
    t_envi *env_list = NULL;
    t_envi *new_node;
    int i = 0;

    while (envp[i])
    {
        new_node = malloc(sizeof(t_envi));
        if (!new_node)
            return NULL;
        char *equal_sign = strchr(envp[i], '=');
        if (equal_sign)
        {
            new_node->name = strndup(envp[i], equal_sign - envp[i]);
            new_node->vale = strdup(equal_sign + 1); 
        }
        new_node->next = env_list;
        env_list = new_node;
        i++;
    }
    return env_list;
}


t_mini *initialize_mini(char **env)
{
    t_mini *box = (t_mini *)malloc(sizeof(t_mini));
    if (!box) exit(EXIT_FAILURE);
    box->env = int_env(env);
    box->ptr = NULL;
    box->arr = NULL;
    return box;
}

int	main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    char	    *input;
    Token	    **tokens;
    t_queue     *queue;
    t_ast       *ast;
    t_parser    *parsed;
    t_mini *box = initialize_mini(envp);
    (void)ast;
    tokens = NULL;
    while (1)
    {
        handle_signal();
        input = readline("Minishell$ ");
        if (!input)
            break ;
        if (!strlen(input))
            continue;
        add_history(input);
        tokens = tokenize(input);
        expand(*tokens);
        parsed = analyse_tokens(tokens);
        queue = generate_postfix(parsed);
        ast = generate_ast_from_postfix(queue);
        postorder_execution(ast, box);
    }
    return (0);
}
