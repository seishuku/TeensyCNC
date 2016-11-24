#ifndef __CDC_H
#define __CDC_H

#ifndef __UserType_CDC_TComData
#define __UserType_CDC_TComData
  typedef uint8_t CDC_TComData ;      /* User type for communication data type. */
#endif

#define CDC_GetFreeInTxBuf() Tx_NofFreeElements()
uint8_t CDC_RecvChar(CDC_TComData *Chr);
uint8_t CDC_SendChar(CDC_TComData Chr);
#define CDC_GetCharsInTxBuf() Tx_NofElements()
uint16_t CDC_GetCharsInRxBuf(void);
uint8_t CDC_Init(void);
uint8_t CDC_App_Task(uint8_t *txBuf, uint16_t txBufSize);
uint8_t CDC_SendString(CDC_TComData *Chr);
#define CDC_GetChar(Chr) Rx_Get(Chr)
uint8_t CDC_PutBufferChecked(uint8_t *buf, uint16_t bufSize);
void CDC_App_Callback(uint8_t controller_ID, uint8_t event_type, void *val);
void CDC_Notify_Callback(uint8_t controller_ID, uint8_t event_type, void *val);
#define CDC_ClearRxBuffer() Rx_Clear()
#define CDC_ClearTxBuffer() Tx_Clear()
uint8_t CDC_SendBlock(uint8_t *data, uint16_t dataSize);
uint8_t CDC_SendDataBlock(uint8_t *data, uint16_t dataSize);
uint8_t CDC_ApplicationStarted(void);
uint8_t CDC_TransactionsStarted(void);

#endif
