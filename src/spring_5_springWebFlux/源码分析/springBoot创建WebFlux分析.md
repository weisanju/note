# 创建上下文

```java
//org.springframework.boot.SpringApplication#createApplicationContext
ApplicationContextFactory DEFAULT = (webApplicationType) -> {
    try {
        switch (webApplicationType) {
            case SERVLET:
                return new AnnotationConfigServletWebServerApplicationContext();
            case REACTIVE:
                return new AnnotationConfigReactiveWebServerApplicationContext();
            default:
                return new AnnotationConfigApplicationContext();
        }
    }
    catch (Exception ex) {
        throw new IllegalStateException("Unable create a default ApplicationContext instance, "
                                        + "you may need a custom ApplicationContextFactory", ex);
    }
};
```

**如何判断 web应用类型**

```java
	private static final String[] SERVLET_INDICATOR_CLASSES = { "javax.servlet.Servlet",
			"org.springframework.web.context.ConfigurableWebApplicationContext" };

	//webmvc判断类
	private static final String WEBMVC_INDICATOR_CLASS = "org.springframework.web.servlet.DispatcherServlet";

	//webflux判断类
	private static final String WEBFLUX_INDICATOR_CLASS = "org.springframework.web.reactive.DispatcherHandler";

	//jersy
	private static final String JERSEY_INDICATOR_CLASS = "org.glassfish.jersey.servlet.ServletContainer";

	//servlet
	private static final String SERVLET_APPLICATION_CONTEXT_CLASS = "org.springframework.web.context.WebApplicationContext";


static WebApplicationType deduceFromClasspath() {
    if (ClassUtils.isPresent(WEBFLUX_INDICATOR_CLASS, null) && !ClassUtils.isPresent(WEBMVC_INDICATOR_CLASS, null)
        && !ClassUtils.isPresent(JERSEY_INDICATOR_CLASS, null)) {
        return WebApplicationType.REACTIVE;
    }
    for (String className : SERVLET_INDICATOR_CLASSES) {
        if (!ClassUtils.isPresent(className, null)) {
            return WebApplicationType.NONE;
        }
    }
    return WebApplicationType.SERVLET;
}
```



# 创建WebServerManager

```java
//org.springframework.boot.web.reactive.context.ReactiveWebServerApplicationContext#refresh
	private void createWebServer() {
		WebServerManager serverManager = this.serverManager;
		if (serverManager == null) {
			StartupStep createWebServer = this.getApplicationStartup().start("spring.boot.webserver.create");
            //获取 ReactiveWebServerFactory bean名
			String webServerFactoryBeanName = getWebServerFactoryBeanName();
            //获取 ReactiveWebServerFactory bean
			ReactiveWebServerFactory webServerFactory = getWebServerFactory(webServerFactoryBeanName);
            
			createWebServer.tag("factory", webServerFactory.getClass().toString());
			boolean lazyInit = getBeanFactory().getBeanDefinition(webServerFactoryBeanName).isLazyInit();
            //获取 HttpHandlerbean,创建ServerManager
			this.serverManager = new WebServerManager(this, webServerFactory, this::getHttpHandler, lazyInit);
			getBeanFactory().registerSingleton("webServerGracefulShutdown",
					new WebServerGracefulShutdownLifecycle(this.serverManager.getWebServer()));
			getBeanFactory().registerSingleton("webServerStartStop",
					new WebServerStartStopLifecycle(this.serverManager));
			createWebServer.end();
		}
		initPropertySources();
	}
```



# 创建WebSevrer

## NettyServer

```java
	public WebServer getWebServer(HttpHandler httpHandler) {
		HttpServer httpServer = createHttpServer();
		ReactorHttpHandlerAdapter handlerAdapter = new ReactorHttpHandlerAdapter(httpHandler);
		NettyWebServer webServer = createNettyWebServer(httpServer, handlerAdapter, this.lifecycleTimeout,
				getShutdown());
		webServer.setRouteProviders(this.routeProviders);
		return webServer;
	}
```

