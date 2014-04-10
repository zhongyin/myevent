/*
 * hello.c
 *
 *  Created on: 2013-1-5
 *      Author: 
 */

#include "hello.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/queue.h>

struct entry
{
    int oper;
    /*
    ..............
    可以加其他内容
    */
    CIRCLEQ_ENTRY(entry) entries;
};

/* 结构体变量申明 */
static CIRCLEQ_HEAD(circleq, entry) head;

int main(int argc, char **argv)
{
    struct entry *np, *n1, *n2;

    CIRCLEQ_INIT(&head);

    n1 = (struct entry *)malloc(sizeof(struct entry));
    n1->oper = 0x01;
    CIRCLEQ_INSERT_HEAD(&head, n1, entries);

    /*
    * 注意:在进行插入元素时，不能再采用n1分配的内存空间，需要重新分配内存
    * 由于CIRCLEQ_INSERT_HEAD 插入队列是利用指针指向n1分配的内存，
    * 如果仍然使用n1，会导致一直循环，大家可以试验下，看结果就知道(表达不是很清楚哈)
    */
    n1 = (struct entry *)malloc(sizeof(struct entry));
    n1->oper = 0x02;
    CIRCLEQ_INSERT_HEAD(&head, n1, entries);
    printf("CIRCLEQ_INSERT_HEAD(0x02):");
    for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    /*
    * 大家可以看到下面三种插入队列的不同位置情况
    */
    n2 = (struct entry *)malloc(sizeof(struct entry));
    n2->oper = 0x03;
    CIRCLEQ_INSERT_TAIL(&head, n2, entries);
    printf("CIRCLEQ_INSERT_TAIL(0x03):");
    for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    n2 = (struct entry *)malloc(sizeof(struct entry));
    n2->oper = 0x04;
    CIRCLEQ_INSERT_AFTER(&head, n1, n2, entries);
    printf("CIRCLEQ_INSERT_AFTER(0x02 0x04):");
    for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    n2 = (struct entry *)malloc(sizeof(struct entry));
    n2->oper = 0x05;
    CIRCLEQ_INSERT_BEFORE(&head, n1, n2, entries);
    printf("CIRCLEQ_INSERT_BEFORE(0x05 0x02):");
    for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    /* 上面是正向遍历队列，下面这个是反向遍历队列 */
    printf("Reverse traversal:");
    //for (np = head.cqh_last; np != (struct entry *) &head; np = np->entries.cqe_prev)
    CIRCLEQ_FOREACH_REVERSE(np, &head, entries) // 功能同上for循环
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    /* 删除n1(0x02)这个节点 */
    CIRCLEQ_REMOVE(&head, n1, entries);
    printf("CIRCLEQ_REMOVE(0x02):");
    //for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    CIRCLEQ_FOREACH(np, &head, entries) // 功能同上for循环
    {
        printf("%d ", np->oper);
    }
    printf("\n");

    /* 删除整个队列 */
    while (head.cqh_first != (void *)&head)
        CIRCLEQ_REMOVE(&head, head.cqh_first, entries);

    printf("CIRCLEQ_REMOVE ALL:");
    for (np = head.cqh_first; np != (struct entry *) &head; np = np->entries.cqe_next)
    {
        printf("%d   ", np->oper);
    }
    printf("\n");

    return 0;
}
