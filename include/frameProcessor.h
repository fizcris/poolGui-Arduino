///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             INCLUDES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             DEFINES
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LENGTH_INPUT_BUFFER      60
#define LENGTH_OUT_DATA_BUFFER   50
#define LENGTH_OUT_FRAME_BUFFER  60

#define FRAME_START              0x8A
#define FRAME_ESCAPE_CHAR        0x8B
#define FRAME_XOR_CHAR           0x20
#define FRAME_NUM_EXTRA_BYTES    4 

#define INDEX_START_OF_FRAME     0
#define INDEX_CMD                1
#define INDEX_DATA_LENGTH        2
#define INDEX_FIRST_DATA_BYTE    3

#define RCV_ST_IDLE              0
#define RCV_ST_CMD               1
#define RCV_ST_DATA_LENGTH       2
#define RCV_ST_DATA              3
#define RCV_ST_CHECKSUM          4
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Global Variables
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
byte g_InputBuffer[LENGTH_INPUT_BUFFER];            // Incoming data buffer
byte g_OutFrameBuffer[LENGTH_OUT_FRAME_BUFFER];     // Data buffer for output Frame
byte g_OutBuffer[LENGTH_OUT_DATA_BUFFER];           // Data buffer for output data

byte g_ReceiverStatus = RCV_ST_IDLE;
byte g_xorValue = 0x00;
byte g_Checksum = 0;
int  g_DataLength = 0;
int  g_BufferIndex = 0;
/*************************************************************************************************************************************************/
//NAME:         CalculateChecksum
//DESCRPTION:   Calculate Checksum value 
//INPUT:        (byte) Frame buffer length
//RETURN:       (byte) Checksum value
//NOTE:
/*************************************************************************************************************************************************/
byte CalculateChecksum(byte length)
{
    byte rv = 0, index;
    for(index = 0; index < length; index++)
    {
        rv += g_OutFrameBuffer[index];
    }
    return rv;
}
/*************************************************************************************************************************************************/
//NAME:         GetDataByte
//DESCRPTION:   Get byte data in the frame
//INPUT:        (byte*) Frame Pointer
//RETURN:       (byte) data frame
//NOTE:         
/*************************************************************************************************************************************************/
byte GetDataByte(byte* frame)
{
    byte rv = 0;
    if(frame)
    {
        rv = frame[INDEX_FIRST_DATA_BYTE];
    }
    return rv;
}
/*************************************************************************************************************************************************/
//NAME:         GetDataWord
//DESCRPTION:   Get word data (2 bytes) in the frame
//INPUT:        (byte*) Frame Pointer
//RETURN:       (word) data frame
//NOTE:         
/*************************************************************************************************************************************************/
word GetDataWord(byte* frame)
{
    word rv = 0;
    if(frame)
        rv = ((word)frame[INDEX_FIRST_DATA_BYTE] << 8) | (word)frame[INDEX_FIRST_DATA_BYTE + 1];
    return rv;
}
/*************************************************************************************************************************************************/
//NAME:         GetDataLong
//DESCRPTION:   Get long data (4 bytes) in the frame
//INPUT:        (byte*) Frame Pointer
//RETURN:       (unsigned long) data frame
//NOTE:         
/*************************************************************************************************************************************************/
unsigned long GetDataLong(byte* frame)
{
    unsigned long rv = 0;
    word hiWord = 0, loWord = 0;
    if(frame)
    {
        hiWord = ((word)frame[INDEX_FIRST_DATA_BYTE] << 8) | (word)frame[INDEX_FIRST_DATA_BYTE + 1];
        loWord = ((word)frame[INDEX_FIRST_DATA_BYTE + 2] << 8) | (word)frame[INDEX_FIRST_DATA_BYTE + 3];
        rv = (((unsigned long)(hiWord)) << 16) | (unsigned long)((unsigned long)(loWord)) ;
    }
    return rv;
}
/*************************************************************************************************************************************************/
//NAME:         GetDataPointer
//DESCRPTION:   Get pointer to the first data byte in the frame
//INPUT:        (byte*) Frame Pointer
//RETURN:       (byte*) Pointer to the first data byte in the frame
//NOTE:         
/*************************************************************************************************************************************************/
byte* GetDataPointer(byte* frame)
{
    return frame + INDEX_FIRST_DATA_BYTE;
}
/*************************************************************************************************************************************************/
//NAME:         SendFrame
//DESCRPTION:   Send Frame over serial port
//INPUT:        (byte*) Frame buffer pointer
//              (byte) length
//RETURN:       void
//NOTE:
/*************************************************************************************************************************************************/
void SendFrame(byte* pFrameBuff, int length)
{
    int i;
    byte dataToSend = 0;

    g_OutBuffer[dataToSend++] = FRAME_START;

    for(i = 1; i < length; i++)
    {
        if(pFrameBuff[i] == FRAME_START || pFrameBuff[i] == FRAME_ESCAPE_CHAR)
        {
            g_OutBuffer[dataToSend++] = FRAME_ESCAPE_CHAR;
            g_OutBuffer[dataToSend++] = pFrameBuff[i] ^ FRAME_XOR_CHAR;
        } else
            g_OutBuffer[dataToSend++] = pFrameBuff[i];
    }
    
    Serial3.write(g_OutBuffer, dataToSend);
}
/*************************************************************************************************************************************************/
//NAME:         SendFrameByte
//DESCRPTION:   Send a byte value
//INPUT:        (byte) command id
//              (byte) data to send
//RETURN:       void
//NOTE:
/*************************************************************************************************************************************************/
void SendFrameByte(byte cmd, byte data)
{
    g_OutFrameBuffer[0] = FRAME_START;                         // Start Frame
    g_OutFrameBuffer[1] = cmd;                                     // Comando
    g_OutFrameBuffer[2] = 0x01;                                    // Lunghezza campo dati
    g_OutFrameBuffer[3] = data;                                    // Data
    g_OutFrameBuffer[4] = CalculateChecksum(4);                    // Checksum

    SendFrame(g_OutFrameBuffer, 5);
}
/*************************************************************************************************************************************************/
//NAME:         SendFrameWord
//DESCRPTION:   Send a word value(2 bytes)
//INPUT:        (byte) command id
//              (word) data to send
//RETURN:       void
//NOTE:
/*************************************************************************************************************************************************/
void SendFrameWord(byte cmd, word data)
{
    g_OutFrameBuffer[0] = FRAME_START;                             // Start Frame
    g_OutFrameBuffer[1] = cmd;                                     // Comando
    g_OutFrameBuffer[2] = 0x02;                                    // Lunghezza campo dati
    g_OutFrameBuffer[3] = (data & 0xFF00) >> 8;                    
    g_OutFrameBuffer[4] = data & 0x00FF;
    g_OutFrameBuffer[5] = CalculateChecksum(5);                    // Checksum

    SendFrame(g_OutFrameBuffer, 6);
}
/*************************************************************************************************************************************************/
//NAME:         SendFrameLong
//DESCRPTION:   Send a long value(4 bytes)
//INPUT:        (byte) command id
//              (unsigned long) data to send
//RETURN:       void
//NOTE:
/*************************************************************************************************************************************************/
void SendFrameLong(byte cmd, unsigned long data)
{
    g_OutFrameBuffer[0] = FRAME_START;                              // Start Frame
    g_OutFrameBuffer[1] = cmd;                                      // Comando
    g_OutFrameBuffer[2] = 0x04;                                     // Lunghezza campo dati
    g_OutFrameBuffer[3] = (data & 0xFF000000) >> 24;                // Data
    g_OutFrameBuffer[4] = (data & 0x00FF0000) >> 16;
    g_OutFrameBuffer[5] = (data & 0x0000FF00) >> 8;
    g_OutFrameBuffer[6] = data & 0x000000FF;
    g_OutFrameBuffer[7] = CalculateChecksum(7);                     // Checksum

    SendFrame(g_OutFrameBuffer, 8);
}
/*************************************************************************************************************************************************/
//NAME:         SendFrameBuffer
//DESCRPTION:   Send a data buffer
//INPUT:        (byte) command id
//              (byte*) buffer pointer
//              (byte) buffer length
//RETURN:       void
//NOTE:
/*************************************************************************************************************************************************/
void SendFrameBuffer(byte cmd, byte* pBuff, int length)
{
    int i = 0;
    g_OutFrameBuffer[0] = FRAME_START;                             // Start Frame
    g_OutFrameBuffer[1] = cmd;                                     // Comando
    g_OutFrameBuffer[2] = length;                                  // Lunghezza campo dati
    for(i = 0; i < length; i++)
        g_OutFrameBuffer[i + 3] = pBuff[i];
    g_OutFrameBuffer[length + 3] = CalculateChecksum(length + 3);  // Checksum

    SendFrame(g_OutFrameBuffer, length + 4);
}