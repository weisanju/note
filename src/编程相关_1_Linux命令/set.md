

# set命令

**set命令**作用主要是显示系统中已经存在的shell变量，以及设置shell变量的新变量值。使用set更改shell特性时，符号"+"和"-"的作用分别是打开和关闭指定的模式。set命令不能够定义新的shell变量。如果要定义新的变量，可以使用[declare](http://man.linuxde.net/declare)命令以`变量名=值`的格式进行定义即可。





# 语法

```
set(选项)(参数)
```



# 选项 

```
NAME
    set - Set or unset values of shell options and positional parameters.

SYNOPSIS
    set [-abefhkmnptuvxBCHP] [-o option-name] [--] [arg ...]

DESCRIPTION
    Set or unset values of shell options and positional parameters.
    
    Change the value of shell attributes and positional parameters, or
    display the names and values of shell variables.
    
    Options:
      -a  Mark variables which are modified or created for export.
      -b  Notify of job termination immediately.
      -e  Exit immediately if a command exits with a non-zero status.
      -f  Disable file name generation (globbing).
      -h  Remember the location of commands as they are looked up.
      -k  All assignment arguments are placed in the environment for a
          command, not just those that precede the command name.
      -m  Job control is enabled.
      -n  Read commands but do not execute them.
      -o option-name
          Set the variable corresponding to option-name:
              allexport    same as -a
              braceexpand  same as -B
              emacs        use an emacs-style line editing interface
              errexit      same as -e
              errtrace     same as -E
              functrace    same as -T
              hashall      same as -h
              histexpand   same as -H
              history      enable command history
              ignoreeof    the shell will not exit upon reading EOF
              interactive-comments
                           allow comments to appear in interactive commands
              keyword      same as -k
              monitor      same as -m
              noclobber    same as -C
              noexec       same as -n
              noglob       same as -f
              nolog        currently accepted but ignored
              notify       same as -b
              nounset      same as -u
              onecmd       same as -t
              physical     same as -P
              pipefail     the return value of a pipeline is the status of
                           the last command to exit with a non-zero status,
                           or zero if no command exited with a non-zero status
              posix        change the behavior of bash where the default
                           operation differs from the Posix standard to
                           match the standard
              privileged   same as -p
              verbose      same as -v
              vi           use a vi-style line editing interface
              xtrace       same as -x
      -p  Turned on whenever the real and effective user ids do not match.
          Disables processing of the $ENV file and importing of shell
          functions.  Turning this option off causes the effective uid and
          gid to be set to the real uid and gid.
      -t  Exit after reading and executing one command.
      -u  Treat unset variables as an error when substituting.
      -v  Print shell input lines as they are read.
      -x  Print commands and their arguments as they are executed.
      -B  the shell will perform brace expansion
      -C  If set, disallow existing regular files to be overwritten
          by redirection of output.
      -E  If set, the ERR trap is inherited by shell functions.
      -H  Enable ! style history substitution.  This flag is on
          by default when the shell is interactive.
      -P  If set, do not resolve symbolic links when executing commands
          such as cd which change the current directory.
      -T  If set, the DEBUG and RETURN traps are inherited by shell functions.
      --  Assign any remaining arguments to the positional parameters.
          If there are no remaining arguments, the positional parameters
          are unset. 重置 shell位置参数
      -   Assign any remaining arguments to the positional parameters.
          The -x and -v options are turned off.
    
    Using + rather than - causes these flags to be turned off.  The
    flags can also be used upon invocation of the shell.  The current
    set of flags may be found in $-.  The remaining n ARGs are positional
    parameters and are assigned, in order, to $1, $2, .. $n.  If no
    ARGs are given, all shell variables are printed.
    
    Exit Status:
    Returns success unless an invalid option is given.xxxxxxxxxx -a：标示已修改的变量，以供输出至环境变量。-b：使被中止的后台程序立刻回报执行状态。-C：转向所产生的文件无法覆盖已存在的文件。-d：Shell预设会用杂凑表记忆使用过的指令，以加速指令的执行。使用-d参数可取消。-e：若指令传回值不等于0，则立即退出shell。-f：取消使用通配符。-h：自动记录函数的所在位置。-H Shell：可利用"!"加<指令编号>的方式来执行history中记录的指令。-k：指令所给的参数都会被视为此指令的环境变量。-l：记录for循环的变量名称。-m：使用监视模式。-n：只读取指令，而不实际执行。-p：启动优先顺序模式。-P：启动-P参数后，执行指令时，会以实际的文件或目录来取代符号连接。-t：执行完随后的指令，即退出shell。-u：当执行时使用到未定义过的变量，则显示错误信息。-v：显示shell所读取的输入值。-x：执行指令后，会先显示该指令及所下的参数。 -o option-name          Set the variable corresponding to option-name:              allexport    same as -a              braceexpand  same as -B              emacs        use an emacs-style line editing interface              errexit      same as -e              errtrace     same as -E              functrace    same as -T              hashall      same as -h              histexpand   same as -H              history      enable command history              ignoreeof    the shell will not exit upon reading EOF              interactive-comments                           allow comments to appear in interactive commands              keyword      same as -k              monitor      same as -m              noclobber    same as -C              noexec       same as -n              noglob       same as -f              nolog        currently accepted but ignored              notify       same as -b              nounset      same as -u              onecmd       same as -t              physical     same as -P              pipefail     the return value of a pipeline is the status of                           the last command to exit with a non-zero status,                           or zero if no command exited with a non-zero status              posix        change the behavior of bash where the default                           operation differs from the Posix standard to                           match the standard              privileged   same as -p              verbose      same as -v              vi           use a vi-style line editing interface              xtrace       same as -xNAME    set - Set or unset values of shell options and positional parameters.SYNOPSIS    set [-abefhkmnptuvxBCHP] [-o option-name] [--] [arg ...]DESCRIPTION    Set or unset values of shell options and positional parameters.        Change the value of shell attributes and positional parameters, or    display the names and values of shell variables.        Options:      -a  Mark variables which are modified or created for export.      -b  Notify of job termination immediately.      -e  Exit immediately if a command exits with a non-zero status.      -f  Disable file name generation (globbing).      -h  Remember the location of commands as they are looked up.      -k  All assignment arguments are placed in the environment for a          command, not just those that precede the command name.      -m  Job control is enabled.      -n  Read commands but do not execute them.      -o option-name          Set the variable corresponding to option-name:              allexport    same as -a              braceexpand  same as -B              emacs        use an emacs-style line editing interface              errexit      same as -e              errtrace     same as -E              functrace    same as -T              hashall      same as -h              histexpand   same as -H              history      enable command history              ignoreeof    the shell will not exit upon reading EOF              interactive-comments                           allow comments to appear in interactive commands              keyword      same as -k              monitor      same as -m              noclobber    same as -C              noexec       same as -n              noglob       same as -f              nolog        currently accepted but ignored              notify       same as -b              nounset      same as -u              onecmd       same as -t              physical     same as -P              pipefail     the return value of a pipeline is the status of                           the last command to exit with a non-zero status,                           or zero if no command exited with a non-zero status              posix        change the behavior of bash where the default                           operation differs from the Posix standard to                           match the standard              privileged   same as -p              verbose      same as -v              vi           use a vi-style line editing interface              xtrace       same as -x      -p  Turned on whenever the real and effective user ids do not match.          Disables processing of the $ENV file and importing of shell          functions.  Turning this option off causes the effective uid and          gid to be set to the real uid and gid.      -t  Exit after reading and executing one command.      -u  Treat unset variables as an error when substituting.      -v  Print shell input lines as they are read.      -x  Print commands and their arguments as they are executed.      -B  the shell will perform brace expansion      -C  If set, disallow existing regular files to be overwritten          by redirection of output.      -E  If set, the ERR trap is inherited by shell functions.      -H  Enable ! style history substitution.  This flag is on          by default when the shell is interactive.      -P  If set, do not resolve symbolic links when executing commands          such as cd which change the current directory.      -T  If set, the DEBUG and RETURN traps are inherited by shell functions.      --  Assign any remaining arguments to the positional parameters.          If there are no remaining arguments, the positional parameters          are unset.      -   Assign any remaining arguments to the positional parameters.          The -x and -v options are turned off.        Using + rather than - causes these flags to be turned off.  The    flags can also be used upon invocation of the shell.  The current    set of flags may be found in $-.  The remaining n ARGs are positional    parameters and are assigned, in order, to $1, $2, .. $n.  If no    ARGs are given, all shell variables are printed.        Exit Status:    Returns success unless an invalid option is given.
```

使用declare命令定义一个新的环境变量"mylove"，并且将其值设置为"Visual C++"，输入如下命令：







