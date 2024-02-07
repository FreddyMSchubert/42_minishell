/*
	@brief	Receives array of tokens, inserts token at index
	@brief	Mode 0: replace index; Mode 1: insert before index;
	@note	e.g. iita({"a", "b", "c"}, "y", 1) -> {"a", "y", "b", "c"}
	@note	Assumes that index is in valid range
*/
t_token **insert_into_token_array(t_token **toks, t_token *ins_tok, int index, char mode)
{
	int		counter;
	int		new_length;
	t_token	**new_toks;

	counter = 0;
	while (toks[counter])
		counter++;
	new_length = counter + mode;
	new_toks = realloc(toks, sizeof(t_token *) * (new_length + 1));
	if (!new_toks)
	{
		free(ins_tok->value);
		free(ins_tok);
		counter = 0;
		while (toks[counter])
		{
			free(toks[counter]->value);
			free(toks[counter]);
			counter++;
		}
		free(toks);
		return (NULL);
	}
	new_toks[new_length] = NULL;
	counter += mode;
	while (counter > index && mode == 1)
	{
		new_toks[counter] = new_toks[counter - 1];
		counter--;
	}
	if (mode == 0)
	{
		free(new_toks[index]->value);
		free(new_toks[index]);
	}
	new_toks[index] = ins_tok;
	return (new_toks);
}

/*
	@brief	Receives an array of tokens, splits up tokens that are not separated by spaces
	@note	For example, "ls" "*;" -> "ls", "*", ";"
*/
char	**split_tokens_without_space_seperation(char **split_input)
{
	// the following can be on other arguments without space in between
	// ' " < << >> > | && || ;
	int		counter;

	counter = 0;
	while (split_input[counter])
	{
		if (ft_strchr(split_input[counter], '\''))
			// idk do something split split
		else if (ft_strchr(split_input[counter], '\"'))
			// idk do something split split
		else if (ft_strchr(split_input[counter], '<') || \
				ft_strchr(split_input[counter], '>'))
			// idk do something split split
			// need to handle << and >> as well
		else if (ft_strchr(split_input[counter], '|'))
			// idk do something split split
		else if (ft_strncmp(split_input[counter], "&&", 2) == 0 || \
				ft_strncmp(split_input[counter], "||", 2) == 0)
			// idk do something split split
		else if (ft_strchr(split_input[counter], ';'))
			// idk do something split split
		counter++;
	}