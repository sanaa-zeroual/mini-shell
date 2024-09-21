#include "minishell.h"

void push_back(t_parser **lst, t_parser *node)
{
    t_parser *tmp;

    if (!*lst)
        *lst = node;
    else
    {
        tmp = *lst;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    }
}
int nbr_argument(Token *tokens)
{
    int nbr;

    nbr = 0;
    while(tokens && !is_operator(tokens))
    {
        nbr++;
        tokens = tokens->next;
    }
    return (nbr);
}

void create_node_arguments(t_parser **node, Token **tokens)
{
    int i;
    int num_args;

    i = 0;
    num_args = nbr_argument(*tokens);
    (*node)->arguments = malloc((num_args + 1) * sizeof(Token *));
    if (!(*node)->arguments)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    (*node)->arguments[0] = *tokens;
    (*tokens) = (*tokens)->next;
    i++;
    while (*tokens && !is_operator(*tokens) &&
           (*tokens)->type != TOKEN_COMMAND && (*tokens)->type != TOKEN_BUILT_IN)
           {
        (*node)->arguments[i] = *tokens;
        (*tokens) = (*tokens)->next;
        i++;
    }
    (*node)->arguments[i] = NULL;
}

t_parser *analyse_tokens(Token **tokens)
{
    t_parser *new;
    t_parser *node;

    new = NULL;
    while (*tokens)
    {
        node = malloc(sizeof(t_parser));
        if (!node)
        {
            printf("allocation failed\n");
            exit(1);
        }
        node->token = malloc(sizeof(Token));
        node->token = (*tokens);
        node->next = NULL;
        node->arguments = NULL;
        if ((*tokens)->type == TOKEN_COMMAND || (*tokens)->type == TOKEN_BUILT_IN)
            create_node_arguments(&node, tokens);
        else
            (*tokens) = (*tokens)->next;
        push_back(&new, node);
    }
    free(tokens);
    return new;
}
