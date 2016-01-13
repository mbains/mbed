/* 
 * File:   sul_ring.h
 * Author: mannyb
 *
 * Created on November 24, 2013, 12:12 PM
 */

#ifndef RING_H
#define	RING_H
#include<stdint.h>

#define SUL_RINGBUF_SIZE (256)

typedef struct {
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint8_t data[SUL_RINGBUF_SIZE];
} SulRingBuffer;

int Sul_Ring_Enqueue(SulRingBuffer *buf, uint8_t data);
int Sul_Ring_Dequeue(SulRingBuffer *buf, uint8_t *data);

#endif	/* RING_H */

