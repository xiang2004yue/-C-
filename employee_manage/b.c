void insertsort(int *arr, int n)
{
    int i, j, temp;
    // i从1开始，第一个元素默认有序
    for(i = 1; i < n; i++)
    {
        temp = arr[i];   // 待插入元素
        j = i - 1;       // 有序区末尾下标

        // 向前遍历，比temp大的元素向后移动
        while(j >= 0 && arr[j] > temp)
        {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = temp; // 插入到正确位置
    }
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>



int  main()
{
    int arr[]={1,12,645,22,75,3,9,32};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 7; j >=0; j--)
        {
            arr[j]
        }
        
    }
    
    return 0;
}