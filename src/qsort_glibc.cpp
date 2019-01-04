/* Copyright (C) 1991,1992,1996,1997,1999,2004 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Written by Douglas C. Schmidt (schmidt@ics.uci.edu).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

/* If you consider tuning this algorithm, you should consult first:
   Engineering a sort function; Jon Bentley and M. Douglas McIlroy;
   Software - Practice and Experience; Vol. 23 (11), 1249-1265, 1993.  */

#include <alloca.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/* Byte-wise swap two items of size SIZE. */
/* 以字节为单位交换两个SIZE长度数组的SWAP宏 */
#define SWAP(a, b,size)
do
{
register size_t __size = (size);
register char *__a = (a), *__b = (b);
do
{
har __tmp = *__a;
*__a++ = *__b;
*__b++ = __tmp;
} while (--__size > 0);
} while (0)

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
/* 当一个数据段的长度小于这个值的时候，将不用快排对其进行排序。因为这个特别的的魔术数在Sun 4/260下的性能最好*/
#define MAX_THRESH 4
/* Stack node declarations used to store unfulfilled partition obligations. */
/* 用来存储还没处理的数据段索引*/
typedef struct
{
    char *lo;
    char *hi;
} stack_node;

/* The next 4 #defines implement a very fast in-line stack abstraction. */
/* 下面四个宏实现了一个非常快速的栈（的确很快，但是现实中真要这样做，请确保1. 这是核心代码。
    2. 这些代码百分比正确。要不然出错了，调试调死你，维护的人骂死你。）*/
/* The stack needs log (total_elements) entries (we could even subtract log(MAX_THRESH)).
   Since total_elements has type size_t, we get as upper bound for log (total_elements):
   bits per byte (CHAR_BIT) * sizeof(size_t).  */
/* 栈需要log(total_elements)个元素（当然我们也可以在这个基础上减去log(MAX_THRESH)。）(PS：log(x/y) = log(x) - log(y）)
   因为传入数据个数total_elements的类型是size_t，所以我们可以把栈元素上限设为：size_t的位数(CHAR_BIT) * sizeof(size_t)。
   PS：栈用来记录的是还没处理的数据段索引，最坏的情况是分开的两个数据段其中一个已经不用再分，这个时候栈不许要任何记录。
   最好的情况是进行log(total_elements)次划分，此时栈需要记录log(total_elements)个索引，但是这个算法在一个分片的元素个数
   小于MAX_THRESH便不再划分，所以实际只需log(total_elements / MAX_THRESH)个空间。CHAR_BIT在limits.h有定义，意思是一个
   字节有多少位，因为sizeof是算出一种类型占几个字节，所以(CHAR_BIT) * sizeof(size_t)是当total_elements去最大值的值，也就
   是这里栈元素个数的上限。*/
#define STACK_SIZE　　　　 (CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)　 ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define POP(low, high)　　((void) (--top, (low = top->lo), (high = top->hi)))
#define STACK_NOT_EMPTY  (stack < top)

/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:

这个快排的程序实现了Sedgewick书中讨论的四个优化，下面从大到小说明：（大概这意思...）

    1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.
   1. 不用递归，用了显示的指针栈存储下一段要排序的数据。为了节省时间，为栈申请了最大的存储空间。假设size_t是一个32位（64位）的整数，这里仅需要 32 * sizeof(stack_node) = 256 bytes（对于64位：1024bytes）。事实上很小。（一个栈节点两指针，32位就是2 * 4 个字节，64位是8 * 2位字节）

2. Chose the pivot element using a median-of-three decision tree. This reduces the probability of selecting a bad pivot value and
 eliminates certain extraneous comparisons.
   2. 用中值决策树选择关键值。这减小了选择一个差关键值的可能性和排除特定的无关的比较。

3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.
   3. 只用快排对TOTAL_ELEMS / MAX_THRESH个数据段进行了排序，用插入排序对每个数据段的
      MAX_THRESH个数据进行排序。这是一个很好的改进，因为插入排序在处理小的、基本有序的数
      据段时比快排更快。
4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!
   4. 大的数据分段通常先压入栈内，算法优先处理小的数据分段。这就保证栈的元素不会超过
      log(total_elems)（事实上这里只用了常数个空间）。
*/
void _quicksort (void *const pbase, size_t total_elems, size_t size, __compar_d_fn_t cmp, void *arg)
{
    /* 寄存器指针，最快的指针，当然系统不一定会把它放到寄存器。register只是一种建议。*/
    register char *base_ptr = (char *) pbase;

    const size_t max_thresh = MAX_THRESH * size;

    if (total_elems == 0)
        /* Avoid lossage with unsigned arithmetic below.  */
        return;

    if (total_elems > MAX_THRESH)
    {
        char *lo = base_ptr;
        char *hi = &lo[size * (total_elems - 1)];
        /* 因为用了上面栈的宏，所以下面两个变量的名字一定不能改...*/
        stack_node stack[STACK_SIZE];
        stack_node *top = stack;

        PUSH (NULL, NULL);

        while (STACK_NOT_EMPTY)
        {
            char *left_ptr;
            char *right_ptr;
            /* Select median value from among LO, MID, and HI. Rearrange
               LO and HI so the three values are sorted. This lowers the
               probability of picking a pathological pivot value and skips a comparison for both the LEFT_PTR and RIGHT_PTR in the while loops. */
            /* 在数组的第一位、中间一位、最后一位中选出一个中值。同时也会对第一位和最后
            一位进行排序以达到这三个值都是有序的目的。这降低了选择一个很烂的关键值的
            可能性，同时也跳过了左指针和右指针在while循环里面的一次比较。*/

            char *mid = lo + size * ((hi - lo) / size >> 1);

            if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
                    SWAP (mid, lo, size);
            if ((*cmp) ((void *) hi, (void *) mid, arg) < 0)
                    SWAP (mid, hi, size);
            else
                goto jump_over;
            if ((*cmp) ((void *) mid, (void *) lo, arg) < 0)
                    SWAP (mid, lo, size);
            jump_over:
            ;

            left_ptr  = lo + size;
            right_ptr = hi - size;

            /* Here's the famous ``collapse the walls'' section of quicksort.
               Gotta like those tight inner loops!  They are the main reason
               that this algorithm runs much faster than others. */
            /* 这里就是快排中著名的“collapse the walls（推墙？？）”。和那些紧凑
               的内层循环非常像！它们是这个算法比其他算法快的主要原因。
               PS：了解过快排的应该对下面这一段都比较熟悉，就是把比关键值小的移
               到数组左边，比关键值大的移到数组右边，把数据分成大小两段的过程*/
            do
            {
                while ((*cmp) ((void *) left_ptr, (void *) mid, arg) < 0)
                    left_ptr += size;

                while ((*cmp) ((void *) mid, (void *) right_ptr, arg) < 0)
                    right_ptr -= size;
                if (left_ptr < right_ptr)
                {
                    SWAP (left_ptr, right_ptr, size);
                    if (mid == left_ptr)
                        mid = right_ptr;
                    else if (mid == right_ptr)
                        mid = left_ptr;
                    left_ptr += size;
                    right_ptr -= size;
                }
                else if (left_ptr == right_ptr)
                {
                    left_ptr += size;
                    right_ptr -= size;
                    break;
                }
            }
            while (left_ptr <= right_ptr);

            /* Set up pointers for next iteration.  First determine whether
               left and right partitions are below the threshold size.  If so,
               ignore one or both.  Otherwise, push the larger partition's
               bounds on the stack and continue sorting the smaller one. */
            /*  给下次迭代的指针赋值。首先判断左右两段数据的元素个数是否小于阈值
                ，如果是，跳过这一个或两个分段。如若不是，把大的数据段的开始和结
                束指针入栈，继续划分小的数据段。*/

            if ((size_t) (right_ptr - lo) <= max_thresh)
            {
                /* 左右两个数据段的元素都小于阈值，取出栈中数据段进行划分*/
                if ((size_t) (hi - left_ptr) <= max_thresh)
                    /* Ignore both small partitions. */
                POP (lo, hi);
                else
                /* Ignore small left partition. （只有左边大于阈值）*/
                lo = left_ptr;
            }
            else if ((size_t) (hi - left_ptr) <= max_thresh)
                /* Ignore small right partition. （只有右边大于阈值）*/
                hi = right_ptr;
            else if ((right_ptr - lo) > (hi - left_ptr))
            {
                /* Push larger left partition indices. */
                /* 两个数据段的元素个数都大于阈值，大的入栈，小的继续划分。 */
                PUSH (lo, right_ptr);
                lo = left_ptr;
            }
            else
            {
                /* Push larger right partition indices. */
                /* 两个数据段的元素个数都大于阈值，大的入栈，小的继续划分。 */
                PUSH (left_ptr, hi);
                hi = right_ptr;
            }
        }
    }
    /* Once the BASE_PTR array is partially sorted by quicksort the rest
       is completely sorted using insertion sort, since this is efficient
       for partitions below MAX_THRESH size. BASE_PTR points to the beginning
       of the array to sort, and END_PTR points at the very last element in
       the array (*not* one beyond it!). */
    /*  当数组经过快排的排序后，已经是整体有序了。剩下的排序由插入排序完成，因
        为数据段小于MAX_THRESH时，插入排序效率更高。此时排序的首指针是数组的首
        指针，尾指针是数组的尾指针（不是倒数第二个）*/

#define min(x, y) ((x) < (y) ? (x) : (y))

    {
        char *const end_ptr = &base_ptr[size * (total_elems - 1)];
        char *tmp_ptr = base_ptr;
        char *thresh = min(end_ptr, base_ptr + max_thresh);
        register char *run_ptr;

        /* Find smallest element in first threshold and place it at the
           array's beginning.  This is the smallest array element,
           and the operation speeds up insertion sort's inner loop. */
        /* 找出第一段的最小一个值并把它放在数组的第一个位置。这是数组的
           最小元素（用快排排过，应该比较容易理解），这一步可以加入插入
           排序的内层循环*/

        for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
            if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
                tmp_ptr = run_ptr;

        if (tmp_ptr != base_ptr)
                SWAP (tmp_ptr, base_ptr, size);

        /* Insertion sort, running from left-hand-side up to right-hand-side.  */
        /* 从左到右执行插入排序。*/

        run_ptr = base_ptr + size;
        while ((run_ptr += size) <= end_ptr)
        {
            /*上面说的加速内层循环，就是不用在这里判断*/
            tmp_ptr = run_ptr - size;
            /*一直往回找直到找到大于或等于当前元素的元素*/
            while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr, arg) < 0)
                tmp_ptr -= size;

            /*把当前元素移到找出元素的后面去*/
            tmp_ptr += size;
            if (tmp_ptr != run_ptr)
            {
                char *trav;

                trav = run_ptr + size;
                while (--trav >= run_ptr)
                {
                    char c = *trav;
                    char *hi, *lo;
                    /*这个内层循环只是把每个元素的最后一位移到后面一个元素去*/
                    for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                        * hi = *lo;
                    *hi = c;
                }
            }
        }
    }
}