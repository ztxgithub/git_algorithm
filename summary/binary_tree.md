# 二叉树

## 基础知识

```shell
    1.结点的高度 = 该结点到叶子结点的最长路劲(边数)
      结点的深度 = 根节点到到这个结点所经历的边的个数
      结点的层数 = 结点的深度 + 1
      树的高度 =  根结点的高度
      
    2. 满二叉树: 叶子节点全都在最底层，除了叶子节点之外，每个节点都有左右两个子节点
       完全二叉树: 叶子节点都在最底下两层，最后一层的叶子节点都靠左排列，并且除了最后一层，其他层的节点个数都要达到最大
    3. 
    
```

## 存储二叉树

```shell
    1. 链式存储法(常用,链表)
        每个节点有三个字段，其中一个存储数据，另外两个是指向左右子节点的指针,只要拿到根结点就能遍历整个二叉树
    2. 顺序存储法(基于数组)
            把根节点存储在下标 i = 1 的位置，那左子节点存储在下标 2 * i = 2 的位置，右子节点存储在 2 * i + 1 = 3 的位置。
            以此类推，B 节点的左子节点存储在 2 * i = 2 * 2 = 4 的位置，右子节点存储在 2 * i + 1 = 2 * 2 + 1 = 5 的位置
            
            如果节点 X 存储在数组中下标为 i 的位置，下标为 2 * i 的位置存储的就是左子节点，下标为 2 * i + 1 的位置存储的就是右子节点。
            下标为 i/2 的位置存储就是它的父节点。只要知道根节点存储的位置(为了方便计算子节点，根节点会存储在下标为 1 的位置)，
            就可以通过下标计算，把整棵树都串起来
            
            (1) 完全二叉树仅仅“浪费”了一个下标为 0 的存储位置。如果是非完全二叉树，会浪费比较多的数组存储空间
                如果某棵二叉树是一棵完全二叉树，那用数组存储无疑是最节省内存的一种方式,
            (2) 数组顺序存储的方式比较适合完全二叉树，其他类型的二叉树用数组存储会比较浪费存储空间。
            
```

## 遍历二叉树

```shell
    主要前，中，后是针对当前结点而言
    1. 前序遍历
            (1) 对于树中的任意节点来说，先打印这个节点，然后再打印它的左子树，最后打印它的右子树。
            (2) 前序遍历的递推公式： preOrder(r) = print r -> preOrder(r->left) -> preOrder(r->right)
            (3) 代码:
                    void preOrder(Node* root) {
                      if (root == null) return;
                      print root // 此处为伪代码，表示打印 root 节点
                      preOrder(root->left);
                      preOrder(root->right);
                    }

    2. 中序遍历
            (1) 对于树中的任意节点来说，先打印它的左子树，然后再打印它本身，最后打印它的右子树。
            (2) 中序遍历的递推公式： inOrder(r) =  inOrder(r->left) ->  print r -> inOrder(r->right)
            (3) 代码:
                    void inOrder(Node* root) {
                      if (root == null) return;
                      inOrder(root->left);
                      print root // 此处为伪代码，表示打印 root 节点
                      inOrder(root->right);
                    }
    3. 后序遍历
            (1) 对于树中的任意节点来说，先打印它的左子树，然后再打印它的右子树，最后打印这个节点本身。
            (2) 后序遍历的递推公式: postOrder(r) = postOrder(r->left) -> postOrder(r->right) -> print r
            (3) 代码:
                    void postOrder(Node* root) {
                      if (root == null) return;
                      postOrder(root->left);
                      postOrder(root->right);
                      print root // 此处为伪代码，表示打印 root 节点
                    }
                    
    4. 层序遍历(广度优先遍历) : 借用队列辅助即可，根节点先入队列，然后循环从队列中pop节点，将pop出来的节点的左子节点先入队列，
                             右节点后入队列，依次循环，直到队列为空，遍历结束
                    
    5. 二叉树遍历的时间复杂度是 O(n), 每个节点最多会被访问两次，所以遍历操作的时间复杂度，跟节点的个数 n 成正比
    
```

## 二叉查找树(Binary Search Tree)

```shell
    1. 二叉查找树也叫二叉搜索树，二叉查找树是为了实现快速查找而生的
    2. 二叉查找树最大的特点就是，支持动态数据集合的快速插入、删除、查找操作，能够实现这些特性依赖于特殊结构，
       二叉查找树要求，在树中的任意一个节点，其左子树中的每个节点的值，都要小于这个节点的值，而右子树节点的值都大于这个节点的值
    3. 二叉树查找树的查找操作
            (1) 中心思想:
                        先取根节点，如果它等于要查找的数据，就返回。如果要查找的数据比根节点的值小，就在左子树中递归查找；
                        如果要查找的数据比根节点的值大，就在右子树中递归查找。
            (2) 代码实现:
                        public class BinarySearchTree {
                          private Node tree;
                         
                          public Node find(int data) {
                            Node p = tree;
                            while (p != null) {
                              if (data < p.data) p = p.left;
                              else if (data > p.data) p = p.right;
                              else return p;
                            }
                            return null;
                          }
                         
                          public static class Node {
                            private int data;
                            private Node left;
                            private Node right;
                         
                            public Node(int data) {
                              this.data = data;
                            }
                          }
                        }
    4. 二叉树查找树的插入操作
            (1) 中心思想：
                        新插入的数据一般都是在叶子节点上，只需要从根节点开始，依次比较要插入的数据和节点的大小关系。
                        如果要插入的数据比节点的数据大，并且节点的右子树为空，就将新数据直接插到右子节点的位置；
                        如果不为空，就再递归遍历右子树，查找插入位置。同理，如果要插入的数据比节点数值小，并且节点的左子树为空，
                        就将新数据插入到左子节点的位置；如果不为空，就再递归遍历左子树，查找插入位置。
            (2) 代码实现:
                        public void insert(int data) {
                          if (tree == null) 
                          {
                            tree = new Node(data);
                            return;
                          }
                         
                          Node p = tree;
                          while (p != null) 
                          {
                            if (data > p.data) 
                            {
                              if (p.right == null) 
                              {
                                p.right = new Node(data);
                                return;
                              }
                              p = p.right;
                            }
                            else 
                            { // data < p.data
                              if (p.left == null) 
                              {
                                p.left = new Node(data);
                                return;
                              }
                              p = p.left;
                            }
                            
                          }
                          
                        }
    5. 二叉树查找树的删除操作
            I. 中心思想:
                (1) 第一种情况: 如果要删除的节点没有子节点，可以直接删除(只需要直接将父节点中，指向要删除节点的指针置为 null)
                (2) 第二种情况: 如果要删除的节点只有一个子节点（只有左子节点(数据比要删除的数据小)或者右子节点(数据比要删除的数据大)）,
                               只需要更新父节点中，由原先指向要删除节点的指针改为指向要删除节点的子节点
                (3) 第三中情况: 如果要删除的节点有两个子节点,需要找到这个节点的右子树中的最小节点，把它的数据替换到要删除的节点上,
                              　然后再删除掉这个最小节点(采用第一种方案或则第二种方案)，
                                因为最小节点肯定没有左子节点（如果有左子结点，那就不是最小节点了）
                                
            II. 代码实现
                    public void delete(int data) 
                    {
                          Node p = tree; // p 指向要删除的节点，初始化指向根节点
                          Node pp = null; // pp 记录的是 p 的父节点
                          while (p != null && p.data != data) 
                          {
                            pp = p;
                            if (data > p.data) p = p.right;
                            else p = p.left;
                          }
                          if (p == null) return; // 没有找到
                         
                          // 要删除的节点有两个子节点
                          if (p.left != null && p.right != null) 
                          { // 查找右子树中最小节点
                            Node minP = p.right;
                            Node minPP = p; // minPP 表示 minP 的父节点
                            while (minP.left != null) 
                            {
                              minPP = minP;
                              minP = minP.left;
                            }
                            p.data = minP.data; // 将 minP 的数据替换到 p 中
                            p = minP; // 下面就变成了删除 minP 了
                            pp = minPP;
                          }
                         
                          // 删除节点是叶子节点或者仅有一个子节点
                          Node child; // p 的子节点
                          if (p.left != null) child = p.left;
                          else if (p.right != null) child = p.right;
                          else child = null;
                         
                          if (pp == null) tree = child; // 删除的是根节点
                          else if (pp.left == p) pp.left = child;
                          else pp.right = child;
                    }
                    
            III. 二叉查找树的删除操作还有个取巧的方法，就是单纯将要删除的节点标记为“已删除”，但并不真正从树中将这个节点删除。
                 这样原本删除的节点还需要存储在内存中，比较浪费内存空间，但是删除操作就变得简单
```

