/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shebaz <shebaz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 01:45:43 by szeroual          #+#    #+#             */
/*   Updated: 2024/10/20 03:42:56 by shebaz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>
# include "../../execution/libftt/libft.h"
# include <unistd.h>
# define MAX_FD 1024
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

// size_t	ft_strlen( char *s);
// char	*ft_strchr(char *s, int c);
// char	*ft_strdup( char *s1);
void	ft_strcat(char *dest, char *src);
// char	*ft_strjoin(char *s1, char *s2);
// char	*ft_substr(char *s, unsigned int start, size_t len);
char	*extarct_line(char *str);
char	*get_rest(char *save);
char	*get_next_line(int fd);
// char	*ft_substr(char *s, unsigned int start, size_t len);
char	*readfile(int fd, char *str);

#endif