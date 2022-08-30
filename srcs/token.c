/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 12:40:11 by cgaillag          #+#    #+#             */
/*   Updated: 2022/08/30 16:06:27 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstdelone_tok(t_token *node)
{
	if (!node)
		return ;
	free(node->token);
	node->token = NULL;
	node->next = NULL;
	free(node);
}

void	ft_free_tok(t_token **tok)
{
	t_token	*tmp;

	if (!*tok)
		return ;
	while (*tok != NULL)
	{
		tmp = (*tok)->next;
		ft_lstdelone_tok(*tok);
		(*tok) = tmp;
	}
	(*tok) = NULL;
}

t_token	*ft_lstlast_tok(t_token *lst)
{
	t_token	*node;

	if (!lst)
		return (0);
	node = lst;
	while (node->next)
		node = node->next;
	return (node);
}

int	ft_lstadd_token(t_token **tok, int type, char *token)
{
	t_token	*new;
	t_token	*last;

	new = malloc(sizeof(t_token));
	if (!new)
	{
		ft_free_tok(tok);
		return (1);
	}
	new->token = token;
	dprintf(2, "new token %s\n", new->token);
	new->type = type;
	dprintf(2, "type %d\n", new->type);
	new->next = NULL;
	if (ft_lstlast_tok(*tok) == 0)
	{
		*tok = new;
		return (0);
	}
	last = ft_lstlast_tok(*tok);
	last->next = new;
	return (0);
}

/*
// cf. val enum s_type
*/

int	ft_is_redir(char *clean_cmd, int *i)
{
	if (clean_cmd[*i] == '<')
	{
		(*i)++;
		if (clean_cmd[*i] && clean_cmd[*i] == '<')
		{
			(*i)++;
			return (13);
		}
		else
			return (12);
	}
	else if (clean_cmd[*i] == '>')
	{
		(*i)++;
		if (clean_cmd[*i] && clean_cmd[*i] == '>')
		{
			(*i)++;
			return (11);
		}
		else
			return (10);
	}
	return (0);
}

int	ft_check_built_in(char *clean_cmd, t_data *data, int i)
{
	int	len;
	int	j;
	char **built_in;

	len = 0;
	j = 0;
	built_in = data->built_in;
	while (built_in[j])
	{
		len = ft_strlen(built_in[j]);
		if (len == i)
		{
			if (ft_strncmp(clean_cmd, built_in[j], len) == 0)
				return (1);
		}
		j++;
	}
	return (0);
}

t_token	*ft_get_token(char *clean_cmd, t_data *data)
{
	t_token	*token;
	int		type;
	int		i;
	int		j;

	i = 0;
	j = 0;
	token = NULL;
	while(clean_cmd[j])
	{
		if (j == 0)
		{
			type = ft_is_redir(clean_cmd, &i);
			if (type > 0)
			{
				if (ft_lstadd_token(&token, type, ft_substr(clean_cmd, j, i - j)))
					return (NULL); //free tout ce qu'il y a à free
				j = i;
				while (clean_cmd[i] && clean_cmd[i] != ' ' && clean_cmd[i] != '>'  && clean_cmd[i] != '<')
					i++;
				if (ft_lstadd_token(&token, type + 10, ft_substr(clean_cmd, j, i - j)))
					return (NULL); //free tout ce qu'il y a à free
				j = i - 1;
				j--;
			}
			else
			{
				while (clean_cmd[i] && clean_cmd[i] != ' ' && clean_cmd[i] != '>'  && clean_cmd[i] != '<')
					i++;
				if (ft_check_built_in(clean_cmd, data, i))
					type = BUILTIN;
				else
					type = COMMAND;
				if (ft_lstadd_token(&token, type, ft_substr(clean_cmd, j, i - j)))
					return (NULL);
				j = i - 1;
			}
		}
		j++;
	}
	return (0);
}

int	ft_len_no_redir(char *clean_cmd)
{
	int	len;
	int	i;

	i = 0;
	len = ft_strlen(clean_cmd);
	while(clean_cmd[i])
	{
		if (clean_cmd[i] == '>' || clean_cmd[i] == '<')
		{
			len--;
			i++;
			if ((clean_cmd[i] == '>' || clean_cmd[i] == '<'))
			{
				len--;
				i++;
			}
			while (clean_cmd[i] && clean_cmd[i] != ' ' && clean_cmd[i] != '>' && clean_cmd[i] != '<')
			{
				len--;
				i++;
			}
			if (clean_cmd[i] == ' ')
				len--;
			i--;
		}
		i++;
	}
	return (len);
}

int	ft_get_redir_list(char *clean_cmd, t_token *tok_redir)
{
	int	i;
	int	j;
	int	type;

	i = 0;
	j = 0;
	type = 0;
	while(clean_cmd[j])
	{
		type = ft_is_redir(clean_cmd, &i);
		if (type)
		{
			if (ft_lstadd_token(&tok_redir, type, ft_substr(clean_cmd, j, (i - j))))
				return (1); //free tout ce qu'il y a à free
			j = i;
			while (clean_cmd[i] && clean_cmd[i] != ' ' && clean_cmd[i] != '>'  && clean_cmd[i] != '<')
				i++;
			if (ft_lstadd_token(&tok_redir, type + 10, ft_substr(clean_cmd, j, i - j)))
				return (1); //free tout ce qu'il y a à free
			j = i - 1;
		}
		j++;
	}
	return (0);
}

void	ft_fill_no_redir(char *clean_cmd, char *clean_cmd_no_redir)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while(clean_cmd[j])
	{
		if (clean_cmd[j] == '>' || clean_cmd[j] == '<')
		{
			j++;
			if ((clean_cmd[j] == '>' || clean_cmd[j] == '<'))
				j++;
			while (clean_cmd[j] && clean_cmd[j] != ' ' && clean_cmd[j] != '>' && clean_cmd[j] != '<')
				j++;
			if (clean_cmd[j] != ' ')
				j--;
		}
		else
		{
			clean_cmd_no_redir[i] = clean_cmd[j];
			i++;
		}
		j++;
	}
	clean_cmd_no_redir[i] = '\0';
}

int	ft_get_redir(t_cmd	*cmd)
{
	char	*clean_cmd;
	char	*clean_cmd_no_redir;
	t_token *tok_redir;

	tok_redir = NULL;
	clean_cmd = cmd->clean_cmd;
	clean_cmd_no_redir = malloc(sizeof(char) * (ft_len_no_redir(clean_cmd) + 1));
	if (!clean_cmd_no_redir)
		return (1); // bien free tout ce qu'il y a à free
	if (ft_get_redir_list(clean_cmd, tok_redir))
		return (1);// FREE
	cmd->tok_redir = tok_redir;
	cmd->clean_cmd_no_redir = clean_cmd_no_redir;
	ft_fill_no_redir(clean_cmd, clean_cmd_no_redir);
	return (0);
}

int	ft_tokenizer(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd;
	while (cmd)
	{
		ft_get_redir(cmd);
		printf("clean cmd no redir = %s\n", cmd->clean_cmd_no_redir);
	//	ft_get_token(cmd->clean_cmd_no_redir, data);
		cmd = cmd->next;
	}
	return (0);
}

