# Dependency Metadata

Artifacts for final releases and milestones are deployed to [Maven Central](https://search.maven.org/), and snapshot artifacts are deployed to Sonatype’s [snapshots repository](https://oss.sonatype.org/content/repositories/snapshots) under [/org/junit](https://oss.sonatype.org/content/repositories/snapshots/org/junit/).





## JUnit Platform

- **Group ID**: `org.junit.platform`
- **Version**: `1.8.2`
- **Artifact IDs**:



### junit-platform-commons

为 *JUnit Platform*  提供通用api和工具。用  `@API(status = INTERNAL)`   注释的任何API仅用于JUnit框架本身。不支持外部方对内部api的任何使用!

### junit-platform-console

Support for discovering and executing tests on the JUnit Platform from the console. See [Console Launcher](https://junit.org/junit5/docs/current/user-guide/#running-tests-console-launcher) for details.

### junit-platform-console-standalone

An executable JAR with all dependencies included is provided in Maven Central under the [junit-platform-console-standalone](https://repo1.maven.org/maven2/org/junit/platform/junit-platform-console-standalone) directory. See [Console Launcher](https://junit.org/junit5/docs/current/user-guide/#running-tests-console-launcher) for details.

### junit-platform-engine

Public API for test engines. See [Registering a TestEngine](https://junit.org/junit5/docs/current/user-guide/#launcher-api-engines-custom) for details.

### junit-platform-jfr

Provides a `LauncherDiscoveryListener` and `TestExecutionListener` for Java Flight Recorder events on the JUnit Platform. See [Flight Recorder Support](https://junit.org/junit5/docs/current/user-guide/#running-tests-listeners-flight-recorder) for details.

### junit-platform-launcher

Public API for configuring and launching test plans — typically used by IDEs and build tools. See [JUnit Platform Launcher API](https://junit.org/junit5/docs/current/user-guide/#launcher-api) for details.

### junit-platform-reporting

`TestExecutionListener` implementations that generate test reports — typically used by IDEs and build tools. See [JUnit Platform Reporting](https://junit.org/junit5/docs/current/user-guide/#junit-platform-reporting) for details.

### junit-platform-runner

Runner for executing tests and test suites on the JUnit Platform in a JUnit 4 environment. See [Using JUnit 4 to run the JUnit Platform](https://junit.org/junit5/docs/current/user-guide/#running-tests-junit-platform-runner) for details.

### junit-platform-suite

JUnit Platform Suite artifact that transitively pulls in dependencies on `junit-platform-suite-api` and `junit-platform-suite-engine` for simplified dependency management in build tools such as Gradle and Maven.

### junit-platform-suite-api

Annotations for configuring test suites on the JUnit Platform. Supported by the [JUnit Platform Suite Engine](https://junit.org/junit5/docs/current/user-guide/#junit-platform-suite-engine) and the [JUnitPlatform runner](https://junit.org/junit5/docs/current/user-guide/#running-tests-junit-platform-runner).

### junit-platform-suite-commons

Common support utilities for executing test suites on the JUnit Platform.

### junit-platform-suite-engine

Engine that executes test suites on the JUnit Platform; only required at runtime. See [JUnit Platform Suite Engine](https://junit.org/junit5/docs/current/user-guide/#junit-platform-suite-engine) for details.

### junit-platform-testkit

Provides support for executing a test plan for a given `TestEngine` and then accessing the results via a fluent API to verify the expected results.

为执行给定TestEngine的测试计划提供支持，然后通过fluent API访问结果以验证预期结果。



## JUnit Jupiter

- **Group ID**: `org.junit.jupiter`
- **Version**: `5.8.2`
- **Artifact IDs**:

### junit-jupiter

JUnit Jupiter aggregator artifact that transitively pulls in dependencies on `junit-jupiter-api`, `junit-jupiter-params`, and `junit-jupiter-engine` for simplified dependency management in build tools such as Gradle and Maven.

*JUnit Jupiter aggregator artifact*，可传递地拉入对junit-jupiter-api，junit-jupiter-params和junit-jupiter-engine的依赖关系，以简化构建工具 (例如Gradle和Maven) 中的依赖关系。

### junit-jupiter-api

JUnit Jupiter API for [writing tests](https://junit.org/junit5/docs/current/user-guide/#writing-tests) and [extensions](https://junit.org/junit5/docs/current/user-guide/#extensions).

### junit-jupiter-engine

JUnit Jupiter test engine implementation; only required at runtime.

### junit-jupiter-params

Support for [parameterized tests](https://junit.org/junit5/docs/current/user-guide/#writing-tests-parameterized-tests) in JUnit Jupiter.

### junit-jupiter-migrationsupport

Support for migrating from JUnit 4 to JUnit Jupiter; only required for support for JUnit 4’s `@Ignore` annotation and for running selected JUnit 4 rules.



## JUnit Vintage

- **Group ID**: `org.junit.vintage`
- **Version**: `5.8.2`
- **Artifact ID**:

### `junit-vintage-engine`

JUnit Vintage test engine implementation that allows one to run *vintage* JUnit tests on the JUnit Platform. *Vintage* tests include those written using JUnit 3 or JUnit 4 APIs or tests written using testing frameworks built on those APIs.

JUnit Vintage 测试引擎实现，允许在JUnit平台上运行老式JUnit测试。老式测试包括使用JUnit 3或JUnit 4 api编写的测试，或使用基于这些api构建的测试框架编写的测试。



## Bill of Materials (BOM)

The *Bill of Materials* POM provided under the following Maven coordinates can be used to ease dependency management when referencing multiple of the above artifacts using [Maven](https://maven.apache.org/guides/introduction/introduction-to-dependency-mechanism.html#Importing_Dependencies) or [Gradle](https://docs.gradle.org/current/userguide/managing_transitive_dependencies.html#sec:bom_import).

- **Group ID**: `org.junit`
- **Artifact ID**: `junit-bom`
- **Version**: `5.8.2`

## Dependencies

Most of the above artifacts have a dependency in their published Maven POMs on the following *@API Guardian* JAR.

以上大多数*artifacts*都在其发布的Maven POMs中依赖于以下 @ API Guardian JAR。

- **Group ID**: `org.apiguardian`
- **Artifact ID**: `apiguardian-api`
- **Version**: `1.1.2`

In addition, most of the above artifacts have a direct or transitive dependency on the following *OpenTest4J* JAR.

- **Group ID**: `org.opentest4j`
- **Artifact ID**: `opentest4j`
- **Version**: `1.2.0`







![component diagram](https://junit.org/junit5/docs/current/user-guide/images/component-diagram.svg)