#include "minishell.h"

// void push_back_stack(t_stack **src, t_stack **dest)
// {
// 	t_stack *node;
// 	t_stack *tmp;

//     if (!src || !*src)
//         return;
//     node = pop_stack(src);
//     if (!node)
//         return;
//     if (*dest == NULL)
//         *dest = node;
//     else
//     {
//         tmp = *dest;
//         while (tmp->next)
//             tmp = tmp->next;
//         tmp->next = node;
//     }
// }

// t_stack *create_node(Token *token)
// {
//     t_stack *node;

//     node = malloc(sizeof(t_stack));
//     if(!node)
//         return (NULL);
//     node->node = token;
//     node->next = NULL;
//     return (node);
// }
// void enqueue(t_queue **queue, Token *token)
// {
//     t_queue *new_node;
//     t_queue *temp;

//     // Allocate memory for the new node
//     new_node = malloc(sizeof(t_queue));
//     new_node->node = token;
//     new_node->next = NULL;

//     // If queue is empty, add the new node as the first element
//     if (*queue == NULL)
//     {
//         *queue = new_node;
//     }
//     else
//     {
//         // Traverse to the end of the queue and add the new node
//         temp = *queue;
//         while (temp->next != NULL)
//             temp = temp->next;
//         temp->next = new_node;
//     }
// }

// void push_to_stack(t_stack **src, t_stack **dest)
// {
// 	t_stack *head;

// 	head = pop_stack(src);
// 	head->next = *dest;
//     *dest = head;
// }
// int get_precedence(int type)
// {
//     if (type == TOKEN_OPEN_PARENTH || type == TOKEN_CLOSE_PARENTH)
//         return (4);
//     if (type == TOKEN_DOUBLE_AMP || type == TOKEN_DOUBLE_PIPE)
//         return (3);
//     if (type == TOKEN_PIPE)
//         return (2);  // Pipe should have lower precedence than redirection
//     if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE_DOC 
//         || type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_IN)
//         return (1);  // Redirection operators should have higher precedence
//     return (0);
// }

// int check_precedence(t_stack *stack, int token_type)
// {
// 	while (stack)
// 	{

// 		if (get_precedence(stack->node->type) >= get_precedence(token_type))
// 			return (0);
// 		stack = stack->next;
// 	}
// 	return (1);
// }

// t_queue *generate_postfix(Token *tokens)
// {
//     t_stack *head;
//     t_queue *output_queue;
//     t_stack *operator_stack;
//     t_stack *popped;

//     head = NULL;
//     output_queue = NULL;
//     operator_stack = NULL;
//     transfer_tokens_to_stack(tokens, &head);
//     while (head)
//     {
//         if (is_operand(head->node))
//         {
//             enqueue(&output_queue, head->node);
//             head = head->next;
//         }
//         else
//         {
//             if (head->node->type == TOKEN_CLOSE_PARENTH)
//             {
//                 while (operator_stack && operator_stack->node->type != TOKEN_OPEN_PARENTH)
//                 {
//                     popped = pop_stack(&operator_stack);
//                     enqueue(&output_queue, popped->node);
//                 }
//                 pop_stack(&operator_stack);
//                 head = head->next;
//             }
//             else if (operator_stack)
//             {
//                 while (!check_precedence(operator_stack, head->node->type))
//                 {
//                     popped = pop_stack(&operator_stack);
//                     enqueue(&output_queue, popped->node);
//                 }
//                 push_to_stack(&head, &operator_stack);
//             }
//             else
//                 push_to_stack(&head, &operator_stack);
//         }
//     }
//     while (operator_stack)
//     {
//         popped = pop_stack(&operator_stack);
//         enqueue(&output_queue, popped->node);
//     }
//     return output_queue;
// }





















//////////////////////////////////////
// void push_back_stack(t_stack **src, t_stack **dest)
// {
// 	t_stack *node;
// 	t_stack *tmp;

//     if (!src || !*src)
//         return;
//     node = pop_stack(src);
//     if (!node)
//         return;
//     if (*dest == NULL)
//         *dest = node;
//     else
//     {
//         tmp = *dest;
//         while (tmp->next)
//             tmp = tmp->next;
//         tmp->next = node;
//     }
// }

// void push_top_stack(t_stack **src, t_stack **dest)
// {
//     if (!src || !*src)
//         return;

//     t_stack *node = pop_stack(src);
//     if (!node)
//         return;
//     node->next = (*dest); 
//     (*dest) = node;
// }

// void par_push(t_stack **head, t_stack **postfix_output, t_stack **holding_stack)
// {
//     t_stack *trash = NULL;
//     push_top_stack(head, &trash);

//     while ((*holding_stack) && (*holding_stack)->node->type != TOKEN_OPEN_PARENTH)
//     {
//         push_back_stack(holding_stack, postfix_output);
//     }

//     if (*holding_stack && (*holding_stack)->node->type == TOKEN_OPEN_PARENTH)
//         push_top_stack(holding_stack, &trash); 
// }

// void tok_push(t_stack **head, t_stack **output_postfix, t_stack **holding_stack)
// {
// 	if (!holding_stack)
// 		push_top_stack(head, holding_stack);
// 	else
// 	{
// 		while ((*holding_stack) && (!check_precedence(*holding_stack, (*head)->node->type)))
//     	{
//         	push_back_stack(holding_stack, output_postfix);
//     	}
//     	push_top_stack(head, holding_stack);
// 	}
// }

// void print_stack(t_stack *head)
// {
// 	while (head)
// 	{
// 		printf(" %s ", head->node->value);
// 		head = head->next;
// 	}
// 	printf("\n");
// }

// int lst_size(t_stack *stack)
// {
// 	int size;

// 	size = 0;
// 	while (stack)
// 	{
// 		size++;
// 		stack = stack->next;
// 	}
// 	return (size);
// }

// t_stack *generate_postfix(Token *tokens)
// {
//     t_stack *postfix_output;
//     t_stack *holding_stack;
//     t_stack *head;

// 	postfix_output = NULL;
// 	holding_stack = NULL;
// 	head = NULL;
//     transfer_tokens_to_stack(tokens, &head);
//     while (head)
//     {
// 		// if (lst_size(postfix_output) == 2 && lst_size(holding_stack) == 1)
// 		// 	push_back_stack(&holding_stack, &postfix_output);
// 		if (head->node->type == TOKEN_COMMAND || head->node->type == TOKEN_UNKNOWN ||
// 			 head->node->type == TOKEN_OPTION || head->node->type == TOKEN_ARGUMENT)
//         	push_back_stack(&head, &postfix_output);
// 		else if (head->node->type != TOKEN_CLOSE_PARENTH && head->node->type != TOKEN_OPEN_PARENTH)
// 		   	tok_push(&head, &postfix_output, &holding_stack);
//         else if (head->node->type == TOKEN_CLOSE_PARENTH)
//             push_top_stack(&head, &holding_stack);
//         else if (head->node->type == TOKEN_OPEN_PARENTH)
//             par_push(&head, &postfix_output, &holding_stack);
//     }
//     while (holding_stack)
// 	{
//         push_back_stack(&holding_stack, &postfix_output);
// 	}
// 	return postfix_output;
// }

