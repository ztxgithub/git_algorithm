# 队列

## 基础知识

```shell
    1. 入队 enqueue(): 向队尾插入数据
    　 出队 dequeue(): 从对头取一个元素
    2. 顺序队列:  用数组实现
       链式队列： 用链表实现　
    3. 对于大部分资源有限的场景，当没有空闲资源时，基本上都可以通过“队列”来实现请求排队。
```

## 队列的分类

```shell
    1.循环队列: 循环队列中需要考虑队列空和满的条件, 例如设队列头的下标索引为 head, 队列尾下标索引为 tail,
               则队列为空的条件是(tail == head), 队列为慢的条件是((tail + 1) % queue.size == head)
    2. 阻塞队列: 相当于消费者，生存者模型，　当队列中元素为空时，要从队列中取阻塞等待．　当队列中已满，往
    　　　　　　　队列中添加元素，阻塞等待．
    3. 无锁队列:
                (1) CAS等原子操作: CAS(Compare And Swap): 对应的是看看内存里的值是不是　oldval，如果是的话，则对其赋值newval
                　　　　　　　　　　　　　　　　　　　　　　　　这些操作是原子性的，要么都成功执行，要么都不执行．
                　　　　　　　　　　如果内存中的值在同一时间被其他线程更新了，那么这个时候检测传入的旧值跟现在内存中的值不相等，
                　　　　　　　　　　则更新失败． CAS 操作返回值指明该操作是否成功
                
                　　　GCC4.1+版本中支持CAS的原子操作：
                        bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval, ...)
                        type __sync_val_compare_and_swap (type *ptr, type oldval, type newval, ...)
                        
                (2) 用 CAS 实现进队列
                
                        version 1:
                        EnQueue(x) //进队列
                        {
                            //准备新加入的结点数据
                            q = new record();
                            q->value = x;
                            q->next = NULL;
                         
                            do {
                                p = tail; //取链表尾指针的快照
                            } while( CAS(p->next, NULL, q) != TRUE); //如果没有把结点链在尾指针上，再试,可能其他线程 CAS 操作成功
                         
                            CAS(tail, p, q); //置尾结点，这个时候不需要 while 来重复判断是否成功，因为一定成功，其他
                            　　　　　　　　　　// 在前面已经成功将 tail->next 替换为 q 后，其他线程可以　CAS　返回值肯定为 false
                        }
                        
                        version 1 有一个 bug, 就是用　CAS(tail, p, q)　更新　tail 指针的之前，线程停掉或是挂掉了，
                        那么其它线程就进入死循环了
                        
                        version 2 :
                            EnQueue(x) //进队列改良版
                            {
                                q = new record();
                                q->value = x;
                                q->next = NULL;
                             
                                p = tail;
                                oldp = p
                                do {
                                    while (p->next != NULL)
                                        p = p->next;
                                } while( CAS(p.next, NULL, q) != TRUE); //如果没有把结点链在尾上，再试
                             
                                CAS(tail, oldp, q); //置尾结点
                            }
                            
                (3) CAS 存在的 ABA 问题
                        ABA 问题描述:
                        进程P1在共享变量中读到值为A
                            第一步: P1 被抢占了，进程 P2 执行
                            第二步: P2把共享变量里的值从 A 改成 B，再改回到 A (其中 A, B 可以是指针地址)，此时被 P1 抢占。
                            第三步: P1 回来看到共享变量里的值没有被改变，于是继续执行。
                            
                        ABA 问题很容易发生在 lock free(无锁)算法中，因为 CAS 判断的是指针的地址,如果这个地址被重用了呢,
                        虽然指针没有发生改变，但指针对应的内容确改变了，已经不是原来的内容了．
                        
                (4) CAS 的 ABA 问题解决方案
                        针对操作的数据加上一个原子操作的使用计数，在CAS执行之前，先获取一下计数是否和之前一样，如果不一样，
                        就说明数据已经被修改过了。
```

