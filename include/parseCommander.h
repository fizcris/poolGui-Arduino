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
#define CMD_STATE_EMERGENCE      8   //   ESP32 -> RPI       STATE STATE_EMERGENCESTATUS (ON, OFF)  

#define CMD_TEMP_POOL            9   //   RPI -> ESP32       Temp degrees * 10 
#define CMD_TEMP_FLOOR           10   //  RPI -> ESP32       Temp degrees * 10 

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

    int dataBytesReceived = 0;
    byte g_InputBuffer[LENGTH_INPUT_BUFFER];            // Incoming data buffer
  
    while(Serial3.available() > 0)
    //Serial.println("Serial availiable");
    {
        byte receivedByte = (byte)Serial3.read();

        //Serial.print(receivedByte, HEX);
        //Serial.print("/");
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
                    dataBytesReceived = 0;
                } break;
    
                case RCV_ST_DATA_LENGTH:
                {
                    //Serial.println("RCV_ST_DATA_LENGTH");

                    g_DataLength = receivedByte;
                    g_DataLengthFixed = g_DataLength;
                    if(g_DataLength > 0 && g_DataLength <= LENGTH_MAX_IN_DATA_BUFFER)
                    {   
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
                    dataBytesReceived++;
                    if (dataBytesReceived>LENGTH_MAX_IN_DATA_BUFFER) {
                        Serial.println("Bytes discard");
                        //Print buffer to serial
                        for(int i=0; i<sizeof(g_InputBuffer); i++){
                            Serial.print(g_InputBuffer[i], HEX);
                            Serial.print(", ");
                        }

                        g_BufferIndex = 0;           // Incoming data buffer
                        memset (g_InputBuffer, 0, sizeof(g_InputBuffer));
                        g_ReceiverStatus = RCV_ST_IDLE; 
                        break; 
                    }
                    g_InputBuffer[g_BufferIndex++] = receivedByte;
                    g_Checksum += receivedByte;
                    if(--g_DataLength == 0)
                        g_ReceiverStatus = RCV_ST_CHECKSUM;
                } break;
    
                case RCV_ST_CHECKSUM:
                {
                    // Serial.println("******");
                    // Serial.println(g_BufferIndex);
                    // Serial.println(g_DataLengthFixed + FRAME_NUM_EXTRA_BYTES -1);
                    // Serial.println("******");
                    if(receivedByte == g_Checksum && g_BufferIndex == (g_DataLengthFixed + FRAME_NUM_EXTRA_BYTES -1))
                    {   
                        g_ReceiverStatus = RCV_ST_IDLE;
                        g_InputBuffer[g_BufferIndex++] = receivedByte;
                        
                        //Serial.println("Read OK");
                        
                        switch(g_InputBuffer[INDEX_CMD])
                        {
                            case CMD_STATE_STOP:
                            {
                                SWDreset();
                                updateScenario(00);                               
                            } break;

                            case CMD_STATE_POOL:
                            {
                                 SWDreset();
                                 updateScenario(10);                                
                            } break;

                            case CMD_STATE_FLOOR:
                            {
                                SWDreset();
                                updateScenario(20); 
                            } break; 

                            case CMD_STATE_HOT_SERIES:
                            {   
                                SWDreset();
                                updateScenario(30); 
                            } break; 

                            case CMD_STATE_HOT_PARALELL:
                            {   
                                SWDreset();
                                updateScenario(40); 
                            } break; 

                            case CMD_STATE_COLD_SERIES:
                            {
                                SWDreset();
                                updateScenario(50); 
                            } break; 

                            case CMD_STATE_COLD_PARALELL:
                            {
                                SWDreset();
                                updateScenario(60); 
                            } break; 

                            case CMD_TEMP_POOL:
                            {
                                SWDreset();
                                desiredTempPool =  GetDataWord(g_InputBuffer);
                            }break; 

                            case CMD_TEMP_FLOOR:
                            {
                                SWDreset();
                                desiredTempFloor = GetDataWord(g_InputBuffer);
                            }break; 
                        }
                    } else 
                    {
                    g_ReceiverStatus = RCV_ST_IDLE;
                    break;
                    }
                } break;
            }
        }
    }
}