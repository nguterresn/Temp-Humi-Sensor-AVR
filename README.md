# Temp-Humi-Sensor-AVR
Temperature and humidity sensor using DHT11, an atmega88 and a LCD 16x2. Pretty much this.

### Program

This project was programmed to avr using usbasp in MacOS Mojave.

- You need CrossPack AVR installed.
- avr_compiler.h 
- Makefile must be editted depending on your hardware 

In the terminal, where the files are saved in, first,
```sh
make 
```
Then last,
```sh
make program
```

### Test

USART is enable for tests! 
- Baudrate = 4800

This fuction lets you receive on serial port *data:
```sh
void USART_Transmit(uint8_t *data )
```
