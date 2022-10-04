/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaillag <cgaillag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 22:14:29 by cgaillag          #+#    #+#             */
/*   Updated: 2022/10/04 02:17:10 by cgaillag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*	<SUMMARY> adds a new node at the beginning of the t_bin list
*/
int	ft_lstadd_bin(t_bin **bin, void *ptr, int type, int size)
{
	t_bin	*new;

	new = NULL;
	new = ft_handle_malloc(1000, NULL, LST_BIN, size);

	printf("test - lstadd_bin début\n");
	new->ptr = ptr;
	new->type = type;
	new->size = size;
	new->next = NULL;
	if ((*bin) != NULL)
		new->next = (*bin);
	(*bin) = new;
	return (0);
}

/*	<SUMMARY> frees one node (in parameter)
**	<REMARK> defines which kind of free to apply depending on type & size
*/
void	ft_lstdelone_bin(t_bin *node)
{
	if (!node)
		return ;
	if (node->type == TAB_STR1 && (char *)node)// peut-être pas nécessaire de caster
		free((char *)node);
	else if (node->type == TAB_INT1 && (int *)node)
		free((int *)node);
	else if (node->type == TAB_STRS && (char **)node)
		ft_free_tabstr((char **)node);
	else if (node->type == TAB_INTS && (int **)node)
		ft_free_tabint((int **)node, node->size);
	else if (node->type == LST_ENV && (t_env *)node)
		ft_free_env((t_env **)node);
	else if (node->type == LST_CMD && (t_cmd *)node)
		ft_free_cmd((t_cmd **)node);
	else if (node->type == LST_TOK && (t_token *)node)
		ft_free_token((t_token **)node);
	node->next = NULL;//vérifier si déjà dans les fonctions de ft_free_bon
	free(node);
	node = NULL;
}

/*	<SUMMARY> frees the node associated to ptr in the linked list
*/
void	ft_lstclearone_bin(t_bin **bin_head, void *ptr)
{
	t_bin	*bin;
	t_bin	*todel;
	t_bin	*tmp;

	bin = (*bin_head);
	todel = NULL;
	tmp = NULL;
	// comment comparer des pointeurs ?
	if (bin->ptr = ptr)
	{
		todel = bin;
		tmp = (*bin_head)->next;
		//todel->next = NULL;// prévu dans ft_lstdelone_bin
		ft_lstdelone_bin(todel);
		(*bin_head) = tmp;
		return ;
	}
	while (bin)
	{
		if (bin->next && bin->next->ptr == ptr)
		{
			todel = bin->next;
			if (bin->next->next != NULL)
			{
				tmp = bin->next->next;
				//todel->next = NULL;// prévu dans ft_lstdelone_bin
				bin->next = tmp;
			}
			else
				bin->next = NULL;
			ft_lstdelone_bin(todel);
		}
		bin = bin->next;
	}
	return ;//pas oblig
}

/*	<SUMMARY> deletes the full t_bin linked list by deleting one node after
**	the other: starting from 1st node (head)
*/
void	ft_free_bin(t_bin **bin)
{
	t_bin	*tmp;

	if (!*bin)
		return ;
	while (*bin != NULL)
	{
		tmp = (*bin)->next;
		ft_lstdelone_bin(*bin);
		(*bin) = tmp;
	}
	(*bin) = NULL;
}
