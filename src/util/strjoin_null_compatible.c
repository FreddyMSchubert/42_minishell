#include "../../include/minishell.h"

char	*strjoin_null_compatible(char const *s1, char const *s2)
{
	size_t		s1size;
	size_t		s2size;
	char		*p;

	s1size = 0;
	s2size = 0;
	if (s1)
		s1size = ft_strlen(s1);
	if (s2)
		s2size = ft_strlen(s2);
	p = (char *)malloc(s1size + s2size + 1);
	if (p == NULL)
		return (NULL);
	if (s1)
		ft_memcpy(&p[0], s1, s1size);
	if (s2)
		ft_memcpy(&p[s1size], s2, s2size);
	p[s1size + s2size] = '\0';
	return (p);
}
