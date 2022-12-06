# STM32F-UART-and-LED-APP

Hello, this repo is a Uart communication application. communication with interrupt. The tasks to be done are as follows; First, the LED is requested to flash constantly. On the basis of multiplication, the second "start" and "stop" information received from the serial port screen. "Start" is echo state that echo every message to serial port with "Uart Transmitter Interrupt". "Stop" is off the echo state and led toggles 1 per second. In addition, If it receives "ledon=xxx" or "ledoff=xxx"(x equal to integer), led on delay time and led off delay time is defined as ms unit.

MPU : STM32F407VGT6 Discovery Board.

UART: USART2
      Baud Rate: 115200,
      WordLength: 8B1
      Parity: NONE
      
To Receive:
  
  IDLE LINE event: Triggered when RX line has been in idle state (normally high state) for 1 frame time, after last received byte. Frame time is based on baudrate.    
  Higher baudrate means lower frame time for single byte.
  
  HAL_UARTEx_ReceiveToIdle_IT():
  
  Receive an amount of data in interrupt mode till either the expected number of data is received or an IDLE event occurs.
  * @note   Reception is initiated by this function call. Further progress of reception is achieved thanks 
    to UART interrupts raised by RXNE and IDLE events. Callback is called at end of reception indicatin number of received data elements.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M = 01), the received data is handled as a set of uint16_t. In this case, Size must indicate the number of uint16_t available through pData.
  * @param huart UART handle.
  * @param pData Pointer to data buffer (uint8_t or uint16_t data elements).
  * @param Size  Amount of data elements (uint8_t or uint16_t) to be received.
  * @retval HAL status
  HAL_UARTEx_ReceiveToIdle_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
  
To Inquary:
  
   Here we do the "if-else" inquiry. Some string commands are used because inquiries are among the series.
   
   strcmp():The strcmp() compares two strings character by character. If the strings are equal, the function returns 0.
   strncmp():The strncmp function returns a negative, zero, or positive integer depending on whether the first n characters of the object pointed to by s1 are less     
   than, equal to, or greater than the first n characters of the object pointed to by s2.
   
Access to project files: https://drive.google.com/drive/folders/1CfXMUDhqTDItDBl-ejTsqo90kI9-vtQD?usp=share_link    
      
  
  
