---
title: 热土豆问题
date: 2025-03-05
tags: algorithms
---

Josephus 问题是下面的游戏：N 个人从 1 到 N 编号，围坐成一个圆圈。从 1 号开始传递一个热土豆。经过 M 次传递后拿着热土豆的人被清除离座，围坐的圆圈缩紧，由坐在被清除的人后面的人拿起热土豆继续进行游戏。最后剩下的人取胜。因此，如果M =0和N=5，则依次清除后5号获胜，若M = 1和 N = 5，则清除顺序是2,4,1,5。

+ 编写一个程序解决M和N在一般值下的Josephus 问题，应使你的程序尽可能地高效，要确保能够清除单元。

+ 你的程序的运行时间是多少？
+ 若M = 1你的程序运行时间为多少，对于大的N(N>10000) ，其free例程是如何影响实际速度的。

首先我们给出一段非常简单的代码

要做的事情是，首先定义链表结构体，它是一个储存1到N的编号的链表。

那么我是使用链表完成的，以下是重要的代码讲解：

首先是定义链表结构体
```c
typedef struct node
{
    int data;
    struct node* next;
}Node;
```

该结构体创建了一个储存编号的变量data和一个指向下一个单元的指针next。接着我们定义一个数量为n的表，它用来初始化游戏需要的条件

```c
Node * create(int n){
    //初始化头结点
    Node * head = NULL,*p=NULL, *r=NULL;
    head = (Node * )malloc(sizeof(Node));
    if(head == NULL){
        printf("申请内存失败...");
    }
    head->data = 1;
    head->next = head;
    p = head;
    //创建剩余的节点
    for (int i =2; i <=n ;i++ ){
        r = (Node * )malloc(sizeof(Node));
        r->data=i;
        r->next = NULL;
        p->next = r;
        p=r;
    }
    p->next = head;
    p = head;
    return head;
}

```

该函数返回一个指向Node结构的指针， head是表头，p和r是两个用来交换数据的临时变量。该函数首先初始化一个表头head，令编号为1，下一个指针指向自己进行初始化。在for循环里面，开始创建2到n的单元，p->next =r完成尾插任务，让前一个单元的指向下一个元素的指针按自然数顺序排列。最后，令p指向head完成循环列表的创建。

最后是约瑟夫问题的求解

```c
int joseph(int n, int m){
    Node *list = create(n); //创建长度为n的约瑟夫环
    Node *temp = (Node*)malloc(sizeof(Node)); //临时变量
    Node *r = (Node*)malloc(sizeof(Node)); //储存要删除的临时变量
    while (list->next != list) //判断表是否删干净
    {   

        if( m == 0 ){
            //m=0按顺序把表打印出来然后free元素
            for(int i =0;i < n-1; i++){
                printf("%d out ",list->data);
                r = list; //储存要删除的变量
                list = list->next;
                free(r);
            }
        }
        else
        {
            for(int i=0; i<m;i++){ //若没到m就继续向前推进单元
                 temp = list;
                 list = list->next;
            }
            printf(" %d out  ",list->data); //打印出列的人
            r = list;
            temp->next = list->next; //让前一个元素的指针指向出列单元的下一个
            list = list->next;
            free(r);
        }   
    }
    int ser = list->data;
    return ser;
}
```

while循环的条件list->next == list的时候意味着表里只有一个元素。此时中断循环。否则进入循环，若m==0，也就是不按间隔让人出列，此时只需要按顺序打印链表并free打印的元素即可。若m 不等于0。则进入另一个for循环中当没有循环m次的时候，让单元做一次移动，移动了m-1次后循环停止，for循环后面的语句处理出列的人，首先是打印出列的人的编号list->data。接着r是用来储存需要free的元素，temp->next 指向list的下一个元素，最后令list = List->next 即可把原来的单元变成单独的单元，此时只需要free(r)即可释放原来的单元。最后，再经过函数删除表的过程后剩下的单元素给返回即可。

接下来是一个创建20个人，出列间隔为2的简单示例

```c
int main(){
    int ser = joseph(100,2); //进行20人,间隔为2的游戏
    printf(" %d live", ser); //打印幸存者
    return 0;
}

```

然后我们解决第二个问题，我们使用了一个while循环和一个2for循环，根据算法时间度的定义，for循环+for循环的时间是O(n)的，而while嵌套for循环的时间是O(n*n)。再这个代码中，我们的表有n个，因此while最差的时间复杂度为O(n)。接着，第二个for循环每次循环m次，所以最坏的结果是O(m * n)。而m=0的情况是O(n)。

最后是第三个问题，若不用free，我们可以减轻很多的步骤，至少是使用free的1/3。