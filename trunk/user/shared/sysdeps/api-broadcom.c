/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define nvram_get_ nvram_get

extern char *nvram_get(const char *name);
extern int _nvram_set(const char *name, const char *value, int is_temp);

char *
nvram_safe_get(const char *name)
{
	static const char nvram_empty[4] = {0};

	char *value = nvram_get_(name);
	if (value)
		return value;
	else
		return (char *)nvram_empty;
}

int
nvram_get_int(const char *name)
{
	char *value = nvram_get_(name);
	if (value)
		return atoi(value);
	else
		return 0;
}

int
nvram_safe_get_int(const char* name, int val_def, int val_min, int val_max)
{
	int i_value = nvram_get_int(name);
	if (i_value < val_min)
		i_value = val_def;
	else if (i_value > val_max)
		i_value = val_def;
	return i_value;
}

int
nvram_set_temp(const char *name, const char *value)
{
	return _nvram_set(name, value, 1);
}

int
nvram_set_int(const char *name, int value)
{
	char int_str[16];
	sprintf(int_str, "%d", value);
	return _nvram_set(name, int_str, 0);
}

int
nvram_set_int_temp(const char *name, int value)
{
	char int_str[16];
	sprintf(int_str, "%d", value);
	return _nvram_set(name, int_str, 1);
}

int
nvram_match(const char *name, char *match)
{
	const char *value = nvram_get(name);
	return (value && !strcmp(value, match));
}

int
nvram_invmatch(const char *name, char *invmatch)
{
	const char *value = nvram_get(name);
	return (value && strcmp(value, invmatch));
}
