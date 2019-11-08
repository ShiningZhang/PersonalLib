#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"

struct HeapItemDp
{
    int ed_;
    int idx_;
    int start_;
    int *list_;
    inline bool operator<(const HeapItemDp &y) const
    {
        if (idx_ == 0 || y.idx_ == 0)
            return idx_ < y.idx_;
        return list_[start_]< y.list_[y.start_];
    }
};

typedef HeapItemDp *pHeapItemDp;

HeapItemDp MinHI;

static inline void LoserAdjust(int *ls, const HeapItemDp *arr, int s, const int n)
{
    int t = ((s + n) >> 1);
    s = ls[s + n];
    while (t > 0)
    {
        if (arr[ls[t]] < arr[s])
        {
            s ^= ls[t];
            ls[t] ^= s;
            s ^= ls[t];
        }
        t >>= 1;
    }
    ls[0] = s;
}

static inline void LoserBuild(int *ls, const HeapItemDp *arr, const int n)
{
    for (int i = 0; i < n; i++)
    {
        ls[i] = n;
        ls[i + n] = i;
    }
    for (int i = n - 1; i >= 0; i--)
    {
        LoserAdjust(ls, arr, i, n);
    }
}

int main()
{
    int r[8][6]{//这里我用一个二维数组来表示有五个归并段，注意这五个归并段必须是先前已经进行了内部排序了的，k为5 为五路归并 
        31,42,53,63,74,1000,
        12,35,54,78,277,1000,
        9,13,53,67,89,1000,
        34,46,66,76,100,1000,
        34,46,66,76,100,1000,
        34,46,66,76,100,1000,
        34,46,66,76,100,1000,
        1,35,44,68,90,1000
    };

    int size = 0;
    int *list = (int *)malloc(100 * sizeof(int));
    int part_num = 8;
    int *loser = (int *)malloc(2 * part_num + 1);
    int sl = 0;
    HeapItemDp *heap = (HeapItemDp *)malloc((part_num + 1) * sizeof(HeapItemDp));
    for (int k = 0; k < part_num; ++k)
    {

        {
            HeapItemDp *pHI = heap + sl++;
            pHI->ed_ = 4;
            pHI->start_ = 0;
            pHI->list_ = r[k];
            pHI->idx_ = part_num;
        }
    }
    if (sl > 0)
    {
        int wlen = 0;
        if (sl > 1)
        {
            heap[sl] = MinHI;
            LoserBuild(loser, heap, sl);
            while (sl > 1)
            {
                int win = loser[0];
                pHeapItemDp pHI = heap + win;
                list[size++] = pHI->list_[pHI->start_];
                if (pHI->start_ < pHI->ed_)
                {
                    ++pHI->start_;
                    LoserAdjust(loser, heap, win, sl);
                }
                else
                {
                    if (win < --sl)
                    {
                        heap[win] = heap[sl];
                    }
                    if (sl > 1)
                    {
                        heap[sl] = MinHI;
                        LoserBuild(loser, heap, sl);
                    }
                }
            }
        }
        if (sl == 1)
        {
            pHeapItemDp pHI = heap + 0;
            memcpy(list + size, pHI->list_ + pHI->start_, (pHI->ed_ - pHI->start_ + 1) * sizeof(char *));
        }
    }
    return 0;
}

