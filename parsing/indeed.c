// int check_precedence(t_stack *stack, int token_type)
// {
// 	while (stack)
// 	{
// 		if (get_precedence(stack->node->type) > get_precedence(token_type))
// 			return (0);
// 		stack = stack->next;
// 	}
// 	return (1);
// }

// t_ast *pop_stack(t_stack **stack)
// {
//     if (!stack || !*stack)
//         return NULL;

//     t_stack *top = *stack;
//     t_ast *node = top->node;
//     *stack = top->next;
//     free(top);
//     return node;
// }

// t_ast *create_ast_node(Token *token)
// {
//     t_ast *new_node = malloc(sizeof(t_ast));
//     if (!new_node)
//         return (NULL);
//     new_node->node = token;
//     new_node->node->left = NULL;
//     new_node->node->right = NULL;
//     return new_node;
// }


// t_queue *generate_postfix(Token *tokens)
// {
//     t_queue *output_queue = NULL;
//     t_stack *holding_stack = NULL;
    
//     while (tokens)
//     {
//         if (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_UNKNOWN ||
//             tokens->type == TOKEN_OPTION || tokens->type == TOKEN_ARGUMENT)
//         {
//             output_queue = push_queue(output_queue, tokens);
//         }
//         else
//         {
//             while (holding_stack && get_precedence(holding_stack->node->type) >= get_precedence(tokens->type))
//             {
//                 output_queue = push_queue(output_queue, holding_stack->node);
//                 holding_stack = holding_stack->next;
//             }
//             holding_stack = push_stack(holding_stack, tokens);
//         }
//         tokens = tokens->next;
//     }
//     while (holding_stack)
//     {
//         output_queue = push_queue(output_queue, holding_stack->node);
//         holding_stack = holding_stack->next;
//     }
//     return (output_queue);
// }
// t_stack *push_ast_stack(t_stack *stack, t_ast *node)
// {
// 	t_stack *head;

// 	head = malloc(sizeof(t_stack));
// 	if (!head)
// 		return (NULL);
// 	head->node = node;
// 	if (!stack)
// 		return (head);
// 	head->next = stack;
// 	stack = head;
// 	return (stack);
// }
// //push_at_the_end

// t_ast *push_to_ast_stack(t_ast *ast_stack, t_ast *ast_node)
// {
//     if (!ast_stack)
//         return (ast_node);
// 	ast_node->next = ast_stack; 
// 	ast_stack = ast_node;
// 	return (ast_stack);
// }
// t_ast *pop_ast_stack(t_ast **ast_stack)
// {
//     t_ast *head;
//     t_ast *next;

//     head = *ast_stack;
//     next = (*ast_stack)->next;
//     (*ast_stack) = next;
//     return (head);
// }


// t_ast *generate_ast_from_postfix(t_queue *postfix_queue)
// {
//     t_ast *ast_stack = NULL;
    
//     while (postfix_queue)
//     {
//         if (is_operand(postfix_queue->node))
//         {
//             t_ast *new_node = create_ast_node(postfix_queue->node);
//             ast_stack = push_to_ast_stack(ast_stack, new_node);
//         }
//         else if (is_operator(postfix_queue->node))
//         {
//             t_ast *right = pop_ast_stack(&ast_stack);
//             t_ast *left = pop_ast_stack(&ast_stack);
//             t_ast *new_node = create_ast_node(postfix_queue->node);
//             new_node->node->left = left;
//             new_node->node->right = right;
//             ast_stack = push_to_ast_stack(ast_stack, new_node);
//         }
//         postfix_queue = postfix_queue->next;
//     }
//     return pop_ast_stack(&ast_stack);
// }


// int	main(void)
// {
// 	char	*input;
// 	Token	*tmp;
// 	Token	**tokens;

// 	tokens = NULL;
// 	while (1)
// 	{
// 		input = readline("Minishell$ ");
// 		tokens = tokenize(input);
// 		tmp = *tokens;
// 		while(tmp)
// 		{
// 			printf("token_value  = %s token_type = %d\n", tmp->value, tmp->type);
// 			tmp = tmp->next;
// 		}
// 		ft_parser (*tokens);
// 	}
//     generate_Ast(*tokens);
// 	// print_ast(ast);
// 	return (0);
// }


// typedef struct ASTNode
// {
// 	TokenType		type;
// 	char			*value;
// 	int				fd_in;
// 	int				fd_out;
// 	int				arg_nbr;
// 	struct ASTNode	*left;
// 	struct ASTNode	*right;
// }					ASTNode;

// typedef struct s_ast
// {
// 	ASTNode			*node;   // Only one node pointer, which points to ASTNode
// 	struct s_ast	*left;
// 	struct s_ast	*right;
// 	struct s_ast	*next;   // Added next for stack manipulation
// }					t_ast;

// typedef struct queue
// {
// 	Token			*node;
// 	struct queue	*next;
// }					t_queue;

// typedef struct stack
// {
// 	t_ast			*node;   // Changed to t_ast pointer
// 	struct stack	*next;
// }					t_stack;

// Push to AST stack (for postfix to AST generation)


/////

// queue => FIFO
// stack => LIFO

// typedef struct ASTNode
// {
// 	TokenType		type;
// 	char			*value;
// 	int				fd_in;
// 	int				fd_out;
// 	int				arg_nbr;
// 	struct ASTNode	*left;
// 	struct ASTNode	*right;
// }					ASTNode;


// typedef struct s_ast
// {
// 	ASTNode			*node;
// 	Token			*node;
// 	struct s_ast	*left;
// 	struct s_ast	*right;
// }					t_ast;

// typedef struct queue
// {
// 	Token *node;
// 	struct queue *next;
// }t_queue;

// typedef struct stack
// {
// 	Token *node;
// 	struct stack *next;
// }t_stack;


// t_stack *push_stack(t_stack *stack, Token *node)
// {
// 	t_stack *head;

// 	head = malloc(sizeof(t_stack));
// 	if (!head)
// 		return (NULL);
// 	head->node = node;
// 	if (!stack)
// 		return (head);
// 	head->next = stack;
// 	stack = head;
// 	return (stack);
// }


// t_queue *generate_postfix(Token *tokens)
// {
//     t_queue *output_queue = NULL;
//     t_stack *holding_stack = NULL;

//     while (tokens)
//     {
//         if (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_UNKNOWN ||
//             tokens->type == TOKEN_OPTION || tokens->type == TOKEN_ARGUMENT)
//         {
//             output_queue = push_queue(output_queue, tokens);
//         }
//         else
//         {
//             while (holding_stack && get_precedence(holding_stack->node->node->type) >= get_precedence(tokens->type))
//             {
//                 output_queue = push_queue(output_queue, holding_stack->node->node);
//                 holding_stack = holding_stack->next;
//             }
//             holding_stack = push_stack(holding_stack, create_ast_node(tokens));
//         }
//         tokens = tokens->next;
//     }
//     while (holding_stack)
//     {
//         output_queue = push_queue(output_queue, holding_stack->node->node);
//         holding_stack = holding_stack->next;
//     }
//     return (output_queue);
// }

//////////////

// void tok_push(t_stack **head, t_stack **output_postfix, t_stack **holding_stack)
// {
// 	if (!holding_stack)
// 		push_top_stack(head, holding_stack);
// 	else 
// 	{
// 		while ((*holding_stack) && 
// 			((get_precedence((*holding_stack)->node->type) > get_precedence((*head)->node->type)) ||
// 				(get_precedence((*holding_stack)->node->type) == get_precedence((*head)->node->type) &&
// 				(*holding_stack)->node->type == TOKEN_PIPE)))
//     	{
//         	push_back_stack(holding_stack, output_postfix);
//     	}
//     	push_top_stack(head, holding_stack);
// 	}
// }

//////////////
#include "main.c"

t_queue *generate_postfix(Token *tokens)
{
    t_queue *output_queue ;
    t_stack *holding_stack ;
    t_stack *tmp ;
    int i = 0;

    output_queue = NULL;
    holding_stack = NULL;
    tmp = NULL;
    while (tokens)
    {
        if (tokens->type == TOKEN_COMMAND || tokens->type == TOKEN_UNKNOWN ||
            tokens->type == TOKEN_OPTION || tokens->type == TOKEN_ARGUMENT)
            output_queue = push_queue(output_queue, tokens);
        else
        {
            if (!holding_stack)
                holding_stack = push_stack(holding_stack, tokens);
            else
            {
                while (holding_stack && !check_precedence(holding_stack, tokens->type))
                {
                    tmp = pop_stack(&holding_stack);
                    output_queue = push_queue(output_queue, tmp->node);
                    holding_stack = holding_stack->next;
                }
                holding_stack = push_stack(holding_stack, tokens);
            }
        }
        tokens = tokens->next;
    }
    while (holding_stack)
    {
        output_queue = push_queue(output_queue, holding_stack->node);
        holding_stack = holding_stack->next;
    }
    return (output_queue);
}























