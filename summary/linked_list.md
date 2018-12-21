# 链表

## 基础知识

```shell
    1. 缓存淘汰策略
            (1) 先进先出策略 FIFO（First In，First Out）
            (2) 最少使用策略 LFU（Least Frequently Used）
            (3) 最近最少使用策略 LRU（Least Recently Used）
    
```

## 链表方面

```shell
    1. 单向链表和双向链表的区别
           (1) 在删除节点上，如果已经找到对应的节点，需要删除这个节点还需要得到前驱节点，而对于单向链表而言需要从头开始
               遍历才能找到该节点的前驱节点(时间复杂度为 O(n))，而双向链表可以直接得到前驱节点(时间复杂度 O(1))
           (2) 对于一个有序链表，双向链表的按值查询的效率也要比单链表高一些。可以记录上次查找的位置 p，每次查询时，
               根据要查找的值与 p 的大小关系，决定是往前还是往后查找，所以平均只需要查找一半的数据。
```

## 快慢指针

```shell
    1. 对于单向链表而言，快指针向后移动 2 步(ptr->next->next), 慢指针向后移动 1 步(ptr->next), 这样可以得到链表
    　 中间的节点．
    2. 快慢指针的应用
            (1) 判断该链表是否为循环链表
                    循环链表的定义：循环链表不仅仅是尾指针指向头指针这一种，还可能是尾指针指向该链表中某个节点．
                    优化思路： 如果是有一个指针跑的快的并且该链表是循环(一个环)，那么快指针总会比慢指针快一圈并且追到．
                    　　　　　　例如：快指针每次走 2 个节点，慢指针每次只走 1 个节点。
                    　　　　　　如果快指针到达NULL，说明链表以NULL为结尾，不是循环链表。如果 快指针追上慢指针，则表示出现了循环
                    代码:
                     
                        LinkList fast, slow;
                        fast = slow = L;
                        while (fast && fast->next)
                        {
                            slow = slow->next;
                            fast = fast->next->next;
                            if (slow == fast)
                            {
                                break;
                            }
                        }
                        
            (2) 如果链表存在环，则如何找到环的入口节点
                    设链起点到环入口点间的距离为x，环入口点到 fast 与 low 重合点的距离为 y，
                    fast 与 low 重合时 fast 已绕环n周（n>0），且此时 low 移动总长度为 s，则 fast 移动总长度为2s，
                    环的长度为r。则
                             s + nr = 2s,n>0      ①
                             s = x + y            ②
                             可以得出:
                              nr = x + y, 
                              
                    思路: 由 nr = x + y 可以知道，结合画图，如果指针 p1 从链表起点处开始遍历，指针 p2 从 fast 与 low 重合节点
                         处开始遍历，且 p1 和 p2 移动步长均为1, 当指针 p1 到达重合点时，刚好指针 p2　从重合点开始绕了 n 圈.
                         由于 2 个指针右一起走了 y 步，所以是在环的入口节点就相遇了.
                         
                    代码:
                        
                            Node* findEntry(Node* head, Node* encounter)
                            { 
                                Node *p1 = head, *p2 = encounter;
                                while(p1 != p2)
                                {
                                    p1 = p1->next;
                                    p2 = p2->next;
                                }
                                return p1;
                            }

                        
            (3) 在有序链表中寻找中位数
                    如果链表节点总数是奇数，当快指针到达链表尾时，则慢指针对应的是数据就是中位数.
                    如果链表节点总数是偶数，则快指针到达倒数第二个节点时，则此时慢指针对应的数据和慢指针下一个数据的平均数
                    是中位数．
                    
            (4) 两个单链表是否相交
                    思路: 将其中一个链表首尾相连，检测另外一个链表是否存在环，如果存在，则两个链表相交，
                          而检测出来的依赖环入口即为相交的第一个点。
                          
                          []->[]->[]->[]->[]
                                 |
                          []->[]->
                          
            (5) 取链表倒数第 n 个节点
                    思路: 置快指针步长为n，然后快慢指针同时以同一速度走，当快指针到达链表尾时，慢指针对应的就
                         是倒数第 n 个节点           
```

## 链表技巧

```shell
    1.利用哨兵简化编程难度
            哨兵结点就是第一个头节点没有数据，真正的有数据的结点是哨兵节点的下一个节点，这样设计有个好处是
            插入第一个结点和插入其他结点，删除最后一个结点和删除其他结点，都可以统一为相同的代码。
```

## 链表的应用

```shell
    1.链表的反转
         struct LinkNode_t{
            int value;
            sturct LinkNode_t *next;
         }LinkNode;
        (1) 递归方案:
                LinkNode* ReverseList(LinkNode *head)
                {
                    if(!head || !head->next)  // 主要是拿到链表的最后一个节点
                        return head;
                    
                    LinkNode *new_head = ReverseList(head->next);
                    head->next->next = head;
                    head->next = NULL;
                    return new_head;　// 返回的始终是反转后的头节点
                }
                
        (2) 非递归方案:
                LinkNode* ReverseList(LinkNode *head)
                {
                    if(!head || !head->next)  // 考虑空指针或则链表中只有一个节点的情况
                        return head;
                        
                    LinkNode* prev = head;
                    LinkNode* cur = head->next;
                    LinkNode* tmp = cur->next;
                    while(cur)
                    {
                        tmp = cur->next;
                        cur->next = prev;
                        prev = cur;
                        cur = tmp;
                    }
                    head->next = NULL;
                    return prev;
                }
```

## 总结

```shell
    1.链表有单向链表，双向链表，循环链表，常用的是双向链表
```