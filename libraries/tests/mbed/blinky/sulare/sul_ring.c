

#include "sul_ring.h"

static void buf_reset(SulRingBuffer *buf);
static int buf_len(SulRingBuffer *buf);
static int buf_isfull(SulRingBuffer *buf);
static int buf_isempty(SulRingBuffer *buf);
static uint8_t buf_get_byte(SulRingBuffer *buf);
static void buf_put_byte(SulRingBuffer *buf, uint8_t val);


static void buf_reset(SulRingBuffer *buf)
{
    buf->head = buf->tail = 0;
}

static int buf_len(SulRingBuffer *buf)
{
    int len = buf->tail - buf->head;
    if (len < 0)
        len += SUL_RINGBUF_SIZE;
    
    return len;
}

static int buf_isfull(SulRingBuffer *buf)
{
    return buf_len(buf) == (SUL_RINGBUF_SIZE-1);
}

static int buf_isempty(SulRingBuffer *buf)
{
    return buf->head == buf->tail;
}

static uint8_t buf_get_byte(SulRingBuffer *buf)
{
    uint8_t item;
    
    item = buf->data[buf->head++];
    if (buf->head == SUL_RINGBUF_SIZE)         // Wrap
        buf->head = 0;
        
    return item;
}

static void buf_put_byte(SulRingBuffer *buf, uint8_t val)
{
    
    buf->data[buf->tail++] = val;
    if (buf->tail == SUL_RINGBUF_SIZE)
        buf->tail = 0;
}

int Sul_Ring_Enqueue(SulRingBuffer* buf, uint8_t data)
{
    if(buf_isfull(buf))
        return 0;
    else {
        buf_put_byte(buf, data);
    }
    return 1;
}

int Sul_Ring_Dequeue(SulRingBuffer* buf, uint8_t *data)
{
    if(buf_isempty(buf))
        return 0;
    else {
        *data = buf_get_byte(buf);
    }
    return 1;
}