## 下载安装

```
https://repo1.maven.org/maven2/org/junit/platform/junit-platform-console-standalone/1.8.2/junit-platform-console-standalone-1.8.2.jar
```





## 启动与运行

ConsoleLauncher是一个命令行Java应用程序，可让您从控制台启动JUnit平台。例如，它可用于运行JUnit Vintage和JUnit Jupiter测试，并将测试执行结果打印到控制台。



包含所有依赖项的可执行junit-platform-console-standalone-1.8.2.jar将在junit-platform-console-standalone目录下的Maven中央存储库中发布。您可以运行独立的consoleauncher，如下所示。



```
java -jar junit-platform-console-standalone-1.8.2.jar <Options>
```

Here’s an example of its output:

```
├─ JUnit Vintage
│  └─ example.JUnit4Tests
│     └─ standardJUnit4Test ✔
└─ JUnit Jupiter
   ├─ StandardTests
   │  ├─ succeedingTest() ✔
   │  └─ skippedTest() ↷ for demonstration purposes
   └─ A special test case
      ├─ Custom test name containing spaces ✔
      ├─ ╯°□°)╯ ✔
      └─ 😱 ✔

Test run finished after 64 ms
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         6 tests found           ]
[         1 tests skipped         ]
[         5 tests started         ]
[         0 tests aborted         ]
[         5 tests successful      ]
[         0 tests failed          ]
```



### exitCode

如果任何容器或测试失败，则consoleelauncher退出，状态代码为1。如果未发现任何测试，并且提供了 -- fail-If-no-tests命令行选项，则consoleauncher退出，状态代码为2。否则退出代码为0。



## Options

```
Usage: ConsoleLauncher [-h] [--disable-ansi-colors] [--disable-banner]
                       [--fail-if-no-tests] [--scan-modules] [--scan-classpath[=PATH[;|:
                       PATH...]]]... [--details=MODE] [--details-theme=THEME]
                       [--reports-dir=DIR] [-c=CLASS]... [--config=KEY=VALUE]... [-cp=PATH
                       [;|:PATH...]]... [-d=DIR]... [-e=ID]... [-E=ID]...
                       [--exclude-package=PKG]... [-f=FILE]... [--include-package=PKG]...
                       [-m=NAME]... [-n=PATTERN]... [-N=PATTERN]... [-o=NAME]...
                       [-p=PKG]... [-r=RESOURCE]... [-t=TAG]... [-T=TAG]... [-u=URI]...
Launches the JUnit Platform from the console.
  -h, --help                 Display help information.
      --disable-ansi-colors  Disable ANSI colors in output (not supported by all
                               terminals).
      --disable-banner       Disable print out of the welcome message.
      --details=MODE         Select an output details mode for when tests are executed.
                               Use one of: none, summary, flat, tree, verbose. If 'none'
                               is selected, then only the summary and test failures are
                               shown. Default: tree.
      --details-theme=THEME  Select an output details tree theme for when tests are
                               executed. Use one of: ascii, unicode. Default: unicode.
      -cp, --classpath, --class-path=PATH[;|:PATH...]
                             Provide additional classpath entries -- for example, for
                               adding engines and their dependencies. This option can be
                               repeated.
      --fail-if-no-tests     Fail and return exit status code 2 if no tests are found.
      --reports-dir=DIR      Enable report output into a specified local directory (will
                               be created if it does not exist).
      --scan-modules         EXPERIMENTAL: Scan all resolved modules for test discovery.
  -o, --select-module=NAME   EXPERIMENTAL: Select single module for test discovery. This
                               option can be repeated.
      --scan-classpath, --scan-class-path[=PATH[;|:PATH...]]
                             Scan all directories on the classpath or explicit classpath
                               roots. Without arguments, only directories on the system
                               classpath as well as additional classpath entries supplied
                               via -cp (directories and JAR files) are scanned. Explicit
                               classpath roots that are not on the classpath will be
                               silently ignored. This option can be repeated.
  -u, --select-uri=URI       Select a URI for test discovery. This option can be repeated.
  -f, --select-file=FILE     Select a file for test discovery. This option can be
                               repeated.
  -d, --select-directory=DIR Select a directory for test discovery. This option can be
                               repeated.
  -p, --select-package=PKG   Select a package for test discovery. This option can be
                               repeated.
  -c, --select-class=CLASS   Select a class for test discovery. This option can be
                               repeated.
  -m, --select-method=NAME   Select a method for test discovery. This option can be
                               repeated.
  -r, --select-resource=RESOURCE
                             Select a classpath resource for test discovery. This option
                               can be repeated.
  -n, --include-classname=PATTERN
                             Provide a regular expression to include only classes whose
                               fully qualified names match. To avoid loading classes
                               unnecessarily, the default pattern only includes class
                               names that begin with "Test" or end with "Test" or
                               "Tests". When this option is repeated, all patterns will
                               be combined using OR semantics. Default: [^(Test.*|.+[.$]
                               Test.*|.*Tests?)$]
  -N, --exclude-classname=PATTERN
                             Provide a regular expression to exclude those classes whose
                               fully qualified names match. When this option is repeated,
                               all patterns will be combined using OR semantics.
      --include-package=PKG  Provide a package to be included in the test run. This
                               option can be repeated.
      --exclude-package=PKG  Provide a package to be excluded from the test run. This
                               option can be repeated.
  -t, --include-tag=TAG      Provide a tag or tag expression to include only tests whose
                               tags match. When this option is repeated, all patterns
                               will be combined using OR semantics.
  -T, --exclude-tag=TAG      Provide a tag or tag expression to exclude those tests whose
                               tags match. When this option is repeated, all patterns
                               will be combined using OR semantics.
  -e, --include-engine=ID    Provide the ID of an engine to be included in the test run.
                               This option can be repeated.
  -E, --exclude-engine=ID    Provide the ID of an engine to be excluded from the test
                               run. This option can be repeated.
      --config=KEY=VALUE     Set a configuration parameter for test discovery and
                               execution. This option can be repeated.
```