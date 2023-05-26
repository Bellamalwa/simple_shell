#include "shell.h"

/**
 * exitShell - exits the shell
 * @info: Structure containing potential arguments.
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */
int exitShell(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* NOTE: If exit arguement exists */
	{
		exitcheck = printAtoiError(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			printCharError(info->argv[1]);
			printStringError('\n');
			return (1);
		}
		info->err_num = printAtoiError(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * cdFunc - changes the current directory of the process
 * @info: Structure containing potential arguments.
 * constant function prototype.
 * Return: Always 0
 */
int cdFunc(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = _getenv(info, "HOME=");
		if (!dir)
			chdir_ret = chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		printCharError(info->argv[1]), printStringError('\n');
	}
	else
	{
		_setenv(info, "OLDPWD", _getenv(info, "PWD="));
		_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * helpFunc - Print the help menu with available functions
 * @info: Structure containing potential arguments.
 * Return: Always 0
 */
int helpFunc(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("Function not yet implemented... \n");
	if (0)
		_puts(*arg_array); /* NOTE: att_unused workaround */
	return (0);
}

