#include "../minishell.h"

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
    int	i;

    nbr = 0;
    while (tokens && !is_operator(tokens))
    {
    	i = 0;
        while (tokens->expanded_value[i])
        {
            i++;
            nbr++;
        }
        tokens = tokens->next;
    }
    return (nbr);
}

void create_node_arguments(t_parser **node, Token **tokens)
{
    int i;
	int j;
    int num_args;

    i = 0;
    num_args = nbr_argument(*tokens);
    (*node)->arguments = malloc((num_args + 1) * sizeof(Token *));
    if (!(*node)->arguments)
    {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    (*node)->arguments[0] = (*tokens)->expanded_value[0];
    (*tokens) = (*tokens)->next;
    i++;
    while (*tokens && !is_operator(*tokens) &&
           (*tokens)->type != TOKEN_COMMAND && (*tokens)->type != TOKEN_BUILT_IN)
           {
				j = 0;
				while ((*tokens)->expanded_value[j])
				{
					(*node)->arguments[i] = (*tokens)->expanded_value[j];
					i++;
					j++;
				}
				(*tokens) = (*tokens)->next;
    		}
    (*node)->arguments[i] = NULL;
}
void free_tokens(Token *tokens)
{

    Token *tmp;
    while (tokens)
    {
        tmp = tokens;
        tokens = tokens->next;
        
        // Free the `value` (assuming it's dynamically allocated)
        if (tmp->value)
            free(tmp->value);

        // Free the `expanded_value` array (if not NULL)
        if (tmp->expanded_value)
        {
            for (int i = 0; tmp->expanded_value[i]; i++)
            {
                free(tmp->expanded_value[i]);  // Free each string in the array
            }
            free(tmp->expanded_value);  // Free the array itself
        }

        // Free the token itself
        free(tmp);
    }
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
		else if (!is_operator(*tokens))
            create_node_arguments(&node, tokens);
		else
            (*tokens) = (*tokens)->next;
        push_back(&new, node);
    }
    // free_tokens(*tokens);
    return new;
}
