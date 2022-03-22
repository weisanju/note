# Apache Maven Archiver

Maven Archiver主要由插件使用以处理打包。

[详见](http://maven.apache.org/shared/maven-archiver/index.html)

## archive示例配置

```xml
<archive>
  <addMavenDescriptor/>
  <compress/>
  <forced/>
  <index/>
  <pomPropertiesFile/>
 
  <manifestFile/>
  <manifest>
    <addClasspath/>
    <addDefaultEntries/>
    <addDefaultImplementationEntries/>
    <addDefaultSpecificationEntries/>
    <addBuildEnvironmentEntries/>
    <addExtensions/>
    <classpathLayoutType/>
    <classpathPrefix/>
    <customClasspathLayout/>
    <mainClass/>
    <packageName/>
    <useUniqueVersions/>
  </manifest>
  <manifestEntries>
    <key>value</key>
  </manifestEntries>
  <manifestSections>
    <manifestSection>
      <name/>
      <manifestEntries>
        <key>value</key>
      </manifestEntries>
    <manifestSection/>
  </manifestSections>
</archive>
```

## archive元素

> since的版本 是 Maven Archiver component 的版本，而不是plugin的版本

| Element                                                      | Description                                                  | Type    | 默认值  | Since |
| :----------------------------------------------------------- | :----------------------------------------------------------- | :------ | ------- | :---- |
| addMavenDescriptor                                           | 是否包含两个文件: `META-INF/maven/${groupId}/${artifactId}/pom.xml` [pom.properties](http://maven.apache.org/shared/maven-archiver/index.html#pom-properties-content) file, located in the archive in `META-INF/maven/${groupId}/${artifactId}/pom.properties` | boolean | *true*  |       |
| compress                                                     | 是由启用压缩                                                 | boolean | *true*  |       |
| forced                                                       | 是否强制重新创建存档（默认），*false*意味着应将包含文件的时间戳与目标存档文件的存档时间戳进行比较，只有时间戳更新过了才会更新存档. 特别是，不会检测到源文件的删除. | boolean | *true*  | 2.2   |
| index                                                        | 创建的档案是否将包含一个“ INDEX.LIST”文件。                  | boolean | *false* |       |
| pomPropertiesFile                                            | 使用这个文件覆盖自动生成的 [pom.properties](http://maven.apache.org/shared/maven-archiver/index.html#pom-properties-content) file (only if `addMavenDescriptor` is set to `true`) | File    |         | 2.3   |
| manifestFile                                                 | 自定义 manifest文件                                          | File    |         |       |
| [manifest](http://maven.apache.org/shared/maven-archiver/index.html#class_manifest) |                                                              |         |         |       |
| manifestEntries                                              | A list of key/value pairs to add to the manifest.            | Map     |         |       |
| [manifestSections](http://maven.apache.org/shared/maven-archiver/index.html#class_manifestSection) |                                                              |         |         |       |

## pom.properties content

自动创建的`pom.properties`文件将包含以下内容

```
artifactId=${project.artifactId}
groupId=${project.groupId}
version=${project.version}
```

## manifest

| Element                         | Type    | Description                                                  | 默认值   | Since       |
| :------------------------------ | :------ | :----------------------------------------------------------- | -------- | :---------- |
| addClasspath                    | boolean | 是否创建 `Class-Path`                                        | true     |             |
| addDefaultEntries               | boolean | 默认设置的 entries,<br />Created-By: Maven Archiver \${maven-archiver.version}<br />Build-Jdk-Spec: ${java.specification.version} | true     | 3.4.0       |
| addDefaultImplementationEntries | boolean | 是否添加以下*entries*<br />Implementation-Title: \${project.name}<br />Implementation-Version: \${project.version}<br />Implementation-Vendor: ${project.organization.name} | false    | 2.1 and 2.6 |
| addDefaultSpecificationEntries  | boolean | 是否添加以下*entries*:<br />Specification-Title: \${project.name}<br />Specification-Version: \${project.artifact.selectedVersion.majorVersion}.​\${project.artifact.selectedVersion.minorVersion}<br />Specification-Vendor: ${project.organization.name} | false    | 2.1         |
| addBuildEnvironmentEntries      | boolean | 是否添加以下*entries*:<br />Build-Tool: \${maven.build.version}<br />Build-Jdk: \${java.version} (​\${java.vendor})<br />Build-Os:  ​\${os.name} (​\${os.version}; (${os.arch}) | false    | 3.4.0       |
| addExtensions                   | boolean | 是否添加以下*entries*<br />`Extension-List` manifest entry   | false    |             |
| classpathLayoutType             | String  | 生成 `Class-Path`的 *layoutType* 可选: <br />`simple`, <br />`repository` (the same as a Maven classpath layout) and <br />`custom`. If you specify a type of `custom` you **must** also set `customClasspathLayout`. | `simple` | 2.4         |
| classpathPrefix                 | String  | A text that will be prefixed to all your `Class-Path` entries. The default value is `""`. |          |             |
| customClasspathLayout           | String  | 使用`custom` 的时候指定的表达式，<br />将根据以下与类路径相关的对象的有序列表对表达式进行求值<br />The current Artifact instance, if one exists.<br />The current ArtifactHandler instance from the artifact above |          | 2.4         |
| mainClass                       | String  | The `Main-Class` manifest entry.                             |          |             |
| packageName                     | String  | Package manifest entry.                                      |          |             |
| useUniqueVersions               |         | 是否使用唯一的时间戳快照版本而不是-SNAPSHOT版本。            | true     |             |

