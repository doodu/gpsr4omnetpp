# Introduction #

模拟的数据最终在omnetpp.vec中，在这里面，我们要提取下面几个数据:

1.查询成功率

2.查询延迟

3。数据包转发次数

我们写了一个专门的脚本来做上面的事情，文件名为data\_process.py

# 说明 #
data\_process.py的输入是过滤了字符串的omnetpp.vec数据，像下面这样调用该例程：

cat omnetpp.vec | grep "^[0-9]" | ./data\_process.py

其输出为下面的格式:

[数据包转发次数］    ［查询延迟］     ［查询成功率］

# Details #

Add your content here.  Format your content with:
  * Text in **bold** or _italic_
  * Headings, paragraphs, and lists
  * Automatic links to other wiki pages