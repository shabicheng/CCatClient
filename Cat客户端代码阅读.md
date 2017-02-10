## 消息分类
1. 所有的消息都继承自Message
2. message提供的方法如下

```Java
/**
	 * add one or multiple key-value pairs to the message.
	 * 
	 * @param keyValuePairs
	 *           key-value pairs like 'a=1&b=2&...'
	 */
	public void addData(String keyValuePairs);

	/**
	 * add one key-value pair to the message.
	 * 
	 * @param key
	 * @param value
	 */
	public void addData(String key, Object value);

	/**
	 * Complete the message construction.
	 */
	public void complete();

	/**
	 * @return key value pairs data
	 */
	public Object getData();

	/**
	 * Message name.
	 * 
	 * @return message name
	 */
	public String getName();

	/**
	 * Get the message status.
	 * 
	 * @return message status. "0" means success, otherwise error code.
	 */
	public String getStatus();

	/**
	 * The time stamp the message was created.
	 * 
	 * @return message creation time stamp in milliseconds
	 */
	public long getTimestamp();

	/**
	 * Message type.
	 * 
	 * <p>
	 * Typical message types are:
	 * <ul>
	 * <li>URL: maps to one method of an action</li>
	 * <li>Service: maps to one method of service call</li>
	 * <li>Search: maps to one method of search call</li>
	 * <li>SQL: maps to one SQL statement</li>
	 * <li>Cache: maps to one cache access</li>
	 * <li>Error: maps to java.lang.Throwable (java.lang.Exception and java.lang.Error)</li>
	 * </ul>
	 * </p>
	 * 
	 * @return message type
	 */
	public String getType();

	/**
	 * If the complete() method was called or not.
	 * 
	 * @return true means the complete() method was called, false otherwise.
	 */
	public boolean isCompleted();

	/**
	 * @return
	 */
	public boolean isSuccess();

	/**
	 * Set the message status.
	 * 
	 * @param status
	 *           message status. "0" means success, otherwise error code.
	 */
	public void setStatus(String status);

	/**
	 * Set the message status with exception class name.
	 * 
	 * @param e
	 *           exception.
	 */
	public void setStatus(Throwable e);
```
3. 直接继承而未改动的方法有：Event，Heartbeat，Metric，Trace
4. Transaction继承自Message，相比而言多出addChild，getDurationInMicros，isStandalone等接口
5. Transaction同时又生出ForkedTransaction和TaggedTransaction，前者用于多线程间使用，后者用于RPC使用


## 消息数据结构
在各种Default*.java中实现
### Message
1. 属性有:type, name, status, timeStamp, data, completeFlag
2. 创建时提供type name
3. addData，keyvaluepair'&'链接到data后面
4. addData, key value以&key=value的方式链接到data后面
5. toString 直接使用PlainTextMessageCodec实现

### Event
1. 默认放到stack上面的tans的childs中
2. 如果stack上没有，则直接发送

### HeartBeat、Metric、Trace和Event一致

### Transaction
1. 增加属性：duration, children, standalone, durationStart
2. children链表实现
3. 默认是standalone
4. Complete多次调用也被认为是错误，重复调用时将添加一个Event("cat", "BadInstrument")
5. 正常设置Complete时，会调用manager的end，将消息发出去
6. 如果trans在Complete时还存在子的trans没有被complete，则认为子的trans全部出现错误，这一机制主要由messagemanager实现


## 消息管理
1. 接口主要有

```Java
public interface MessageManager {
	public void add(Message message);

	/**
	 * Be triggered when a transaction ends, whatever it's the root transaction or nested transaction. However, if it's
	 * the root transaction then it will be flushed to back-end CAT server asynchronously.
	 * <p>
	 * 
	 * @param transaction
	 */
	public void end(Transaction transaction);

	/**
	 * Get peek transaction for current thread.
	 * 
	 * @return peek transaction for current thread, null if no transaction there.
	 */
	public Transaction getPeekTransaction();

	/**
	 * Get thread local message information.
	 * 
	 * @return message tree, null means current thread is not setup correctly.
	 */
	public MessageTree getThreadLocalMessageTree();

	/**
	 * Check if the thread context is setup or not.
	 * 
	 * @return true if the thread context is setup, false otherwise
	 */
	public boolean hasContext();

	/**
	 * Check if current context logging is enabled or disabled.
	 * 
	 * @return true if current context is enabled
	 */
	public boolean isMessageEnabled();

	/**
	 * Check if CAT logging is enabled or disabled.
	 * 
	 * @return true if CAT is enabled
	 */
	public boolean isCatEnabled();

	/**
	 * Check if CAT trace mode is enabled or disabled.
	 * 
	 * @return true if CAT is trace mode
	 */
	public boolean isTraceMode();

	/**
	 * Do cleanup for current thread environment in order to release resources in thread local objects.
	 */
	public void reset();

	/**
	 * Set CAT trace mode.
	 * 
	 */
	public void setTraceMode(boolean traceMode);

	/**
	 * Do setup for current thread environment in order to prepare thread local objects.
	 */
	public void setup();

	/**
	 * Be triggered when a new transaction starts, whatever it's the root transaction or nested transaction.
	 * 
	 * @param transaction
	 * @param forked
	 */
	public void start(Transaction transaction, boolean forked);

	/**
	 * Binds the current message tree to the transaction tagged with <code>tag</code>.
	 * 
	 * @param tag
	 *           tag name of the tagged transaction
	 * @param title
	 *           title shown in the logview
	 */
	public void bind(String tag, String title);

	/**
	 * get domain
	 * 
	 */
	public String getDomain();

```
2. DefaultMessageManager是MessageManager的实现，主要包括ClientConfigManager， TransportManager，MessageIdFactory，ThreadLocal Context， TransactionHelper
3. 隶属于该模块的是ThreadLocal Context， TransactionHelper
4. 这里面先忽略Tagged和fock的trans逻辑，主要关注其他消息管理
5. MessageProducer在MessageManager之上封装出消息生成接口，实现是DefaultMessageProducer

### MessageIdFactory
1. 主要功能就是产生全局唯一的ID，多线程安全
2. 主要成员m_timestamp， m_index，  m_domain，m_ipAddress，m_byteBuffer，m_markFile，m_reusedIds
3. mark主要是记录当前ID保存的情况，保存两个字段：timestamp和index
4. 初始化时，设置domain，设置domain，拿到本机IP，创建markfile，读出上一次的timeStamp和index，如果是同一小时，则index加10000，否则index直接为0，保存mark
5.  getNextId为其主要逻辑，首先从reusedIds中检查是否存在ID，存在直接返回；否则生成ID，拿到当前时间，如果超过一小时，则index为0， index自加，注意，这里是原子的自加，然后根据“domain-ip-timestamp-index”的方式构建id并返回
6.  其实这里面前面的domain-ip是不变的，后面的timestamp一小时变一次，可以进行一些优化

### ThreadLocal Context
1. 每个线程单独维护的变量，最大的好处就是不用加锁，速度很快；而且监控本就针对单线程（Tagged和fock的trans除外），这样同一线程内的逻辑也比较好维护
2. Context在每个线程初次调用的时候会被初始化
3. Context内包含MessageTree, Stack tans, length, totalDuration, traceMode成员
4. 初始化时除了new一系列的对象外，主要对MessageTree进行初始化，设置thread, groupname, domain, hostname, ipaddress
5. addMessage，如果栈空，则直接发送，否则的话加到栈顶的那个trans下
6. addTransactionChild，将message加到trans的child中，加之前先判断length是否超过最大length，如果超过先将缓存的消息发送清空，然后在addChild
7. adjustForTruncatedTransaction，这个是和截断的逻辑一起使用的，即：在addTransaction时发现length太吵，则先将缓存的消息发送，这时候trans实际还没有结束，此时将m_totalDurationInMicros设为之前已执行的时间；在缓存清空（也可以说截断）后，trans完成，这时候检测到m_totalDurationInMicros不为0，执行此操作；此操作就是在trans后加上一个event("TruncatedTransaction", "TotalDuration")，用来向服务器告知，此trans执行的真正时间
8. end函数中首先检查栈是否为空，为空直接返回失败；随后对栈中内容进行逐个检查，如果这个trans不是栈顶的trans，先在栈上逐个pop找到这个trans，然后把栈上面的trans全部markAsNotCompleted；最后查看栈是否为空，为空则将消息flush
9. linkAsRunAway 先不讲
10. shouldLog检查exception在当前的hash中是否存在，存在返回false，不存在返回true，即exception只log一次
11. start时将trans加入到栈顶trans的后面，并入栈；如果栈为空，那就设置messagetree，然后入栈；fock不讲

### TransactionHelper
1. 主要就是几个函数，不包含实体
2. markAsNotCompleted就是在trans后加上一个event("TruncatedTransaction", "TotalDuration")，用来向服务器告知，此trans执行的真正时间
3. linkAsRunAway不讲
4. markAsRunAway不讲
5. migrateMessage，就是拷贝一个新的trans，这个trans维持栈上的结构，并将栈上trans中的childmessage（除了trans）移到新的trans中
6. truncateAndFlush步骤如下：messagetree上拿到ID并构造childID，使用migrateMessage构造新的trans，将原来栈上的trans时间重设，在新的trans上加入一个event("RemoteCall", "Next")并把childID加上，用来标识这个trans其实还没完，构造新的tree，将原来tree上的ID更新，把ID设为childID，parentID设为原来的ID，rootID如果为空则设为原来的ID，更新m_length为栈大小，更新m_totalDurationInMicros为m_totalDurationInMicros+target.getDurationInMicros()，最后发送新的messagetree
7. validate抛开fock和tag不谈，就是递归判断trans的各个child trans是否已经完成

### MessageManager
1. 介绍完MessageManager的左右护法之后再看MessageManager就比较简单了
2. initialize，拿到domain，host，ip
3. addMessage，直接调用context的addMessage实现，注意这个Message是非trans的Msg
4. bind不讲
5. end，针对trans，调用context的end，返回true即trans已经完成时，remove context
6. flush，针对tree，拿sender，判断sender是否可发，如果可发就发送，然后reset，否则++m_throttleTimes并记录日志
7. getContext，判断是否为null，为null则new再返回
8. reset，如果m_totalDurationInMicros为0，说明不是未完成（截断）的trans，可以直接remove掉context，否则只clear掉exception
9. start，针对trans，调用context的start

### MessageProducer
1. MessageProducer主要是封装MessageManager并提供消息产生的接口
2. logError，检查是否需要Log，将message加到字符串上，打印stack，判断cause类型，如果是Error则logEvent类型为Error，RuntimeException则为RuntimeException，否则为Exception
3. logEvent， 创建一个event，设置type，name, nameValuePairs, setStatus, complete
4. logHeartbeat与logEvent类似
5. logMetric除了type为空，其他域logEvent类似
6. logTrace，判断是否处于trace模式，如果是则发trace，否则过滤
7. newTransaction，创建后调用manager的start方法
8. 

## 消息编解码
1. MessageCodec接口，主要decode，encode
2. PlainTextMessageCodec实现MessageCodec接口，提供文本方式的消息解包打包
3. PlainTextMessageCodec里面有个Context，主要为codec线程提供一段内存用于编解码，默认4MB
4. 主要看下encode，decode客户端用不到，其实就是反着来，encode通了decode自然而然通了


### Encode
1. encode首先保留四字节头部，用来填写msg大小
2. encode过程是保留四字节，encodeHeader，encodeMsg，set保留字节为后面报文的大小
3. encodeHeader类型为:
 `version\tdomain\thostname\tipAddress\tthreadGroup\tthreadId\tthreadName\tmessageId\tparentMsgId\trootMsgId\tsessionToken\n`
4. encodeMessage首先判断msg类型，如果非trans，则直接调用相应的encodeLine；如果是Transaction，判断trans内部有无children，如果无，则调用相应的encodeLine，否则，首先encodeLine，然后递归调用children的encodeMsg，最后调用encode，相当于用前后用两个把他们封起来
5. encodeLine，write type， 判断是否为trans且type==T，如果是则需要加上duration，随后格式为`\tmsgType\tmsgName\t`，然后判断policy，如果不是WITHOUT_STATUS，则write status，write \t， 如果msg是trans且WITH_DURATION，则write duration, write us, write \t，然后write data,write \t，最后write \n
6. policy策略为：非trans则为default，trans结尾为WITH_DURATION，trans若包含child，则第一次encode的为WITHOUT_STATUS；也就是说，则第一次encode的为WITHOUT_STATUS不需要额外加东西，DEFAULT下需要加上status、data，WITH_DURATION需要在DEFAULT之间再加上一个duration



## 网络管理
1. 网络部分主要分为两块：可用连接管理和当前连接管理
2. 代码逻辑DefaultTransportManager-> TcpSocketSender-> ChannelManager
3. ChannelManager, 可用连接管理，定期（60秒）检查可用连接，如果发现失效则重连
4. TcpSocketSender，维持着队列，采用异步发送方式，发送的时候从ChannelManager中拿可用连接来发
5. DefaultTransportManager主要是封装以及初始化工作
6. 这里面主要将以下TcpSocketSender和ChannelManager

### TcpSocketSender
1. 负责队列维护，合并消息，消息打包以及消息发送
2. 内部有两个队列，一个是发送队列，一个原子Tree队列，后者主要用于合并
3. send接口，异步过程，判断消息是否为AtomicMessage，如果是，则将其插入atomicTrees队列，否则插入queue
4. run函数，两部分功能：正常情况下，从队列中拉出tree，调用sendInternal并发送；channel失效时，查看queue中消息是否超过1小时，超出部分全部删掉并记录
5. sendInternal主要工作就是打包、发送、统计
6. shouldMerget，当queue的size > MAX_CHILD_NUMBER或当前时间与queue第一个message时间超过30s时返回true
7. MergeAtomicTask，单独线程，主要做的工作就是Merge这个m_atomicTrees，每5ms执行一次，判断shouldMerget，为true则调用mergeTree方法，并把tree加入queue队列
8. isAtomicMessage判断messagetree上的message类型，只有非“Cache”“SQL”的trans返回false，其他都为true
9. mergeTree，创建一个DefaultTransaction("_CatMergeTree", "_CatMergeTree", null)，从m_atomicTrees中不停poll出message并加入到新的trans上，然后在第一个MessageTree上连接这个trans；注意这里除了第一个连上trans的tree，其他tree中的messageid都不会发往服务器，这时候这个ID就可以被重新使用了；
10. *mergeTree这段代码中，主要有两个问题：1， merge的时候，while循环中只有tree==NULL才设置Duration，max为0退出循环时并不设，这会有问题；2，reuse的ID虽说不会重复，但有可能ID值低于前面已经被发送的ID，不知道服务器这边会不会有影响，如果服务器只需确定ID唯一，则无影响，若要求ID递增则有影响*

### ChannelManager
1. 主要成员Bootstrap，MessageQueue， ChannelHolder，MessageIdFactory
2. 初始化时，创建NioEventLoopGroup，从服务器拿到router，初始化ChannelHolder
3. checkServerChanged， 定期从服务器拿router，与当前router比较，如果改变，重新初始化Channel，如果isConnectChanged，则关闭之前的连接
4. createChannel，调用connect方法，只等100ms
5. doubleCheckActiveServer，判断channelFuture是否stalled以及是否被disabled，stalled主要判断队列是否阻塞，disable判断连接是否open
6. initChannel，从给的连接中找到一个可用连接并返回；如果可用连接与原来的连接一致，则setConnectChanged(false)否则setConnectChanged(true)；如果没有可用连接，打印错误并返回失败
7. loadServerConfig，通过URL向服务器发起http请求，并拿到配置的router
8. reconnectDefaultServer，先判断是否存在activeServer，然后从index 0开始连接server，成功则关闭原来的连接并返回
9. run，主要做的事情有：saveMark，checkServerChanged，doubleCheckActiveServer，reconnectDefaultServer；对于连接主要做的事情是：查看config的router是否变化，如果变化则初始化连接，获取当前连接，判断连接是否有效，无效则删除连接，如果连接失效，则从连接中找到一个可用连接，如果连接有效，则看在有效连接所在的index之前是否有新的连接，有就连上去
10. *reconnectDefaultServer意思是永远把前面的连接作为优先考虑的连接，这里面为什么不随机选服务器，至少可以负载均衡啊*


