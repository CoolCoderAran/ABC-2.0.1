#ifndef ABC_EHDRS_QUEU_HEADER
#define ABC_EHDRS_QUEU_HEADER

/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 2026. */

/*
 * B editor -- Definitions for queues of nodes.
 */

typedef struct queue *queue;

struct queue {
	HEADER;
	node q_data;
	queue q_link;
};

#define Qnil ((queue) Vnil)
#define emptyqueue(q) (!(q))

#endif  /* HEADER GUARD */
