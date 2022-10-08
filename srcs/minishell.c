/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 15:47:25 by lmelard           #+#    #+#             */
/*   Updated: 2022/10/08 01:57:44 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_val_exit;

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
	data->oldpwd = NULL;
	data->home = NULL;
	data->val_exit = 0;
	data->str_exit = NULL;
	data->nb_pipes = -1;
	data->cmd = NULL;
	data->built_in = NULL;
	data->pid = NULL;
	data->pipe_fd = NULL;
	data->env_path = NULL;
	data->s_env_path = NULL;
	//TO CHECK : que toutes les variables de t_data sont init_0
}

void	ft_get_home(t_data *data)
{
	(void) data;
	t_env	*env;

	env = data->env;
	if (data->home)
		free(data->home);
	data->home = NULL;
	if (env != NULL)
	{
		while (env)
		{
			if (ft_strncmp(env->var_equal, "HOME=", ft_strlen(env->var_equal)) == 0)
				break ;
			env = env->next;
		}
	}
	if (env == NULL)
	{
		data->home = NULL;
		return ;
	}
	if (env->content)
	{
		data->home = ft_strdup(env->content);
		if (!data->home)
			return ;//malloc free
	}
	return ;
}

int	ft_init_data_1(t_data *data, char **envp)
{
	ft_init_data_0(data);
	data->env = ft_get_env(envp);
	data->prompt = ft_strdup("--minishell> ");//à ajuster
	if (data->prompt == NULL)
	{
		ft_free_env(&(data->env));
		free(data->prompt);
		return (1);//on peut mettre un message de pb de malloc car strdup/ could not initialise minishell
	}
	data->built_in = ft_built_in();
	if (data->built_in == NULL)
	{
		ft_free_env(&(data->env));
		free(data->prompt);
		ft_free_tabstr(data->built_in);
		return (1);//on peut mettre un message de pb de malloc car strdup/ could not initialise minishell
	}
	data->cwd = getcwd(NULL, 0);// sera mise à jour dans cd
	//http://manpagesfr.free.fr/man/man3/getcwd.3.html : ok pour buf NULL et size 0 (POSIX)
	if (!data->cwd)
	{
		ft_free_env(&(data->env));
		free(data->prompt);
		ft_free_tabstr(data->built_in);
		return (1);//could not initialise minishell
	}
	ft_get_home(data);
	dprintf(2, "val data home = %s\n", data->home);
	ft_get_env_path(data);
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
	if (data->oldpwd != NULL)
		free(data->oldpwd);
	if (data->home != NULL)
		free(data->home);
	free(data->prompt);
	if (data->env != NULL)
		ft_free_env(&(data->env));
	if (data->env_path)
		ft_free_env(&(data->env_path));
	if (data->s_env_path)
		ft_free_tabstr(data->s_env_path);
	return (0);
}

void	ft_minishell(t_data *data)
{
	if (ft_lexer(data->line))
	{//dprintf(2, "ft_lexer = %d\n", ft_lexer(data.line));
		//data->val_exit = ft_msg(2, "", "", ERRSTX);
		//g_val_exit = data->val_exit;
		g_val_exit = ft_msg(2, "", "", ERRSTX);
	}
	else
	{
		ft_parser(data); // peut etre faire un if == 0 ou 1
		ft_exec(data); // peut etre faire un if == 0 ou 1
		ft_free_cmd(&(data->cmd)); // A AJUSTER
	}
	ft_clean_cmdline(data);
}

void	ft_exit_ctrl_d(t_data *data)
{
	write(2, "exit\n", 5);
	ft_free_cmd(&(data->cmd)); // A AJUSTER
	ft_clean_cmdline(data);
	ft_clean_cmdline(data);
	rl_clear_history();
	ft_clean_loop(data);
	exit(0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc == 1 && !argv[1])
	{
		if (ft_init_data_1(&data, envp))
			return (1);
		// while (1)
		{
			ft_init_signals(&data);
			data.line = readline(data.prompt);
			if (!data.line)
				ft_exit_ctrl_d(&data);
			if (data.line && ft_strlen(data.line) != 0 && ft_only_space(data.line) == 1)
			{
				add_history(data.line);
				ft_minishell(&data);
			}
		}
		rl_clear_history();
		ft_clean_loop(&data);
		ft_handle_malloc(0, NULL, 0, 0);
		return (0);
	}
	ft_msg(1, "minishell", ": ", ERRARC);
	return (1);
}

// int	main(void)
// {
// 	char	*str;
// 	char	*str2;
// 	char	*s_malloc;
// 	int		i;
// 	int		*t_int;


// 	i = 0;
// 	str = ft_strdup("coucou");
// 	printf("val str = %s\n", str);
// 	printf("str = %p\n", str);
// 	ft_handle_malloc(ADD_M, str, TAB_STR1, 6);

// 	str2 = ft_substr(str, 0, 4);
// 	ft_handle_malloc(ADD_M, str2, TAB_STR1, (4 + 1));

// 	s_malloc = ft_handle_malloc(MALLOC_C, NULL, TAB_STR1, (6 + 1));
// 	while (i < 6)
// 	{
// 		s_malloc[i] = 'c';
// 		i++;
// 	}
// 	s_malloc[i] = '\0';
// 	printf("s_malloc = %s\n", s_malloc);

// 	ft_handle_malloc(DELONE, str, 0, 0);

// 	t_int = ft_handle_malloc(MALLOC_M, NULL, TAB_INT1, 5);
// 	i = 0;
// 	while (i < 5)
// 	{
// 		t_int[i] = i;
// 		i++;
// 	}

// 	ft_handle_malloc(DELONE, s_malloc, 0, 0);
// 	ft_handle_malloc(0, NULL, 0, 0);

// 	printf("val exit %d\n", g_val_exit);

// 	return (1);
// }
