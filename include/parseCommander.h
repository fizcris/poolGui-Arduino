///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             INCLUDES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             DEFINES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Commands         ID       DIRECTION           DECRIPTION      Motes: //MAX 255 (1 byte)!! Forbidden: 138 139 32 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CMD_STATE_STOP           1   //   RPI -> ESP32       STATE STOP STATUS (ON, OFF)  
#define CMD_STATE_POOL           2   //   RPI -> ESP32       STATE POOL STATUS (ON, OFF)   
#define CMD_STATE_FLOOR          3   //   RPI -> ESP32       STATE FLOOR STATUS (ON, OFF)    
#define CMD_STATE_HOT_SERIES     4   //   RPI -> ESP32       STATE HOT_SERIES STATUS (ON, OFF)   
#define CMD_STATE_HOT_PARALELL   5   //   RPI -> ESP32       STATE HOT_PARALELL STATUS (ON, OFF)  
#define CMD_STATE_COLD_SERIES    6   //   RPI -> ESP32       STATE COLD_SEERIES STATUS (ON, OFF)  
#define CMD_STATE_COLD_PARALELL  7   //   RPI -> ESP32       STATE STATE_PARALELL STATUS (ON, OFF) 
#define CMD_STATE_EMERGENCE      8   //  TODO RPI -> ESP32       STATE STATE_EMERGENCESTATUS (ON, OFF)  

#define CMD_TEMP_POOL            9   //   RPI -> ESP32       STATE STATE_EMERGENCESTATUS (ON, OFF)
#define CMD_TEMP_FLOOR           10   //  RPI -> ESP32      STATE STATE_EMERGENCESTATUS (ON, OFF)

#define CMD_VK_1                 20  //   TODO RPI -> ESP32      VALVE NA (ON, OFF)
#define CMD_VK_2                 21  //   TODO RPI -> ESP32       VALVE 3 WAY (ON, OFF)
#define CMD_VK_3                 22  //   TODO RPI -> ESP32       VALVE NC (ON, OFF)
#define CMD_VK_4                 23  //   TODO RPI -> ESP32       VALVE 3 WAY (ON, OFF)
#define CMD_PUMP_POOL_IMP        24  //   TODO RPI -> ESP32       PUMP IMPUSION 1cv (ON, OFF)
#define CMD_PUMP_POOL            25  //   TODO RPI -> ESP32       PUMP HEAT EXCHANGE (ON, OFF)
#define CMD_PUMP_FLOOR           26  //   TODO RPI -> ESP32       PUMP FLOOR (ON, OFF)

#define TEMP_POOL                100  //   ESP32 -> RPI      Temp degrees * 10   
#define TEMP_FLOOR               101  //   ESP32 -> RPI      Temp degrees * 10   
#define TEMP_HEATER              102  //   ESP32 -> RPI      Temp degrees * 10   
#define TEMP_POOL_IMP            103  //   ESP32 -> RPI      Temp degrees * 10   
#define TEMP_FLOOR_IMP           104  //   ESP32 -> RPI      Temp degrees * 10   
#define TEMP_RETURN              105  //   ESP32 -> RPI      Temp degrees * 10 
#define TEMP_SERIES              106  //   ESP32 -> RPI      Temp degrees * 10   

#define PRESS_RETURN             107  //   ESP32 -> RPI      Pressure bar * 10   
#define HG_ROOM                  108  //   ESP32 -> RPI      Hg% * 10 

#define STATE_VK_1               109  //   ESP32 -> RPI       VALVE NA (ON, OFF)
#define STATE_VK_2               110  //   ESP32 -> RPI       VALVE 3 WAY (ON, OFF)
#define STATE_VK_3               111  //   ESP32 -> RPI       VALVE NC (ON, OFF)
#define STATE_VK_4               112  //   ESP32 -> RPI       VALVE 3 WAY (ON, OFF)
#define STATE_PUMP_POOL_IMP      113  //   ESP32 -> RPI       PUMP IMPUSION 1cv (ON, OFF)
#define STATE_PUMP_POOL          114  //   ESP32 -> RPI       PUMP HEAT EXCHANGE (ON, OFF)
#define STATE_PUMP_FLOOR         115  //   ESP32 -> RPI       PUMP FLOOR (ON, OFF)

#define DESIRED_TEPM_POOL        116  //   ESP32 -> RPI      Temp degrees * 10 
#define DESIRED_TEMP_FLOOR       117  //   ESP32 -> RPI      Temp degrees * 10 
#define DESIRED_STATE            118  //   ESP32 -> RPI      Desired state [1-7]

/*************************************************************************************************************************************************/
//NAME:         parseSerialCommand
//DESCRPTION:   Parse input command and execute action
//INPUT:        
//RETURN:       
//NOTE:
/*************************************************************************************************************************************************/
void parseSerialCommand(){

    if(Serial3.available() > 0)
    {
        byte receivedByte = (byte)Serial3.read();

        //Serial.println(receivedByte);
        if(receivedByte == FRAME_ESCAPE_CHAR)
        {
            g_xorValue = FRAME_XOR_CHAR;
            //Serial.println("FRAME_ESCAPE_CHAR");
        } else
        {
            receivedByte ^= g_xorValue;
            g_xorValue = 0x00;
            

            switch(g_ReceiverStatus)
            {
                case RCV_ST_IDLE:
                {
                    //Serial.println("RCV_ST_IDLE");
                    if(receivedByte == FRAME_START)
                    {   
                        //Serial.println("FRAME_START");
                        g_BufferIndex = 0;
                        g_InputBuffer[g_BufferIndex++] = receivedByte;
                        g_Checksum = receivedByte;
                        g_ReceiverStatus = RCV_ST_CMD;
                    }
                } break;

                case RCV_ST_CMD:
                {
                    //Serial.println("RCV_ST_CMD");
                    g_InputBuffer[g_BufferIndex++] = receivedByte;
                    g_Checksum += receivedByte;
                    g_ReceiverStatus = RCV_ST_DATA_LENGTH;
                } break;
    
                case RCV_ST_DATA_LENGTH:
                {
                    //Serial.println("RCV_ST_DATA_LENGTH");
                    g_DataLength = receivedByte;
                    if(g_DataLength > 0)
                    {   
                        // if (g_DataLength>LENGTH_IN_DATA_BUFFER) {
                        //     g_ReceiverStatus = RCV_ST_IDLE; 
                        //     break; 
                        // }
                        g_InputBuffer[g_BufferIndex++] = receivedByte;
                        g_Checksum += receivedByte;
                        g_ReceiverStatus = RCV_ST_DATA;
                    } else
                    {   
                        g_ReceiverStatus = RCV_ST_IDLE;
                    }
                } break;
    
                case RCV_ST_DATA:
                {
                    //Serial.println("RCV_ST_DATA");
                    g_InputBuffer[g_BufferIndex++] = receivedByte;
                    g_Checksum += receivedByte;
                    if(--g_DataLength == 0)
                        g_ReceiverStatus = RCV_ST_CHECKSUM;
                } break;
    
                case RCV_ST_CHECKSUM:
                {
                    //Serial.println("RCV_ST_CHECKSUM");
                    if(receivedByte == g_Checksum)
                    {   
                        g_ReceiverStatus = RCV_ST_IDLE;
                        g_InputBuffer[g_BufferIndex++] = receivedByte;
                        // Print buffer serial PC
                        // for(int i=0; i<sizeof(g_InputBuffer); i++){
                            
                        //     Serial.print(g_InputBuffer[i]);
                        //     Serial.print(", ");
                            
                        // }
                        // Serial.println("");
                        //Serial.println("CMD");
                        
                        switch(g_InputBuffer[INDEX_CMD])
                        {
                            case CMD_STATE_STOP:
                            {
                                serialWD.reset();
                                scenario_STOP();                                
                            } break;

                            case CMD_STATE_POOL:
                            {
                                 serialWD.reset();
                                 scenario_pool();                                
                            } break;

                            case CMD_STATE_FLOOR:
                            {
                                serialWD.reset();
                                scenario_floor();
                            } break; 

                            case CMD_STATE_HOT_SERIES:
                            {   
                                serialWD.reset();
                                scenario_hot_series();
                            } break; 
                            case CMD_STATE_HOT_PARALELL:
                            {   
                                serialWD.reset();
                                scenario_hot_paralell();
                            } break; 
                            case CMD_STATE_COLD_SERIES:
                            {
                                serialWD.reset();
                                scenario_cold_series();
                            } break; 
                            case CMD_STATE_COLD_PARALELL:
                            {
                                serialWD.reset();
                                scenario_cold_paralell();
                            } break; 
                            case CMD_TEMP_POOL:
                            {
                                serialWD.reset();
                                desiredTempPool =  GetDataWord(g_InputBuffer);
                            }break; 
                            case CMD_TEMP_FLOOR:
                            {
                                serialWD.reset();
                                desiredTempFloor = GetDataWord(g_InputBuffer);
                            }break; 
                        }
                    } else {
                        g_ReceiverStatus = RCV_ST_IDLE;
                        break;
                        }
                } break;
            }
        }
    }
}