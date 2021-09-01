# RandomAccessFileAppender

RandomAccessFileAppender 类似于标准的 FileAppender，除了它总是被缓冲（不能关闭），并且在内部它使用 ByteBuffer + RandomAccessFile 而不是 BufferedOutputStream





The RandomAccessFileAppender is similar to the standard [FileAppender](https://logging.apache.org/log4j/2.x/manual/appenders.html#FileAppender) except it is always buffered (this cannot be switched off) and internally it uses a ByteBuffer + RandomAccessFile instead of a BufferedOutputStream. We saw a 20-200% performance improvement compared to FileAppender with "bufferedIO=true" in our [measurements](https://logging.apache.org/log4j/2.x/performance.html#whichAppender). Similar to the FileAppender, RandomAccessFileAppender uses a RandomAccessFileManager to actually perform the file I/O. While RandomAccessFileAppender from different Configurations cannot be shared, the RandomAccessFileManagers can be if the Manager is accessible. For example, two web applications in a servlet container can have their own configuration and safely write to the same file if Log4j is in a ClassLoader that is common to both of them.



