# 图

## 基础知识

```shell
    1. 图中的元素叫作顶点(vertex), 图中的一个顶点可以与任意其他顶点建立连接关系,这种建立的关系叫作边（edge）
    2. 顶点的度(degree): 跟顶点相连接的边的条数
    3. 边的“方向”
            如果用户 A 关注了用户 B，就在图中画一条从 A 到 B 的带箭头的边，来表示边的方向。如果用户 A 和用户 B 互相关注，
            就画一条从 A 指向 B 的边，再画一条从 B 指向 A 的边.
            有向图: 边有方向的图   
            无向图: 边没有方向的图
    4. 有向图中度分为入度(In-degree)和出度(Out-degree),顶点的入度: 表示有多少条边指向这个顶点；
       顶点的出度: 表示有多少条边是以这个顶点为起点指向其他顶点
    5. 带权图: 每条边都有一个权重(weight)，可以通过这个权重来表示 QQ 好友间的亲密度
    
```

## 存储图的数据结构

```shell
    1. 邻接矩阵存储方法(Adjacency Matrix)
            (1) 邻接矩阵的底层依赖一个二维数组。对于无向图，如果顶点 i 与顶点 j 之间有边，将 A[i][j] 和 A[j][i] 标记为 1；
                对于有向图来，如果顶点 i 到顶点 j 之间，有一条箭头从顶点 i 指向顶点 j 的边，将 A[i][j] 标记为 1。
                如果有一条箭头从顶点 j 指向顶点 i 的边，将 A[j][i] 标记为 1。对于带权图，数组中元素的值就存储相应的权重。
            (2) 缺点:
                    浪费存储空间, 对于无向图，如果 A[i][j] 等于 1，那 A[j][i] 也肯定等于 1。只需要存储一个就可以。
                    向图的二维数组中，将其用对角线划分为上下两部分，只需要利用上面或者下面这样一半的空间就足够了,
                    对于存储的是稀疏图(Sparse Matrix)(顶点很多，但每个顶点的边并不多，那邻接矩阵的存储方法就更加浪费空间)
            (3) 优点:
                    a. 邻接矩阵的存储方式简单、直接，因为基于数组，获取两个顶点的关系时非常高效。
                    b. 方便计算,可以将很多图的运算转换成矩阵之间的运算。比如求解最短路径问题时会提到一个 
                    　　Floyd-Warshall 算法，就是利用矩阵循环相乘若干次得到结果。
    2. 邻接表存储方法(Adjacency List)
            邻接表有点像散列表, 每个顶点对应一条链表，链表中存储的是与这个顶点相连接的其他顶点。
                                
```

## 图的应用

```shell
    1.存储微博好友的关系
            邻接表中存储用户的关注关系, 邻接表中，每个顶点的对应的链表中，存储的就是这个顶点指向的顶点
            逆邻接表中存储的是用户的被关注关系(用户的粉丝),　逆邻接表中，每个顶点的对应的链表中，存储的是指向这个顶点的顶点
            
            因为需要按照用户名称的首字母排序，分页来获取用户的粉丝列表或者关注列表，用跳表比较合适。因为跳表插入、删除、查找
            都非常高效，时间复杂度是 O(logn)，空间复杂度上稍高，是 O(n)。跳表中存储的数据本来就是有序的了，
            分页获取粉丝列表或关注列表，非常高效。
            
            如果对于小规模的数据，比如社交网络中只有几万、几十万个用户，可以将整个社交关系存储在内存中，
            但是像微博那样有上亿的用户，数据规模太大，无法全部存储在内存中．
            方法一: 通过哈希算法等数据分片方式， 将邻接表存储在不同的机器上。在机器 1 上存储顶点 1，2，3 的邻接表，
                    在机器 2 上存储顶点 4，5 的邻接表。 当要查询顶点与顶点关系的时候，利用同样的哈希算法，
                    先定位顶点所在的机器，然后再在相应的机器上查找。
            方法二: 可以用数据库的索引
            
```

## 搜索算法

```shell
    1. 深度优先搜索算法和广度优先搜索算法都是基于“图”这种数据结构
    2. 图的数据结构
            public class Graph { // 无向图
              private int v; // 顶点的个数
              private LinkedList<Integer> adj[]; // 邻接表
             
              public Graph(int v) {
                this.v = v;
                adj = new LinkedList[v];
                for (int i=0; i<v; ++i) {
                  adj[i] = new LinkedList<>();
                }
              }
             
              public void addEdge(int s, int t) { // 无向图一条边存两次
                adj[s].add(t);
                adj[t].add(s);
              }
            }
    3. 广度优先搜索(BFS)
            (1) 广度优先搜索: Breadth-First-Search(BFS)
            (2) 广度优先搜索代码实现, 其中 s 代表起始顶点，t 代表终止顶点。搜索一条从 s 到 t 的路径(从 s 到 t 的最短路径)
                a. visited 是记录各个节点是否已经被访问(避免顶点被重复访问), 如果顶点 q 被访问，那 visited[q] 置为 true
                b. queue 是一个队列，用来存储已经被访问、但相连的顶点还没有被访问的顶点。广度优先搜索是逐层访问的，
                   只有把第 k 层的顶点都访问完后，才能访问第 k+1 层的顶点。当我们访问到第 k 层的顶点时，
                   需要把第 k 层的顶点记录下来，之后才能通过第 k 层的顶点来找第 k+1 层的顶点。
                   用这个队列来实现记录的功能
                c. prev 用来记录搜索路径。从顶点 s 开始，广度优先搜索到顶点 t 后，prev 数组中存储的就是搜索的路径。
                  这个路径是反向存储的。prev[w] 存储的是，顶点 w 是从哪个前驱顶点遍历过来的。比如，通过顶点 2 的邻接表访问
                  到顶点 3，那 prev[3] 就等于 2。为了正向打印出路径，需要递归地来打印
                
                    public void bfs(int s, int t) {
                      if (s == t) return;
                      boolean[] visited = new boolean[v];
                      visited[s]=true;
                      Queue<Integer> queue = new LinkedList<>();
                      queue.add(s);
                      int[] prev = new int[v];
                      for (int i = 0; i < v; ++i) {
                        prev[i] = -1;
                      }
                      
                      while (queue.size() != 0)
                      {
                        int w = queue.poll();
                       for (int i = 0; i < adj[w].size(); ++i) 
                       {
                          int q = adj[w].get(i);
                          if (!visited[q]) 
                          {
                                prev[q] = w;
                                if (q == t) 
                                {
                                  print(prev, s, t);
                                  return;
                                }
                                visited[q] = true;
                                queue.add(q);
                          }
                          
                        }
                      }
                      
                    }
                     
                    private void print(int[] prev, int s, int t) { // 递归打印 s->t 的路径
                      if (prev[t] != -1 && t != s) {
                        print(prev, s, prev[t]);
                      }
                      System.out.print(t + " ");
                    }
                    
            (3) 复杂度分析
                    (1) 时间复杂度 O(E) 
                    最坏情况下，终止顶点 t 离起始顶点 s 很远，需要遍历整个图才能找到。每个顶点都要进出一遍队列，
                    每个边也都会被访问一次，广度优先搜索的时间复杂度是 O(V+E)，V 表示顶点的个数，E 表示边的个数。
                    对于一个连通图来说，一个图中的所有顶点都是连通的，E 要大于等于 V-1，广度优先搜索的时间复杂度也可以
                    简写为 O(E)
                    
                    广度优先搜索的空间消耗主要在几个辅助变量 visited 数组、queue 队列、prev 数组上。
                    这三个存储空间的大小都不会超过顶点的个数，所以空间复杂度是 O(V)
                    
    4. 深度优先搜索(DFS)
            (1) 深度优先搜索(Depth-First-Search), 深度优先搜索找出来的路径，并不是顶点 s 到顶点 t 的最短路径。
                深度优先搜索用的是回溯思想, 这种思想解决问题的过程，适合用递归来实现。
            (2) 变量 found，它的作用是，当我们已经找到终止顶点 t 之后，就不再递归地继续查找。
            (3) 
                boolean found = false; // 全局变量或者类成员变量
                 
                public void dfs(int s, int t) {
                  found = false;
                  boolean[] visited = new boolean[v];
                  int[] prev = new int[v];
                  for (int i = 0; i < v; ++i) {
                    prev[i] = -1;
                  }
                  recurDfs(s, t, visited, prev);
                  print(prev, s, t);
                }
                 
                private void recurDfs(int w, int t, boolean[] visited, int[] prev) {
                  if (found == true) return;
                  visited[w] = true;
                  if (w == t) {
                    found = true;
                    return;
                  }
                  for (int i = 0; i < adj[w].size(); ++i) {
                    int q = adj[w].get(i);
                    if (!visited[q]) {
                      prev[q] = w;
                      recurDfs(q, t, visited, prev);
                    }
                  }
                }
            (4) 复杂度分析
                    a. 时间复杂度 O(E), 每条边最多会被访问两次，一次是遍历，一次是回退
                    b. 空间复杂度 O(V), 深度优先搜索算法的消耗内存主要是 visited、prev 数组和递归调用栈。
                       visited、prev 数组的大小跟顶点的个数 V 成正比，递归调用栈的最大深度不会超过顶点的个数
                    
```