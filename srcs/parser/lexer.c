/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 14:55:03 by cgaillag          #+#    #+#             */
/*   Updated: 2022/08/24 14:55:03 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	<SUMMARY> checks redirections in 2 steps:
**	1. checking of (i + 1) content (only spaces, direct closing quotes)
**	2. checking file
*/
int	ft_check_redir(char *line, int i)
{
	int		j;

	j = i;
	if (line[j] && line[j] == ' ')
	{
		while (line[j] && line[j] == ' ')
			j++;
		if ((line[j] == '\0') || (line[j] == '|') || (line[j] == 60) \
			|| (line[j] == 62))
			return (-1);
	}
	while (line[j] && (line[j] != '|') && (line[j] != 60) && (line[j] != 62) \
		&& (line[j] != ' '))
	{
		j = ft_btw_quotes(line, j);
		j++;
	}
	if (j == i)
		return (-1);
	return (j - 1);
}

/*	<SUMMARY> checks redirections - main function (nb & file/name that follows)
**	ASCII value for infile : 60 and for outfile redir : 62
*/
int	ft_redir(char *line)
{
	int		i;
	char	c;

	i = 0;
	while (line[i])
	{
		if (line[i] == 34 || line[i] == 39)
			i = ft_btw_quotes(line, i);
		else if (line[i] == 60 || line[i] == 62)
		{
			c = line[i];
			i++;
			if (line[i] == c)
				i++;
			i = ft_check_redir(line, i);
			if (i == -1)
				return (1);
		}
		i++;
	}
	return (0);
}

/*	<SUMMARY> Helper function to go through (closing) quotes
*/
int	ft_btw_quotes(char *line, int i)
{
	int		j;
	char	c;

	j = i;
	if (line[j] == 34 || line[j] == 39)
	{
		c = line[j];
		j++;
		while (line[j] && line[j] != c)
			j++;
		return (j);
	}
	return (i);
}

/*	<SUMMARY> checks if any open quote has its matching closing quote 
**	ASCII value for a simple quote : 39 and for a doubble quote : 34
*/
int	ft_quote(char *line)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == 39 || line[i] == 34)
		{
			c = line[i];
			j = i + 1;
			while (line[j] && line[j] != c)
				j++;
			if (!line[j])
				return (1);
			i = j;
		}
		i++;
	}
	return (0);
}

int	ft_lexer(char *line)
{
	int	count;

	count = 0;
	if (!line)
		return (0);
	else
	{
		if (ft_quote(line) == 1)
			return (1);
		count = ft_count_pipe(line);
		if (ft_pipe(line, count) == 1)
			return (2);
		if (ft_redir(line) == 1)
			return (3);
	}
	return (0);
}
