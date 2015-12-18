# Introduction #

网络的存储

# Details #

因为gpsr路由的需要，在网络引导的时候节点会互发beacon包，目的是让个各节点得到自已的邻节点的信息，以建立路由表。但是当网络比较大的时候，这个过程会很长很长，所以有必要把各个节点的路由表保存起来，下次启动网络的时候直接加载路由表。
为了保存网络，我们定义了一种文件格式，如下：

myNetwAddr

neghbore1:x:y

neghbore2:x:y

....


%%

myNetwAddr

neghbore1:x:y

neghbore2:x:y

....

这是一种分节的文件格式(它来自于<<The Art Of Unix Programming>>，我们先择尽量简单的方式来存储数据，关于扩展性暂不考虑)，每一节的内容存储了一个节点的信息，各节之间用"%%\n"分割。每节的第一个字段是节点的网络层地址，后面跟的是路由表，每行存路由表中一个节点的信息，格式（netwaddr:x:y）其中netwaddr是网络层的地址，x,y是他的坐标.

为此，我们新建了一个模块用来支持这种这文件的读写,模块对应的源文件为NetworkFile.h NetworkFile.cc


Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages