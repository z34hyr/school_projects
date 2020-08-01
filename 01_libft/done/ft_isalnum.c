/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qmalcom <qmalcom@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/07 16:23:20 by qmalcom           #+#    #+#             */
/*   Updated: 2020/05/07 16:23:25 by qmalcom          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_isalnum(int c)
{
	if ((c >= 65 && c <= 90) || \
		(c >= 97 && c <= 122) || \
		(c >= 48 && c <= 57))
		return (1);
	return (0);
}
