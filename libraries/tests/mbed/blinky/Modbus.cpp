/* 
 * File:   Modbus.cpp
 * Author: mbai
 * 
 * Created on January 8, 2016, 5:21 PM
 */

#include "Modbus.h"

int Modbus::init(uint8_t id, Serial * device) {

    m_id = id;
    m_device = device;
    return 0;
};

uint8_t Modbus::poll() {
    m_device->readable();
    
    //XXX: Missing timeout code
    
    if(!m_device->readable()) {
        return 0;
    }
    
//    if
}

int8_t Modbus::getRxBuffer() {
    
    bool overflow = false;
    
    
    while(m_device->readable()) {
        
    }
}

Modbus::~Modbus()
{
}

