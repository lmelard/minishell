/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/16 17:30:46 by cgaillag          #+#    #+#             */
/*   Updated: 2022/10/17 05:51:34 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_val_exit;

char	**ft_init_cmd_opt(t_cmd *cmd, t_data *data)
{
	char	**cmd_opt;
	t_token	*token;
	int		i;

	(void) data;
	token = cmd->token;
	i = 0;
	while (token)
	{
		token = token->next;
		i++;
	}
	cmd_opt = ft_handle_malloc(MALLOC_C + TAB_STR2, NULL, (i + 1), data);
	// cmd_opt = malloc(sizeof(char *) * (i + 1));
	// if (!cmd_opt)
	// 	return (NULL);
	i = 0;
	token = cmd->token;
	while (token)
	{
		cmd_opt[i] = ft_strdup(token->token);
		// dprintf(2, "cmd option[%i] = %s\n", i, cmd_opt[i]);
		// cmd_opt[i] = NULL;/// TEST
		// printf("TEST cmd opt [i] = NULL\n");// TEST
		ft_handle_malloc(ADD_C + TAB_STR1, cmd_opt[i], 0, data);
		// if (!cmd_opt[i])
		// {
		// 	ft_free_tabstr(cmd_opt);
		// 	return (NULL);
		// }
		i++;
		token = token->next;
	}
	cmd_opt[i] = NULL;
	return (cmd_opt);
}

int	**ft_init_pipe(t_data *data)
{
	int	**tab_int;
	int	i;

	//dprintf(2, "entre dans init pipe\n");
	tab_int = ft_handle_malloc(MALLOC_M + TAB_INT2, NULL, data->nb_pipes, data);
	// tab_int = (int **) malloc(sizeof(int *) * data->nb_pipes);
	// if (tab_int == NULL)
	// 	return (free(tab_int), NULL);
	i = 0;
	while (i < data->nb_pipes)
	{
		tab_int[i] = ft_handle_malloc(MALLOC_M + TAB_INT1, NULL, 2, data);
		// tab_int[i] = (int *) malloc(sizeof(int) * 2);
		// if (!tab_int[i])
		// {
		// 	ft_free_tabint(tab_int, i - 1);
		// 	break ;
		// }
		tab_int[i][0] = -1;
		tab_int[i][1] = -1;
		i++;
	}
	return (tab_int);
}

int	*ft_init_pid(t_data *data)
{
	int	*pid;
	int	i;

	i = 0;
	//dprintf(2, "entre dans init pid\n");
	pid = ft_handle_malloc(MALLOC_M + TAB_INT1, NULL, (data->nb_pipes + 1), data);
	// pid = malloc(sizeof(int) * (data->nb_pipes + 1));
	// if (!pid)
	// 	return (NULL);//FREE TOUT ET EXIT
	while (i < (data->nb_pipes + 1))
	{
		pid[i] = -1;
		i++;
	}
	return (pid);
}

int	ft_get_files_io(t_data *data)
{
	// int		res;
	t_cmd	*cmd;
	t_token	*tok_redir;

	// res = 0;
	cmd = data->cmd;
	tok_redir = NULL;
	//dprintf(2, "entre dans get files io\n");
	while (cmd)
	{
		tok_redir = cmd->tok_redir;
		while (tok_redir)
		{
			if (tok_redir->type == GREAT)
			{
				tok_redir = tok_redir->next;
				tok_redir->fd = open(tok_redir->token, O_CREAT | O_RDWR | O_TRUNC, 0644);
				if (tok_redir->fd < 0)
				{
					cmd->file_err = 1;
					g_val_exit = ft_msg(errno, tok_redir->token, ": ", strerror(errno)); // exit avec free
				}
				cmd->outfile = tok_redir->fd;
			}
			else if (tok_redir->type == D_GREAT)
			{
				tok_redir = tok_redir->next;
				tok_redir->fd = open(tok_redir->token, O_CREAT | O_RDWR | O_APPEND, 0644);
				if (tok_redir->fd < 0)
				{
					cmd->file_err = 1;
					g_val_exit = ft_msg(errno, tok_redir->token, ": ", strerror(errno)); // exit avec free
				}
				cmd->outfile = tok_redir->fd;
			}
			else if (tok_redir->type == LESS)
			{
				tok_redir = tok_redir->next;
				tok_redir->fd = open(tok_redir->token, O_RDONLY);
				if (tok_redir->fd < 0)
				{
					cmd->file_err = 1;
					g_val_exit = ft_msg(errno, tok_redir->token, ": ", strerror(errno)); // exit avec free
				}
				cmd->infile = tok_redir->fd;
			}
			else if (tok_redir->type == D_LESS)
				ft_heredoc(data, data->cmd, tok_redir);
			tok_redir = tok_redir->next;
		}
	//	dprintf(2, "pour cmd->token = %s\n", cmd->token->token);
	//	dprintf(2, "cmd->infile = %d et cmd->outfile = %d\n", cmd->infile, cmd->outfile);
		cmd = cmd->next;
	}
	return (0);
}

// int	ft_get_files_io(t_data *data)
// {
// 	t_cmd	*cmd;
// 	t_token	*tok_redir;

// 	cmd = data->cmd;
// 	tok_redir = NULL;
// 	//dprintf(2, "entre dans get files io\n");
// 	while (cmd)
// 	{
// 		tok_redir = cmd->tok_redir;
// 		while (tok_redir)
// 		{
// 			if (tok_redir->type == GREAT)
// 			{
// 				tok_redir = tok_redir->next;
// 				tok_redir->fd = open(tok_redir->token, O_CREAT | O_RDWR | O_TRUNC, 0644);
// 				if (tok_redir->fd < 0)
// 				{
// 					cmd->file_err = 1;
// 					//printf("outfile error\n"); // afficher error avec erno + strerror
// 					ft_msg(errno, ERRMSG, "", strerror(errno));
// 				}
// 				cmd->outfile = tok_redir->fd;
// 			}
// 			else if (tok_redir->type == D_GREAT)
// 			{
// 				tok_redir = tok_redir->next;
// 				tok_redir->fd = open(tok_redir->token, O_CREAT | O_RDWR | O_APPEND);
// 				if (tok_redir->fd < 0)
// 				{
// 					cmd->file_err = 1;
// 					//printf("outfile error\n"); // afficher error avec erno + strerror
// 					ft_msg(errno, ERRMSG, "", strerror(errno));
// 				}
// 				cmd->outfile = tok_redir->fd;
// 			}
// 			else if (tok_redir->type == LESS)
// 			{
// 				tok_redir = tok_redir->next;
// 				tok_redir->fd = open(tok_redir->token, O_RDONLY);
// 				if (tok_redir->fd < 0)
// 				{
// 					cmd->file_err = 1;
// 					//printf("infile error\n"); // afficher error avec erno + strerror
// 					ft_msg(errno, ERRMSG, "", strerror(errno));
// 				}
// 				cmd->infile = tok_redir->fd;
// 			}
// 			else if (tok_redir->type == D_LESS)
// 			{
// 				tok_redir = tok_redir->next;
// 				tok_redir->fd = open(tok_redir->token, O_RDONLY);
// 				if (tok_redir->fd < 0)
// 				{
// 					cmd->file_err = 1;
// 					//printf("infile error\n"); // afficher error avec erno + strerror
// 					ft_msg(errno, ERRMSG, "", strerror(errno));
// 				}
// 				cmd->infile = tok_redir->fd;
// 			}
// 			tok_redir = tok_redir->next;
// 		}
// 	//	dprintf(2, "pour cmd->token = %s\n", cmd->token->token);
// 	//	dprintf(2, "cmd->infile = %d et cmd->outfile = %d\n", cmd->infile, cmd->outfile);
// 		cmd = cmd->next;
// 	}
// 	return (0);
// }
