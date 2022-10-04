/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 15:47:25 by lmelard           #+#    #+#             */
/*   Updated: 2022/09/26 02:45:41 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_only_space(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && ft_isdigit(line[i + 1]))
			i++;
		else if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

static void ft_init_data_0(t_data *data)
{
	data->line = NULL;
	data->prompt = NULL;
	data->env = NULL;
	data->cwd = NULL;
	data->val_exit = 0;
	data->str_exit = NULL;
	data->nb_pipes = -1;
	data->cmd = NULL;
	data->built_in = NULL;
	data->pid = NULL;
	data->pipe_fd = NULL;
	data->env_path = NULL;
	data->s_env_path = NULL;
	data->bin = NULL;
	//TO CHECK : que toutes les variables de t_data sont init_0
}

int	ft_init_data_1(t_data *data, char **envp)
{
	ft_init_data_0(data);
	data->env = ft_get_env(data, envp);
	data->prompt = ft_strdup_malloc(data, "minishell> ");
	if (data->prompt == NULL)
	// {
	// 	ft_free_env(&(data->env));
	// 	free(data->prompt);
	// 	return (1);//on peut mettre un message de pb de malloc car strdup/ could not initialise minishell
	// }
		return (ft_msg(1, "prompt memory alloc failure: ", "", "could not initialise minishell" ));
	data->built_in = ft_built_in(data);
	// //if malloc failed --> already taken into account dans ft_built_in() : exit
	// {
	// 	ft_free_env(&(data->env));
	// 	free(data->prompt);
	// 	ft_free_tabstr(data->built_in);
	// 	return (1);//on peut mettre un message de pb de malloc car strdup/ could not initialise minishell
	// }
	data->cwd = getcwd(NULL, 0);// sera mise à jour dans cd
	if (!data->cwd)
	// {
	// 	ft_free_env(&(data->env));
	// 	free(data->prompt);
	// 	ft_free_tabstr(data->built_in);
	// 	return (1);//could not initialise minishell
	// }
	{
		ft_lstclear_bin(data->bin);//free bin !!
		return (ft_msg(1, "cwd memory alloc failure: ", "", "could not initialise minishell" ));
	}
	ft_get_env_path(data, envp);// à commenter pour export - PATH
	//ft_get_env_path(data);// à activer pour export - PATH
	return (0);
}

/*	Clean data->cmd
*/
int	ft_clean_cmdline(t_data *data)
{
	if (data != NULL)
	{
		if (data->line)
		{
			free(data->line);
			data->line = NULL;
		}
		if (data->str_exit)
		{
			free(data->str_exit);
			data->str_exit = NULL;
		}
	}
	return (0);
}

int	ft_clean_loop(t_data *data)
{
	if (data->built_in != NULL)
		ft_free_tabstr(data->built_in);
	if (data->cwd != NULL)
		free(data->cwd);
	free(data->prompt);
	if (data->env != NULL)
		ft_free_env(&(data->env));
	if (data->env_path)
		ft_free_env(&(data->env_path));
	return (0);
}

void	ft_minishell(t_data *data)
{
	if (ft_lexer(data->line))
	{//dprintf(2, "ft_lexer = %d\n", ft_lexer(data.line));
		data->val_exit = ft_msg(2, "", "", ERRSTX);
	}
	else
	{
		ft_parser(data);// peut etre faire un if == 0 ou 1
		ft_exec(data);// peut etre faire un if == 0 ou 1
		ft_free_cmd(&(data->cmd));// A AJUSTER
	}
	ft_clean_cmdline(data);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc == 1 && !argv[1])
	{
		if (ft_init_data_1(&data, envp))
			return (1);
		while (1)
		{
			data.line = readline(data.prompt);
			if (data.line && ft_strlen(data.line) != 0 && ft_only_space(data.line) == 1)
			{
				add_history(data.line);
				ft_minishell(&data);
			}
		}
		rl_clear_history();
		ft_clean_loop(&data);
		return (0);
	}
	// penser à faire un message d'erreur si nb d'arg incorrect
	return (1);
}