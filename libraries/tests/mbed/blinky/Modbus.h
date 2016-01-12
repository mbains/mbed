/* 
 * File:   Modbus.h
 * Author: mbai
 *
 * Created on January 8, 2016, 5:21 PM
 */

#ifndef MODBUS_H
#define	MODBUS_H

#include <mbed.h>

class Modbus {
public:
    Modbus() {
        m_device = NULL;
    }
    int init(uint8_t id, Serial * device);
    uint8_t poll();
    
    virtual ~Modbus();
    
private:
    Modbus(const Modbus& orig);
    uint8_t m_id;
    Serial * m_device;
    
    int8_t getRxBuffer();

};

#endif	/* MODBUS_H */

