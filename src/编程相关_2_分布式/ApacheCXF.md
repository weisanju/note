# 简介

Apache CXF 是一个开源的 Services 框架，CXF 帮助您利用 Frontend 编程 API 来构建和开发 Services

支持多协议,SOAP、XML/HTTP、RESTful HTTP 或者 CORBA 

# 代码生成

- Java to WSDL；
- WSDL to Java；
- XSD to WSDL；
- WSDL to XML；
- WSDL to SOAP；
- WSDL to Service；

# 目录结构

## bin

提供代码生成,校验,管理控制台工具

- Java to WSDL : java2wsdl
- CXF Management Console Tool : mc
- WSDL to Java : wsdl2java
- WSDL to Service : wsdl2service
- WSDL to SOAP : wsdl2soap
- WSDL to XML : wsdl2xml
- WSDL Validation : wsdlvalidator
- XSD to WSDL : xsd2wsdl

## docs

CXF 所有类（class）对应的 API 文档，为开发者使用 CXF 完成应用开发提供应有的帮助。

## etc

包含一个基本的 Service 暴露所需要的 web.xml 文件，及其它的配置文件

## lib

lib 目录中包含 CXF 及其运行时所需要的和可选的第三方支持类包

## licenses

引用第三方 jar 包的相关许可协议

## modules

包含了 CXF 框架根据不同特性分开进行编译的二进制包文件。发布基于 CXF 框架的 Web 项目时，可以选择使用该目录下的所有 .jar 文件，也可以选择 lib 目录中的 cxf-2.0.2-incubator.jar 文件。

## samples

包含了所有随 CXF 二进制包发布的示例,包含这些示例的源代码和相关 Web 应用配置文件,可以方便地用 Ant 来编译运行测试这些示例



CXF 框架是一种基于 Servlet 技术的 SOA 应用开发框架