# MyTest
一个关于多客户端通讯的小项目

在MS和MC目录下的Release下面有可执行的exe文件，使用格式如下
服务端 ms-lite.exe --port 1998 --netlog D:\net.log
客户端 mc-lite.exe --name alice
客户端进入后，可以使用如下命令：
The client should support the following commands:
Connect to the server: > HI <server-identifier>
Send message to a MC, like: > @bob hello bob
Broadcast message to all the MC's, like: > @all hello all
Disconnect to the server: > BYE
Quit the client: > QUIT
下面是一个例子：
example but design your own as you wish.
C:\> mc-lite --name alice # execute the client
alice> HI 127.0.0.1:12345 # connect
127.0.0.1:12345|alice > @bob hi bob # send message to bob
2021/06/04 15:35:29 alice: hi bob # echo of the sent message
127.0.0.1:12345|alice >
2021/06/04 15:35:30 bob: hi alice # if got a reply from bob
127.0.0.1:12345|alice >
2021/06/04 15:35:40 tom: hi alice, I'm tom # if got a message from tom
127.0.0.1:12345|alice > @all hi bob
2021/06/04 15:35:50 broadcast from alice: hi all # broadcast
127.0.0.1:12345|alice >
2021/06/04 15:35:53 broadcast from tom: hi all # tom broadcasted
127.0.0.1:12345|alice > BYE # bye
C:\>

其他需求由于时间因素，部分满足：
You could choose any communication mechanism like socket / pipe / ... in this challenge, but
please consider the possibility of its change in future.---支持
The MS should keep listening the CONNECT requests from MC's and is required to launch a
handler for each connected MC. It's your freedom of how to implement the handlers, like
determinations of choosing either threads or processes. In this challenge, it's the handlers
who're expected to manage the clients. If a MC closed or lost connection, its handler should
terminate as well. Furthermore, the handler should be able to detect whether its client is
alive and terminates if not.---支持，但未加心跳以及完善的异常捕获
The message can be multiple lines with \ at the line ending.---不支持
(Optional) registration. When an MC connects to a MS for the first time, it registers itself in
the MS. The MS would prompt to create a password for registration and will required the
password when the MC connects to the MS In future. The MS would remember all the---部分支持，支持REGISTER,但由于密码沟通过程繁琐，未添加
registered MC even it is restarted.---未理解
(Optional) offline message. If an message is sent to an registered user who is offline, the
server should be able to resend the message to the user once it becomes online.---部分支持，使用了消息队列，可扩展
You could add any other features as you think it's useful.---加入了服务器端参数处理，自定义接口和网络消息的日志。

