#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "ht.c"
#include "sts_queue.c"

// queue for request
StsHeader *g_queue_task_in;

// queue for response 
StsHeader *g_queue_task_out;

// global g_stored data
struct ht *g_store;


#endif