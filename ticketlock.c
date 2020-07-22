#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "ticketlock.h"

void initTicketLock(struct ticketlock *lk, char *name)
{
    lk->name = name;
    lk->next = 0;
    lk->turn = 0;
}

void acquireTicketLock(struct ticketlock *lk)
{
     int my_turn = fetch_and_add(&lk->next, 1);
     while (lk->turn != my_turn)
         ticketlockSleep(lk);
        // Record info about lock acquisition for debugging.
    getcallerpcs(&lk, lk->pcs);
}

void releaseTicketLock(struct ticketlock *lk)
{
    fetch_and_add(&lk->turn, 1);
    wakeup(lk);
} 