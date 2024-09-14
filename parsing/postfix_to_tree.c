#include "minishell.h"

t_ast *create_ast_node(Token *token)
{
    t_ast *new_node;
    
    new_node = malloc(sizeof(t_ast));
    if (!new_node)
        return NULL;
    new_node->node = malloc(sizeof(ASTNode));
    if (!new_node->node)
    {
        free(new_node);
        return NULL;
    }
    new_node->node->token = malloc(sizeof(Token));
    if (!new_node->node->token)
    {
        free(new_node->node);
        free(new_node);
        return NULL;
    }
    new_node->node->token->value = strdup(token->value);
    new_node->node->token->type = token->type;
    new_node->node->left = NULL;
    new_node->node->right = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->next = NULL;
    return (new_node);
}

AST_TYPE get_ast_type(TokenType type)
{
    if (type == TOKEN_PIPE)
        return PIPELINE;
    if (type == TOKEN_REDIR_IN)
        return REDERECTION_IN;
    if (type == TOKEN_REDIR_OUT)
        return REDERECTION_OUT;
    if (type == TOKEN_REDIR_HERE_DOC)
        return REDERECTION_HEREDOC;
    if (type == TOKEN_REDIR_APPEND)
        return REDERECTION_APPEND;
    return COMMAND;   
}

t_ast *generate_ast_from_postfix(t_queue *postfix_output)
{
    t_ast   *ast_stack;
    t_ast   *ast_node;
    t_ast   *left_node;
    t_ast   *right_node;

    ast_stack = NULL;
    ast_node = NULL;
    left_node = NULL;
    right_node = NULL;
    while (postfix_output)
    {
        if (is_operand(postfix_output->node))
        {
            ast_node = create_ast_node(postfix_output->node);
            ast_stack = push_to_ast_stack(ast_stack, ast_node);
        }
        else if (is_operator(postfix_output->node))
        {
            right_node = pop_ast_stack(&ast_stack);
            left_node = pop_ast_stack(&ast_stack);
            ast_node = create_ast_node(postfix_output->node);
            ast_node->type = get_ast_type(postfix_output->node->type);
            ast_node->left = left_node;
            ast_node->right = right_node;
            ast_stack = push_to_ast_stack(ast_stack, ast_node);
        }
        postfix_output = postfix_output->next;
    }
    return (ast_stack);
}