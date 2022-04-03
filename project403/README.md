# day09
## 实现
1. 实现了基于多线程IO复用的http服务器，主线程用来接受新的连接和监听发起请求的客户端连接，子线程用来处理客户端发起的http请求。
2. 基于模板类、thread库、mutex、condition variable、future、function实现了线程池。由于虚拟机限制，线程池的线程数设置为4,通过将处理请求事件的函数指针放入线程池的任务队列，然后会通过条件变量对象唤醒空闲的线程，进而处理该事件。
3. 利用有限状态机实现了基本的http请求的解析。能够解析请求行，进而判断使用的方法(只支持get、post)、请求的文件路径、http版本;解析请求头，判断客户端主机名等信息。
4. 将服务器的echo功能、处理http请求功能、客户端的echo功能进行了分离，server类只用来创建socket、请求连接(客户端功能也应该分离，暂时不做)和接受连接的功能。处理事件的函数在主函数内实现，后期也可以封装成类。

## 遇到的问题：
1. 在进行多线程的代码编写时，认识到一个问题，不应该将各个类之间进行复合，而是应该将各个类设计为一个单独的模块。比如线程池类，内部实现的功能应该仅仅是创建多个线程，并且当把任务添加到线程池的任务队列时，线程池能分配给这个任务线程，进而执行这个任务。关键在于线程池不用管理进入队列的是什么任务，而仅仅去处理这个任务即可。
2. 通过mutex和condition variable进行线程间互斥和同步的管理，每当有新的任务被添加时，判断当前队列是否已满，否则任务队列加锁，使用一个线程处理该任务。
3. 在写解析http的类时，使用有限状态机进行请求行、请求体的顺序解析，要注意http请求的格式，在请求行中方法、url后均有空格‘ ’，协议后是回车符‘\r’和换行符‘\n’，请求体的每一行最后都有\r\n。

## 使用方法
运行 make，将生成server和client文件，分别运行./server 127.0.0.1 8888和./client 127.0.0.1 8888，客户端会发送http请求，如果解析成功，客户端会收到“ok”，否则收到“error”。