## Struct [std](https://doc.rust-lang.org/std/index.html)::[process](https://doc.rust-lang.org/std/process/index.html)::[Command]

流程构建器，提供对应如何生成新流程的细粒度控制。

可以使用`Command::new(program)`生成默认配置，其中

1. program 给出要执行的程序的路径。
2. 其他构建器方法允许在生成之前更改配置 (例如，通过添加参数):
3. 继承当前进程的环境
4. 继承当前进程的工作目录
5. Inherit stdin/stdout/stderr for [`spawn`](https://doc.rust-lang.org/std/process/struct.Command.html#method.spawn) or [`status`](https://doc.rust-lang.org/std/process/struct.Command.html#method.status), but create pipes for [`output`](https://doc.rust-lang.org/std/process/struct.Command.html#method.output)

```
use std::process::Command;

let output = if cfg!(target_os = "windows") {
    Command::new("cmd")
            .args(["/C", "echo hello"])
            .output()
            .expect("failed to execute process")
} else {
    Command::new("sh")
            .arg("-c")
            .arg("echo hello")
            .output()
            .expect("failed to execute process")
};

let hello = output.stdout;
```



**命令可以重复使用以产生多个进程**

```
use std::process::Command;

let mut echo_hello = Command::new("sh");
echo_hello.arg("-c")
          .arg("echo hello");
let hello_1 = echo_hello.output().expect("failed to execute process");
let hello_2 = echo_hello.output().expect("failed to execute process");
```



**生成进程后调用方法**

```
use std::process::Command;

let mut list_dir = Command::new("ls");

// Execute `ls` in the current directory of the program.
list_dir.status().expect("process failed to execute");

println!();

// Change `ls` to execute in the root directory.
list_dir.current_dir("/");

// And then execute `ls` again but in the root directory.
list_dir.status().expect("process failed to execute");
```

```
use std::process::Command;

Command::new("sh")
        .spawn()
        .expect("sh command failed to start");
```





## API

### new

```
pub fn new<S: AsRef<OsStr>>(program: S) -> Command
```

### arg

```
.arg("-C /path/to/repo")

.arg("-C")
.arg("/path/to/repo")
```

### args

```
use std::process::Command;

Command::new("ls")
        .args(["-l", "-a"])
        .spawn()
        .expect("ls command failed to start");
```

### env

插入或者更新环境变量

```
use std::process::Command;

Command::new("ls")
        .env("PATH", "/bin")
        .spawn()
        .expect("ls command failed to start");
```

### envs

```
use std::process::{Command, Stdio};
use std::env;
use std::collections::HashMap;

let filtered_env : HashMap<String, String> =
    env::vars().filter(|&(ref k, _)|
        k == "TERM" || k == "TZ" || k == "LANG" || k == "PATH"
    ).collect();

Command::new("printenv")
        .stdin(Stdio::null())
        .stdout(Stdio::inherit())
        .env_clear()
        .envs(&filtered_env)
        .spawn()
        .expect("printenv failed to start");
```

### env_remove

```
use std::process::Command;

Command::new("ls")
        .env_remove("PATH")
        .spawn()
        .expect("ls command failed to start");
```

### Env_clear

```
use std::process::Command;

Command::new("ls")
        .env_clear()
        .spawn()
        .expect("ls command failed to start");
```

### current_dir

```
use std::process::Command;

Command::new("ls")
        .current_dir("/bin")
        .spawn()
        .expect("ls command failed to start");
```

### stdin

子进程的标准输入 (stdin) 句柄的配置。

```
use std::process::{Command, Stdio};

Command::new("ls")
        .stdin(Stdio::null())
        .spawn()
        .expect("ls command failed to start");
```

### stdout

```
use std::process::{Command, Stdio};

Command::new("ls")
        .stdout(Stdio::null())
        .spawn()
        .expect("ls command failed to start");
```

### stderr

```
use std::process::{Command, Stdio};

Command::new("ls")
        .stderr(Stdio::null())
        .spawn()
        .expect("ls command failed to start");
```

### spawn

```
use std::process::Command;

Command::new("ls")
        .spawn()
        .expect("ls command failed to start");
```

1. 将命令作为子进程执行，并返回一个句柄。

2. 默认情况下，stdin、stdouts和stderr是从父级继承的。

### output

将命令作为子进程执行，等待它完成并收集其所有输出。

默认情况下，stdot和stderr被捕获 (并用于提供结果输出)。Stdin不会从父级继承，并且子进程尝试从stdin流读取的任何尝试都会导致流立即关闭。



```
use std::process::Command;
use std::io::{self, Write};
let output = Command::new("/bin/cat")
                     .arg("file.txt")
                     .output()
                     .expect("failed to execute process");

println!("status: {}", output.status);
io::stdout().write_all(&output.stdout).unwrap();
io::stderr().write_all(&output.stderr).unwrap();

assert!(output.status.success());
```

### status

1. 作为子进程执行命令，等待它完成并收集其状态。
2. 默认情况下，stdin、stdouts和stderr是从父级继承的。



```
use std::process::Command;

let status = Command::new("/bin/cat")
                     .arg("file.txt")
                     .status()
                     .expect("failed to execute process");

println!("process finished with: {status}");

assert!(status.success());
```

### get_program

返回给程序路径。

```
use std::process::Command;

let cmd = Command::new("echo");
assert_eq!(cmd.get_program(), "echo");
```



### get_args

1. 返回参数迭代器
2. 这不包括程序的路径作为第一个参数; 它只包括  [`Command::arg`](https://doc.rust-lang.org/std/process/struct.Command.html#method.arg)  指定的参数



### get_envs

1. 返回设置子进程的迭代器
2. 元素是(&OsStr, Option<&OsStr>)
   1. 其中第一个值是键，第二个值是值，
   2. 如果要显式删除环境变量，则该值为None。
3. 这仅包括使用 Command::env，Command::envs和Command::env_remove显式设置的环境变量。
4. 它不包括将由子进程继承的环境变量。



### get_current_dir

```
use std::path::Path;
use std::process::Command;

let mut cmd = Command::new("ls");
assert_eq!(cmd.get_current_dir(), None);
cmd.current_dir("/bin");
assert_eq!(cmd.get_current_dir(), Some(Path::new("/bin")));
```





[参考链接](https://doc.rust-lang.org/std/process/struct.Command.html)



