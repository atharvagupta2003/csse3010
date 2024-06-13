#include "s4819560_txradio.h"
#include "s4819560_rcmcont.h"
#include "s4819560_console.h"
#include "s4819560_hamming.h"
#include "s4819560_board_pb.h"
#include "s4819560_mfs_led.h"
#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus.h"
#include "myconfig.h"
#include "debug_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void prepare_Join_Packet(void);
void prepare_Xyz_Packet(XyzPayload_t xyz);
void prepare_Rot_Packet(RotPayload_t rot);
void prepare_Zoom_Packet(ZoomPayload_t zoom);
typedef enum
{
    INIT,
    TRANSMITTING,
    TRANSMITTED
} TxcommandType_t;
uint8_t unencoded_packet[16] = {0};
uint8_t encoded_packet[32];
TxcommandType_t txcommand;
int flag = 0;

void s4819560_txradio_init(){
     
    BRD_delayInit();            // Enable uS Delay

    nrf24l01plus_spi_init();    // Enable SPI

    // Set CE low for idle state
    NRF_CE_LOW();

    // Power down nrf before powering up.
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, myradiotxaddr, 5);            // Writes TX_Address to nRF24L01
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_RX_ADDR_P0, myradiotxaddr, 5);    //NRF24L01P_TX_ADR_WIDTH);

    nrf24l01plus_wr(NRF24L01P_EN_AA, 0x00);                                          // Disable Auto.Ack
    nrf24l01plus_wr(NRF24L01P_EN_RXADDR, 0x01);                                       // Enable Pipe0
    nrf24l01plus_wr(NRF24L01P_RX_PW_P0, NRF24L01P_TX_PLOAD_WIDTH);     // Select same RX payload width as TX Payload width

    nrf24l01plus_wr(NRF24L01P_RF_CH, MYRADIOCHAN);                                   // Select RF channel
    nrf24l01plus_wr(NRF24L01P_RF_SETUP, 0x06);                                         // TX_PWR:0dBm, Datarate:1Mbps

    nrf24l01plus_wr(NRF24L01P_CONFIG, 0x02); 
}

void s4819560_tsk_txradio(){
    commandQueue = xQueueCreate(50, sizeof(command));
    xyzpayloadQueue = xQueueCreate(50, sizeof(xyzPayload));
    rotpayloadQueue = xQueueCreate(50, sizeof(rotPayload));
    zoompayloadQueue = xQueueCreate(50, sizeof(zoomPayload));
    
    CommandType_t recv_command;
    XyzPayload_t recv_xyz;
    ZoomPayload_t recv_zoom;
    RotPayload_t recv_rot;
    s4819560_reg_board_pb_init(1);
    s4819560_reg_board_pb_semaphore_init();
    for(;;){
    xQueueReceive(commandQueue, &recv_command, 5);
    xQueueReceive(xyzpayloadQueue, &recv_xyz, 5);
    xQueueReceive(zoompayloadQueue, &recv_zoom, 5);
    xQueueReceive(rotpayloadQueue, &recv_rot, 5);
   //  debug_log("state:--%d",recv_command);
    int next_state = INIT;
    int a;
     switch (txcommand)
     {
     case INIT:
             if (xSemaphoreTake(bd_semaphore,0) == pdTRUE)
            {

               if (s4819560_reg_board_pb_press_get(1)>0)
               {              
               prepare_Join_Packet(); 
               flag = 1;
               s4819560_REG_MFS_LED_D1_ON();
               s4819560_reg_board_pb_press_reset(1); 
               }
            xSemaphoreGive(bd_semaphore); 
            }
            next_state = TRANSMITTING; 
         // debug_log("leaving init state");
         break;
     case TRANSMITTING:
         //   debug_log("%d",recv_command);
         if (flag == 1)
         {
            if (recv_command == 1)
            {
               s4819560_REG_MFS_LED_D2_ON();
               s4819560_REG_MFS_LED_D1_OFF();
               s4819560_REG_MFS_LED_D4_OFF();
               //  s4819560_REG_MFS_LED_D3_OFF();
               prepare_Xyz_Packet(recv_xyz);
            }else if (recv_command == 2)
            {
               s4819560_REG_MFS_LED_D3_ON();
               s4819560_REG_MFS_LED_D1_OFF();
               s4819560_REG_MFS_LED_D4_OFF();
               s4819560_REG_MFS_LED_D2_OFF();
               prepare_Rot_Packet(recv_rot);
            }else if (recv_command == 3)
            {
               s4819560_REG_MFS_LED_D4_ON();
               s4819560_REG_MFS_LED_D1_OFF();
               s4819560_REG_MFS_LED_D3_OFF();
               s4819560_REG_MFS_LED_D2_OFF();
               prepare_Zoom_Packet(recv_zoom);
            }
            next_state = TRANSMITTED;
         }else{
            next_state = INIT;
         }
         break;
     case TRANSMITTED:
         // for (int i = 0; i < 16; i++)
         // {
         //    debug_log("%d\n",unencoded_packet[i]); 
         // } 
         for (int i = 0; i < 16; i++){
         uint16_t encoded_byte = s4819560_lib_hamming_byte_encode(unencoded_packet[i]);
         encoded_packet[i*2] = encoded_byte & 0xFF;
         encoded_packet[i*2 + 1] = (encoded_byte >> 8) & 0xFF;
         }
         // for (int i = 0; i < 33; i++)
         // {
         //    debug_log("%d",encoded_packet[i]);
         // }
         
         // debug_log("transmitted state");
         nrf24l01plus_send(encoded_packet);
         next_state = IDLE;
         // debug_log("leaving transmitted state");
         break;
     }
   //   vTaskDelay(5);
     txcommand = next_state;
     }
     
}
void prepare_Join_Packet(void){
      unencoded_packet[0] = 0x20;
      unencoded_packet[1] = 0x09;
      unencoded_packet[2] = 0x56;
      unencoded_packet[3] = 0x19;
      unencoded_packet[4] = 0x48;
      unencoded_packet[5] = 'J';
      unencoded_packet[6] = 'O';
      unencoded_packet[7] = 'I';
      unencoded_packet[8] = 'N';
      debug_log("Join packet prepared");
}

void prepare_Xyz_Packet(XyzPayload_t xyz){
      unencoded_packet[0] = 0x22;
      unencoded_packet[1] = 0x09;
      unencoded_packet[2] = 0x56;
      unencoded_packet[3] = 0x19;
      unencoded_packet[4] = 0x48;
      unencoded_packet[5] = 'X';
      unencoded_packet[6] = 'Y';
      unencoded_packet[7] = 'Z';
      unencoded_packet[8] = xyz.x / 100;
      unencoded_packet[9] = (xyz.x/10)%10;
      unencoded_packet[10] = xyz.x % 10;
      unencoded_packet[11] = xyz.y / 100;
      unencoded_packet[12] = (xyz.y/10)%10;
      unencoded_packet[13] = xyz.y % 10; 
      unencoded_packet[14] = xyz.z /10;
      unencoded_packet[15] = xyz.z % 10;  
      // debug_log("%d",unencoded_packet[8]);
      // debug_log("%d",unencoded_packet[9]);
      // debug_log("%d",unencoded_packet[10]);
      // debug_log("%d",unencoded_packet[11]);
      // debug_log("%d",unencoded_packet[12]);
      // debug_log("%d",unencoded_packet[13]); 
      // debug_log("%d",unencoded_packet[14]);
      // debug_log("%d",unencoded_packet[15]); 
      debug_log("xyz packet prepared");         
}

void prepare_Rot_Packet(RotPayload_t rot){
      unencoded_packet[0] = 0x23;
      unencoded_packet[1] = 0x09;
      unencoded_packet[2] = 0x56;
      unencoded_packet[3] = 0x19;
      unencoded_packet[4] = 0x48;
      unencoded_packet[5] = 'R';
      unencoded_packet[6] = 'O';
      unencoded_packet[7] = 'T';
      unencoded_packet[8] = rot.angle / 100;
      unencoded_packet[9] = (rot.angle/10)%10;
      unencoded_packet[10] = rot.angle % 10;  
      debug_log("Rot packet prepared");   
}

void prepare_Zoom_Packet(ZoomPayload_t zoom){
      unencoded_packet[0] = 0x25;
      unencoded_packet[1] = 0x09;
      unencoded_packet[2] = 0x56;
      unencoded_packet[3] = 0x19;
      unencoded_packet[4] = 0x48;
      unencoded_packet[5] = 'Z';
      unencoded_packet[6] = 'O';
      unencoded_packet[7] = 'O';
      unencoded_packet[8] = 'M';
      unencoded_packet[9] = zoom.zoomFactor;
      debug_log("Zoom packet prepared");
}

