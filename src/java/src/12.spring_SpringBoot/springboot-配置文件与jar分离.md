1. 配置maven resource插件

   ```xml
   <plugin>
       <artifactId>maven-resources-plugin</artifactId>
       <executions>
           <execution>
               <id>copy-resources</id>
               <phase>package</phase>
               <goals>
                   <goal>copy-resources</goal>
               </goals>
               <configuration>
                   <resources>
                       <resource>
                        	<directory>src/main/resources</directory> 
                       </resource>
                   </resources>
                   <outputDirectory>${project.build.directory}/config</outputDirectory>
               </configuration>
           </execution>
       </executions>
   </plugin>
   ```

2. 将项目