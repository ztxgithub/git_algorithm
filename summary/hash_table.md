# 散列表( hash 表或则 哈希表)

## 基础知识

```shell
    1. 散列表是数组的扩展，用的是数组随机访问的特性，如果没有数组，就没有散列表
    2. 散列思想:
            参赛选手的编号叫作键（key）或者关键字(用它来标识一个选手),
            把参赛编号转化为数组下标的映射方法就叫作散列函数(“Hash 函数”,“哈希函数”)，
            而散列函数计算得到的值就叫作散列值（“Hash 值”,“哈希值”）
    3. 散列表用的是数组支持按下标随机访问其时间复杂度是 O(1) 的特性。通过散列函数把元素的键值映射为下标，然后将数据保存在对应下标中。
    　 要根据键值查询元素时，用同样的散列函数，将键值转化数组下标，从对应的数组下标中取数据。
    
    4. 装载因子(load factor) : 填入表中的元素个数 / 散列表的总长度, 装载因子越大，说明空闲位置越少，冲突越多，散列表的性能会下降
    5. 散列表碰撞攻击: 通过构造数据，使得这些数据经过散列函数后都在同一个槽内，使之退化为单链表，查询的效率下降到 n 倍，
    　　　　　　　　　　如果散列表中有 10 万个数据，退化后的散列表查询的效率就下降了 10 万倍。更直接点说，
    　　　　　　　　　　如果之前运行 100 次查询只需要 0.1 秒，那现在就需要 1 万秒。这样就有可能因为查询操作消耗大量 CPU 或者线程资源，
    　　              导致系统无法响应其他请求，从而达到拒绝服务攻击（DoS）的目的
            
```

## 散列函数

```shell
    1. 散列函数设计的基本要求
        (1) 散列函数计算得到的散列值是一个非负整数, 因为数组下标是从 0 开始的
        (2) 如果 key1 = key2，那 hash(key1) == hash(key2)；
        (3) 如果 key1 ≠ key2，最好 hash(key1) ≠ hash(key2)，散列函数可以会存在冲突，
        　　要想找到一个不同的 key 对应的散列值都不一样的散列函数，几乎是不可能的，像 MD5、SHA、CRC等哈希算法，
            也无法完全避免这种散列冲突。而且因为数组的存储空间有限，也会加大散列冲突的概率。
    2. 散列函数设计的好坏，决定散列表冲突的概率大小，也直接决定散列表的性能
            (1) 散列函数的设计不能太复杂, 太复杂会消耗很多计算时间，也间接影响到散列表的性能
            (2) 散列函数生成的值要尽可能随机并且均匀分布, 
                    随机是避免或者最小化散列冲突，均匀分布是出现冲突，散列到每个槽里的数据平均分布，不会出现某个槽内数据特别多的情况。
    3. 散列函数设计方法
            (1) 数据分析法, 例如手机号码前几位重复的可能性很大，但是后面几位就比较随机，我们可以取手机号的后四位作为散列值
            
```

## 装载因子过大

```shell
    1. 装载因子过大,会影响到插入查找的效率, 这时候需要动态扩容．
    2. 避免低效地扩容
            为了解决一次性扩容耗时过多的情况(其操作是先申请 2 倍空间于原来容量，在重新计算哈希值，将原来的散列表中数据搬到新的散列表中)，
            当装载因子到达阈值后，只申请新空间，但并不将老的数据搬移到新散列表中。当有新数据要插入时，将新数据插入新散列表中，
            并且从老的散列表中拿出一个数据放入到新散列表。每次插入一个数据到散列表，我们都重复上面的过程。
            经过多次插入操作之后，老的散列表中的数据就一点一点全部搬移到新散列表中了。这样没有了集中的一次性数据搬移，
            插入操作就都变得很快。在这期间如果要查询数据，则先从新散列表中查找，如果没有找到，再从老的散列表中查找。
```

## 解决散列冲突

```shell
    1.开放寻址法 (open addressing)
            (1) 优点:
                    开放寻址法中散列表数据都是保存在数组中，可以有效地利用 CPU 缓存加快查询速度，同时要比链表法更方便序列化
            (2) 缺点:
                    删除数据的时候比较麻烦，需要特殊标记已经删除掉的数据。而且所有的数据都存储在一个数组中，
                    比链表法来说，冲突的代价更高。
            (3) 当数据量比较小、装载因子小的时候，适合采用开放寻址法。
                 这也是 Java 中的 ThreadLocalMap 使用开放寻址法解决散列冲突的原因。
            方案一: 线性探测(Linear Probing)
                        插入操作:
                              在散列表中插入数据时，键值经过散列函数散列之后得到的哈希值(数组下标)已经被占用了，就从当前位置开始，
                              依次往后查找(步长为 1 )，看是否有空闲位置，直到找到为止。
                        查找操作:
                                通过散列函数求出要查找元素的键值对应的散列值，然后比较数组中下标为散列值的元素和要查找的元素。
                                如果相等，则说明就是我们要找的元素；否则就顺序往后依次查找(步长为 1)。
                                如果遍历到数组中的空闲位置，还没有找到，就说明要查找的元素并没有在散列表中。
                        删除操作:
                               将删除的元素，特殊标记为 deleted。当线性探测查找的时候，遇到标记为 deleted 的空间，
                               并不是停下来，而是继续往下探测(特殊标记为 deleted　这样是为了防止删除操作只是简单把元素置空，在查找时
                               刚好找到这个原来的位置(已被置空)，剩下就不继续查找)
                        
                        缺点: 
                              当散列表中插入的数据越来越多时，散列冲突发生的可能性就会越来越大，空闲位置会越来越少，
                              线性探测的时间就会越来越久。极端情况下，我们可能需要探测整个散列表，所以最坏情况下的时间复杂度为 O(n)。
                              同理，在删除和查找时，也有可能会线性探测整张散列表，才能找到要查找或者删除的数据。
            方案二: 二次探测(Quadratic probing)
                        冲突时探测步长为线性的二次方，例如: 
                        数组下标序列就是 hash(key) + 0, hash(key) + 1 ^ 2, hash(key) + 2 ^ 2, hash(key) + 3 ^ 2
            方案三: 双重散列(Double hashing)
                        使用一组散列函数 hash1(key)，hash2(key)，hash3(key)……
                        先用第一个散列函数，如果计算得到的存储位置(哈希值或则数组下标)已经被占用，再用第二个散列函数，
                        依次类推，直到找到空闲的存储位置
    2. 链表法
           在散列表中，每个“桶(bucket)”或者“槽(slot)”会对应一条链表，所有散列值相同的元素都放到相同槽位对应的链表中。
           当插入的时候，只需要通过散列函数计算出对应的散列槽位(数组下标)，将其插入到对应链表中即可，所以插入的时间复杂度是 O(1)。
           查找和删除也一样，先计算出哈希值，找到对应的桶，在该桶对应的链表中查找对应的元素
           
           (1) 优点:
                    链表法对内存的利用率比开放寻址法要高,链表法比开放寻址法其对应装载因子容忍度更高，只要散列函数的值随机均匀，
                    则查找时要比顺序查找效率高得多．
           (2) 缺点:
                    链表中的结点是零散分布在内存中的，不是连续的，对于执行效率而言，链表法要比开放寻址法执行效率低.
                    因为链表是要多余的空间来占用指针，所以对于比较小的对象而言，内存会消耗翻倍．
                    如果我们存储的是大对象，要存储的对象的大小远远大于一个指针的大小，那链表中指针的内存消耗在大对象面前就可以忽略
           (3) 其中链表可以用跳表、红黑树等动态数据结构替换，即使出现散列冲突，极端情况下，所有的数据都散列到同一个桶内(桶内有原先链表改为红黑树)，
           　　那最终退化成的散列表的查找时间也只不过是 O(logn)。
           (4) 链表法比较适合存储大对象、大数据量的散列表，而且，比起开放寻址法，它更加灵活，支持更多的优化策略，比如用红黑树代替链表。
            
```

## 设计散列表

```shell
    1. 设计一个合适的散列函数；
    2. 定义装载因子阈值，并且设计动态扩容策略；
    3. 选择合适的散列冲突解决方法
```

## 散列表和链表的联合使用

```shell
    1. LRU 缓存淘汰策略
            (1) 低效率做法:
                    维护一个按照访问时间从大到小有序排列的链表。当缓存空间不够，需要淘汰一个数据时，直接将链表头部的结点删除。
                    当要缓存(插入)某个数据的时候，先在链表中查找这个数据。如果没有找到，则直接将数据放到链表的尾部；
                    如果找到，把它移动到链表的尾部。
                    因为查找数据需要遍历链表，所以单纯用链表实现的 LRU 缓存淘汰算法的时间复杂很高，是 O(n)
            (2) 高效率算法:
                    将散列表和链表两种数据结构组合使用,链表是采用双向链表，链表中的每个结点处理存储数据(data)、前驱指针(prev)、
                    后继指针（next）之外，还新增 hash_next 指针, 因为散列表是通过链表法解决散列冲突，所以每个结点会在两条链中。
                    一个链是双向链表，另一个链是散列表中的拉链。前驱和后继指针是为了将结点串在双向链表中，hash_next 指针是
                    将结点串在散列表的拉链中。在散列表的构建中其对应数组下标可以用哈希值 % n(数组大小)　来确定
                    
                    查找操作:
                            散列表中查找数据的时间复杂度接近 O(1)，所以通过散列表，可以很快地在缓存中找到一个数据。
                            当找到数据之后，还需要将它移动到双向链表的尾部。
                    删除操作:
                            通过散列表，可以在 O(1) 时间复杂度里找到要删除的结点。
                            双向链表可以通过前驱指针 O(1) 时间复杂度获取前驱结点，所以在双向链表中，
                            删除结点只需要 O(1) 的时间复杂度。
                    插入操作:
                            需要先看这个数据是否已经在缓存中。如果已经在其中，需要将其移动到双向链表的尾部；
                            如果不在其中，还要看缓存有没有满。如果满了，则将双向链表头部的结点删除，然后再将数据放到链表的尾部；
                            如果没有满，就直接将数据放到链表的尾部。 同时注意维护节点中 hash_next 的值
    2. java 中的 LinkedHashMap 也是通过散列表和链表组合在一起实现的，
        (1) 支持按照插入顺序遍历数据，
            HashMap<Integer, Integer> m = new LinkedHashMap<>();
            m.put(3, 11);
            m.put(1, 12);
            m.put(5, 23);
            m.put(2, 22);
             
            for (Map.Entry e : m.entrySet()) {
              System.out.println(e.getKey());
            }
            结果打印的是 3,1,5,2
        (2) 支持按照访问顺序来遍历数据
            // 10 是初始大小，0.75 是装载因子，true 是表示按照访问时间排序
            HashMap<Integer, Integer> m = new LinkedHashMap<>(10, 0.75f, true);
            m.put(3, 11);
            m.put(1, 12);
            m.put(5, 23);
            m.put(2, 22);
             
            m.put(3, 26);
            m.get(5);
             
            for (Map.Entry e : m.entrySet()) {
              System.out.println(e.getKey());
            }
            结果打印的是 1,2,3,5
            
            LinkHashMap 每次调用 put() 函数，往 LinkedHashMap 中添加数据时，都会将数据添加到链表的尾部
        (3) LinkedHashMap 是通过双向链表和散列表这两种数据结构组合实现的。LinkedHashMap 中的“Linked”实际上是指的是双向链表，
            并非指用链表法解决散列冲突。
            
    3. 散列表这种数据结构虽然支持非常高效的数据插入、删除、查找操作，但是散列表中的数据都是通过散列函数打乱之后无规律存储的。也就说，
       它无法支持按照某种顺序快速地遍历数据,如果想要支持，则必须配合使用双向链表
```

## 应用

```shell
    1. 实现 Word 文档中的单词拼写检查功能(不具备语法功能)
            用散列表来存储整个英文单词词典,再将输入的单词通过对应的哈希算法，看是否找到，找到则拼写正确，否则拼写错误.其中散列函数
            设计可以是将单词中每个字母的 ASCll 码值“进位”相加，然后再跟散列表的大小取模，作为散列值.例如
            　hash("nice")=(("n" - "a") * 26*26*26 + ("i" - "a")*26*26 + ("c" - "a")*26+ ("e"-"a")) % hash_size

    2. Java 中的 HashMap 散列表分析
            (1) HashMap 默认的初始大小是 16，如果事先知道大概的数据量有多大，可以通过修改默认初始大小，减少动态扩容的次数，
                提高 HashMap 的性能
            (2) 装载因子和动态扩容, 最大装载因子默认是 0.75，当 HashMap 中元素个数超过 0.75 * capacity(capacity 表示散列表的容量)
                就会启动扩容，每次扩容都会扩容为原来的两倍
            (3) 散列冲突解决方法, HashMap 底层采用链表法来解决冲突,当冲突发送时其同一个桶的链表长度过长(默认超过 8), 
               利用红黑树快速增删改查的特点，提高 HashMap 的性能。当红黑树结点个数少于 8 个的时候，又会将红黑树转化为链表。
               在数据量较小的情况下，红黑树要维护平衡，比起链表来，性能上的优势并不明显。
            (4) 散列函数
                    hashcode 的随机性，加上移位异或算法，得到一个非常随机的hash值，再通过「除留余数法」
                    
                    int hash(Object key) {
                        int h = key.hashCode()；
                        return (h ^ (h >>> 16)) & (capitity -1); //capicity 表示散列表的大小 A % B = A & (B - 1)
                    }
                    
                    public int hashCode() {
                      int var1 = this.hash;
                      if(var1 == 0 && this.value.length > 0) {
                        char[] var2 = this.value;
                        for(int var3 = 0; var3 < this.value.length; ++var3) {
                          var1 = 31 * var1 + var2[var3];
                        }
                        this.hash = var1;
                      }
                      return var1;
                    }
    3. python的字典 dict 结构也是用 散列表
                    
```

# 哈希算法

## 概要

```shell
    1.哈希算法 : 将任意长度的二进制值串映射为固定长度的二进制值串的映射规则
      哈希值: 通过原始数据映射之后得到的固定二进制值串
    2. 优秀的哈希算法的设计特点:
            (1) 从哈希值不能反向推导出原始数据(哈希算法也叫单向哈希算法) 
            (2) 对输入数据敏感，原始数据只修改一个 Bit，最后得到的哈希值也要不相同；
            (3) 散列冲突的概率要很小，对于不同的原始数据，哈希值相同的概率非常小；
            (4) 哈希算法的执行效率要尽量高效，针对较长的文本，也能快速地计算出哈希值。
```

## 哈希算法应用

```shell
    1. 安全加密
            (1) 最常用于加密的哈希算法是 MD5(MD5 Message-Digest Algorithm，MD5 消息摘要算法)和 
                SHA(Secure Hash Algorithm，安全散列算法)
            (2) 对用于加密的哈希算法来说，有两点格外重要。第一点是很难根据哈希值反向推导出原始数据，第二点是散列冲突的概率要很小
            (3) 哈希算法无法做到零冲突
                    哈希算法产生的哈希值的长度是固定且有限的。比如 MD5，哈希值是固定的 128 位二进制串(16 字节)，能表示的数据是有限的，
                    最多能表示 2 ^ 128 个数据，而我们要哈希的数据是无穷的。基于鸽巢原理(10 个巢, 11 个鸽子，总有 1 个巢有 2 个鸽子)，
                    如果我们对 2 ^ 128 + 1 个数据求哈希值，就必然会存在哈希值相同的情况。 哈希值越长的哈希算法，散列冲突的概率越低
            (4) 越复杂、越难破解的加密算法，需要的计算时间也越长。比如 SHA-256 比 SHA-1 要更复杂、更安全，相应的计算时间就会比较长
    2. 唯一标识
            在海量的图库中快速查找图片
                第一步: 给每一个图片取一个唯一标识(信息摘要),具体的做法可以是从图片的二进制码串开头取 100 个字节，从中间取 100 个字节，
                       从最后再取 100 个字节，然后将这 300 个字节放到一块，通过哈希算法(比如 MD5)，得到一个哈希字符串，
                       用它作为图片的唯一标识.
                第二步: 把每个图片的唯一标识，和相应的图片文件的路径信息，都存储在散列表中。当要查看某个图片是不是在图库中时，
                　　　　先通过哈希算法对这个图片取唯一标识，然后在散列表中查找是否存在这个唯一标识。
                     　如果不存在，那就说明这个图片不在图库中；如果存在，再通过散列表中存储的文件路径，获取到这个已经存在的图片，
                       跟现在要插入的图片做全量的比对，看是否完全一样。如果一样，就说明已经存在；
                       如果不一样，说明两张图片尽管唯一标识相同，但是并不是相同的图片。
    3. 数据校验
            校验文件块的安全、正确、完整(BT 种子下载)
                通过哈希算法，对 100 个文件块分别取哈希值，并且保存在种子文件中。哈希算法有一个特点，对数据很敏感。
                只要文件块的内容有一丁点儿的改变，最后计算出的哈希值就会完全不同。当文件块下载完成之后，通过相同的哈希算法，
                对下载好的文件块逐一求哈希值，然后跟种子文件中保存的哈希值比对。如果不同，说明这个文件块不完整或者被篡改了，
                需要再重新从其他宿主机器上下载这个文件块。
    4. 散列函数
            散列函数也是哈希算法的一种应用
            散列函数关心点:
                    (1) 散列后的值是否能平均分布(一组数据是否能均匀地散列在各个槽中)
                    (2) 散列函数执行的快慢
            散列函数不太关注点:
                    (1) 散列算法冲突的要求: 即便出现个别散列冲突，只要不是过于严重，都可以通过开放寻址法或者链表法解决。
                    (2) 对于散列算法计算得到的值，是否能反向解密
    5. 负载均衡
            (1) 会话粘滞(session sticky)的负载均衡算法: 在同一个客户端上，一次会话(session)中的所有请求都路由到同一个服务器上。
            (2) 低效的做法:
                    维护一张映射关系表，这张表的内容是客户端 IP 地址或者会话 ID 与服务器编号的映射关系。客户端发出的每次请求，
                    都要先在映射表中查找对应的服务器编号，然后再请求编号对应的服务器.
                    缺点:
                        I.  如果客户端很多,映射表可能会很大,比较浪费内存空间；
                        II. 客户端下线、上线，服务器扩容、缩容都会导致映射失效，其维护映射表的成本很大
            (3) 高效的做法:
                    通过哈希算法，对客户端 IP 地址或者会话 ID 计算哈希值，将取得的哈希值与服务器列表的大小进行取模运算，
                    最终得到的值就是应该被路由到的服务器编号, 因为这样就不用维护映射表，而如果会话 ID, 不同其对应的哈希值
                    大概率也会不同且唯一．
    6. 数据分片
            (1) 在一个数据量很大的日志文件中，需要统计各个关键字的引用次数，需要用多台机器共同协同处理，这时就要进行数据分片，某个
            　　　关键字只能有某一台机器统计，那么具体的做法就是将关键字进行哈希算法后，在跟机器个数 n 取模，这样关键字数据就能
            　　　唯一由某个机器进行统计了，增加统计的效率
            (2) 快速判断图片是否在图库(1 亿张图)
                单机上的方法是给每个图片取唯一标识（或者信息摘要），然后构建散列表。在单台机器上构建散列表是行不通的。
                因为单台机器的内存有限，而 1 亿张图片构建散列表显然远远超过了单台机器的内存上限。
                
                可以对数据进行分片，然后采用多机处理。准备 n 台机器，让每台机器只维护某一部分图片对应的散列表。
                从图库中读取一个图片，计算唯一标识，然后与机器个数 n 取模，得到的值就对应要分配的机器编号，
                然后将这个图片的唯一标识和图片路径发往对应的机器构建散列表。
                如果想要查询图片是否在图库中，则先将图片通过哈希算法得出唯一标识，再与 n 取模得到机器编号，再在该机器中进行
                查找
    7. 分布式存储
            前提: 在前面数据分片的思想(通过哈希算法对数据取哈希值，然后对机器个数取模，这个最终值就是应该存储的缓存机器编号)
                  如果在数据分片好了，运行过程中数据容量还是太大需要服务器扩容，那么这个时候所有的数据需要重新分片(哈希值与机器数取模)
                  所有的数据请求都会穿透缓存，直接去请求数据库
            解决方法: 一致性哈希算法,假设有 k 个机器，数据的哈希值的范围是 [0, MAX],将整个范围划分成 m 个小区间(m 远大于 k),
                    每个机器负责 m/k 个小区间。当有新机器加入时，就将某几个小区间的数据，从原来的机器中搬移到新的机器中。
                    这样，既不用全部重新哈希、搬移数据，也保持了各个机器上数据数量的均衡。

```

## 实战

```shell
    1.防止数据库中的用户信息被脱库
            (1) 第一：将密码通过哈希算法进行加密，最好选择相对安全的加密算法(例如 SHA),这样即使数据库被泄露拿到只是加过密的密码，
            　　　　　　不过要防止字典攻击(在常用的密码字典中，把字典中的每个密码用哈希算法计算哈希值，然后拿哈希值跟脱库后的密文比对)
            (2) 第二: 针对字典攻击，可以加盐，与用户的密码组合在一起，再进行哈希算法加密，将它存储到数据库中
    2. 区块链
            区块链是多个区块组成的，每个区块包含区块头和区块体. 区块头保存自己区块体的哈希值和上一个区块头的哈希值。
            因为这种链式关系和哈希值的唯一性，只要区块链上任意一个区块被修改过，后面所有区块保存的哈希值就不对了。
            区块链使用的哈希算法是 SHA256 哈希算法
```