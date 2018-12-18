# 数据结构与算法

## 数据结构与算法的意义

```shell
    1. 不需要自己实现，并不代表什么都不需要了解,我们通过学习算法，了解框架基本实现可以更好的使用该框架．
    　 知道它的时间复杂度和空间复杂度，以及知道这个框架适用于哪些场景．
    2. 学习算法需要自己实现一遍．
    3. 数据结构是对数据的存储结构．而算法是对存储结构的一组操作．
    4.  10 个数据结构：数组、链表、栈、队列、散列表、二叉树、堆、跳表、图、Trie 树；
        10 个算法： 递归、排序、二分查找、搜索、哈希算法、贪心算法、分治算法、回溯算法、动态规划、字符串匹配算法
    5. https://github.com/wangzheng0822/algo
```

### avl算法

#### 概念

``` c
    自平衡二叉查找树,也被称为高度平衡树,AVL树中任何节点的两个子树的高度最大差别为1
    AVL树的查找、插入和删除在平均和最坏情况下都是O(logn)
    如果在AVL树中插入或删除节点后,使得高度之差大于1.此时,AVL树的平衡状态就被破坏,它就不再是一棵二叉树；为了让它重新维持在一个平衡状态,
    就需要对其进行旋转处理.学AVL树，重点的地方也就是它的旋转算法

```

### 基数树算法

#### 概念

``` c
    基数树,或称Patricia trie/tree，或crit bit tree,压缩前缀树,是一种更节省空间的Trie（前缀树）.对于基数树的每个节点,
    如果该节点是唯一的子树的话,就和父节点合并.
    应用:
        trie 树的一个典型应用是前缀匹配,比如下面这个很常见的场景,在我们输入时,搜索引擎会给予提示.
        IP选路,也是前缀匹配,一定程度会用到trie
        
    相关代码: ngx_radix_tree.h

```

### 红黑树算法

#### 概念

``` c
       
    相关代码: ngx_rbtree.h

```