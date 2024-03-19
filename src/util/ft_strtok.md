/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:53:51 by nburchha          #+#    #+#             */
/*   Updated: 2024/03/16 13:26:37 by nburchha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_strstok_util(char *s, const char *delim)
{
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (s && s[++i] && ft_strchr(delim, s[i]) == NULL)
		;
	if (s[i] && s[i + 1])
	{
		s[i] = '\0';
		return (&s[i + 1]);
	}
	else
		return (NULL);
}

char	*ft_strtok(char *s, const char *delim)
{
	static char	*last;
	char		*tok;
	int			i;
	int			j;

	i = -1;
	if ((!s && !last) || !delim)
		return (NULL);
	if (last)
		s = last;
	while (ft_strchr(delim, s[++i]))
		;
	printf("s: %s\n", &s[i]);
	tok = &s[i];
	last = ft_strstok_util(tok, delim);
	if (tok[0] == '\0')
		return (NULL);
	return (tok);
}

int main(void)
{
	char *str = "   $   echo hallo $USER$USER hallo1 welt";
	char *delims = " $";

	char *tok = ft_strtok(str, delims);
	while (tok != NULL) {
		printf("%s\n", tok);
		tok = ft_strtok(NULL, delims);
	}
	
}