#include "minishell.h"

char	*adapt_quoted_str(char *str)
{
	char				*new_str;
	int				a;
	int				b;

	a = 0;
	b = sizeof_str(str, '\0');
	if (str && str[0] == '"' && str[b - 1] == '"')
	{
		a += 1;
		b -= 1;
	}
	new_str = malloc((b - a) + 1);
	s_strcopy(new_str, str, a, b);
	free(str);
	return (new_str);
}

char	**merge_it(char **f_args, char **_cmd_)
{
	int				a;
	char				**new_args;

	if (!f_args)
		return (NULL);
	a = 1;
	while (_cmd_[a])
		a++;
	new_args = malloc((a + 1) * sizeof(char *));
	a = 0;
	new_args[a] = adapt_quoted_str(strcopy(f_args[a]));
	while (_cmd_[++a])
			new_args[a] = adapt_quoted_str(strcopy(_cmd_[a]));
	new_args[a] = 0;
	free_multible(f_args);
	return (new_args);
}

char	*ex_new_line(char *line, int a, int b)
{
	char				*new_line;

	new_line = malloc(sizeof_str(line, '\0') + 1);
	while (line[a])
	{
		if (line[a] != 34 && line[a] != 39)
			new_line[b++] = line[a];
		a++;
	}
	free(line);
	new_line[b] = '\0';
	return (new_line);
}

int	_statment_caution(char *line)
{
	int				a;
	int				b[2];
	int				res;

	a = 0;
	res = 0;
	b[0] = 0;
	b[1] = 0;
	while (line[a])
	{
		if (line[a] == 34 || line[a] == 39)
		{
			if (line[a] == 34)
				b[0] += 1;
			else
				b[1] += 1;
			res += 1;
		}
		a++;
	}
	if (res && !(b[0] % 2) && !(b[1] % 2))
		return (1);
	return (0);
}

char	*echo_new_line(char *line, int a, int b, s_en *env)
{
	char				*new_line;
	int					c;

	line = _catch_var(line, env);
	new_line = malloc(sizeof_str(line, '\0') + 3);
	c = sizeof_str(line, ' ');
	while (line[a])
	{
		if (a == c + 1)
			new_line[b++] = '"';
		if (line[a] != 34 && line[a] != 39)
			new_line[b++] = line[a];
		a++;
	}
	free(line);
	new_line[b++] = '"';
	new_line[b] = '\0';
	printf("---> echo_fixed_statment: .%s.\n",  new_line);
	return (new_line);
}

char	*handle_special_misdefinitions(char *line, s_en *env)
{
	int				b;
	char				*new_line;

	b = sizeof_str(line, ' ');
	new_line = malloc(b + 1);
	s_strcopy(new_line, line, 0, b);
	if (str_cmp(new_line, "export", NULL)
		&& _statment_caution(line))
	{
		free(new_line);
		return (ex_new_line(line, 0, 0));
	}
	else if (str_cmp(new_line, "echo", NULL)
		&& _statment_caution(line))
	{
		free(new_line);
		return (echo_new_line(line, 0, 0, env));
	}
	free(new_line);
	return (line);
}
