# Modules

首先是 Modules，如下图所示，最开始生成的 Module 里的 jdwork，并没有把我们新生成的 webapp以及子目录放入里边，这样子的话项目在部署和启动的时候是找不到web.xml这个文件的。与此对应的是，项目目录结构中webapp没有任何标记，跟一般文件夹没区别。