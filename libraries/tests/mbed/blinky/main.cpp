
#include "mbed.h"
#include "hx711.h"
#include "x_utils.h"
#include "mfrc522.h"
#include <stdio.h>
#include "pinmap.h"


//DigitalOut myled(PB_3);

static CAN_HandleTypeDef can_handle;
static CAN_FilterConfTypeDef can_filter;




Ticker ticker;

/**
 * 
 *  signals
 *  PA_10 -> RST
 *  PA_4 -> CS
 *  PB_3 -> SCK
 *  PB_4 -> MISO
 *  PB_5 -> MOSI
 * 
 */


static void can_struct_init(CAN_InitTypeDef * can_init) {
  /* Reset CAN init structure parameters values */
  
  /* Initialize the time triggered communication mode */
  can_init->TTCM = DISABLE;
  
  /* Initialize the automatic bus-off management */
  can_init->ABOM = DISABLE;
  
  /* Initialize the automatic wake-up mode */
  can_init->AWUM = DISABLE;
  
  /* Initialize the no automatic retransmission */
  can_init->NART = DISABLE;
  
  /* Initialize the receive FIFO locked mode */
  can_init->RFLM = DISABLE;
  
  /* Initialize the transmit FIFO priority */
  can_init->TXFP = DISABLE;
  
  /* Initialize the Mode member */
  can_init->Mode = CAN_MODE_NORMAL;
  
  /* Initialize the SJW member */
  can_init->SJW = CAN_SJW_1TQ;
  
  /* Initialize the CAN_BS1 member */
  can_init->BS1 = CAN_BS1_4TQ;
  
  /* Initialize the CAN_BS2 member */
  can_init->BS2 = CAN_BS2_3TQ;
  
  /* Initialize the Prescaler member */
  can_init->Prescaler = 1;
}


static void can_init() {
    
    can_handle.Init.TTCM = DISABLE;
    can_handle.Init.ABOM = DISABLE;
    can_handle.Init.AWUM = DISABLE;
    
    can_handle.Init.NART = DISABLE;
    can_handle.Init.RFLM = DISABLE;
    can_handle.Init.TXFP = DISABLE;
    
    can_handle.Init.Mode = CAN_MODE_LOOPBACK;
    can_handle.Init.SJW = CAN_SJW_1TQ;
    
    can_handle.Init.BS1 = CAN_BS1_9TQ;
    can_handle.Init.BS2 = CAN_BS2_8TQ;
    can_handle.Init.Prescaler = 16;
    
    HAL_CAN_Init(&can_handle);
    
}

static void filter_init() {
    can_filter.FilterNumber = 0;
    can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter.FilterIdHigh = 0x0000;
    can_filter.FilterIdLow = 0x0000;
    can_filter.FilterMaskIdHigh = 0x0000;
    can_filter.FilterMaskIdLow = 0x0000;
    can_filter.FilterFIFOAssignment = CAN_FIFO0;
    can_filter.FilterActivation = ENABLE;
    
    
    HAL_CAN_ConfigFilter(&can_handle, &can_filter);
}

static int send_message() {
    CanTxMsgTypeDef TxMessage;
    
    TxMessage.StdId = 0x11;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 2;
    TxMessage.Data[0] = 0xCA;
    TxMessage.Data[1] = 0xFE;
    
    
    can_handle.pTxMsg = &TxMessage;
    
//    uint8_t TransmitMailBox = 0;
//    uint32_t i = 0;
    
    
    HAL_StatusTypeDef status = HAL_CAN_Transmit(&can_handle, 1000);
    
    printf("transmit status = %d\r\n", status);
    
//    __HAL_CAN_TRANSMIT_STATUS(&can_handle, CAN_TXMAILBOX_0);
//    while( (__HAL_CAN_TRANSMIT_STATUS(&can_handle, CAN_TXMAILBOX_0) != true) && (i  !=  0xFFFF))
//    {
//      i++;
//    }
    
    status = HAL_CAN_Receive(&can_handle, CAN_FIFO0, 1000);
    printf("receive status0 = %d\r\n", status);
    printf("stdid: %d\r\n", can_handle.pRxMsg->StdId);
    status = HAL_CAN_Receive(&can_handle, CAN_FIFO1, 1000);
    printf("receive status1 = %d\r\n", status);
    printf("stdid: %d\r\n", can_handle.pRxMsg->StdId);
    
        
    return 0;
}


void read_rfid() {
    MFRC522 rfid(PA_4, PA_10);
    rfid.PCD_Init();
    
     while (1) {

        wait(0.1);
        //myled = 1;
        int present = rfid.PICC_IsNewCardPresent();
        printf("reading: %d \r\n", present);


        if (!present) {
            continue;
        }

        // Select one of the cards
        if (!rfid.PICC_ReadCardSerial()) {
            continue;
        }

        // Dump debug info about the card; PICC_HaltA() is automatically called
        rfid.PICC_DumpToSerial(&(rfid.uid));

        wait(0.1);
        //myled = 0;
    }
}

int can_test() {
    __CAN_CLK_ENABLE();
    __CAN1_CLK_DISABLE();
    __CAN1_CLK_ENABLE();
    
    memset(&can_handle, 0, sizeof(can_handle));
    
    
  //  RCC->APB1ENR |= RCC_APB1ENR_CANEN;
    pin_function(PA_11, STM_PIN_DATA(STM_MODE_AF_OD, GPIO_PULLUP, 4));
    pin_function(PA_12, STM_PIN_DATA(STM_MODE_AF_PP, GPIO_NOPULL, 4));
    
    
    
    HAL_CAN_DeInit(&can_handle);
    can_struct_init(&(can_handle.Init));
    
    can_init();
    filter_init();
    while(1) {
        send_message();
    }
}

void read_hx711(Serial &device) {
    HX711 load_cell(D4, D3);
    char print_buf[20];
    device.printf("Taring in 3 seconds...");
    wait(3);
    device.printf("Now\r\n");
    //load_cell.tare(25);
    while(1) {
        wait(0.1);
        double_to_str(load_cell.read_average(2), print_buf);
        device.printf("raw value = %s\r\n", print_buf);
        //device.printf("raw value = %d\r\n", load_cell.read());
    }
}

Serial device(USBTX, USBRX);  // tx, rx

 
int main() {
    device.baud(57600);
    //test_large_val_to_str(&device);
    read_hx711(device);
}
