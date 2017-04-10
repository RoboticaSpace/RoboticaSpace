#include <iostream>
#include <memory.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <wiringPiI2C.h>
#include <stdio.h>
#include <fcntl.h>				//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

using namespace std;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef char  sint8;
typedef short sint16;
typedef long  sint32;

uint8 CalcCheckByte(uint8* data, uint8 start, uint8 number)
{
    uint8 checkByte = 0xFF;

    for (uint8 index = 0; index < number; index++)
    {
        checkByte ^= data[start + index];
    }
    return checkByte;
}

int main(void)
{
    int file_i2c;
    int length;
    unsigned char buffer[60] = { 0 };

    //----- OPEN THE I2C BUS -----
    char *filename = (char*)"/dev/i2c-1";
    if ((file_i2c = open(filename, O_RDWR)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        printf("Failed to open the i2c bus");
        return 0;
    }

    int addr = 0x08;          //<<<<<The I2C address of the slave
    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
    {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        //ERROR HANDLING; you can check errno to see what went wrong
        return 0;
    }

    uint8 data[8];

    data[0] = 0xA1;
    data[1] = 0x90;
    data[2] = 0x70;
    data[3] = 0x08;
    data[4] = 0x01;
    data[5] = 0x02;
    data[6] = 0x0c;
    data[7] = CalcCheckByte(data, 0, 7);

    //----- WRITE BYTES -----
    length = 8;			//<<< Number of bytes to write
    if (write(file_i2c, data, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }

    usleep(1500);

    //----- READ BYTES -----
    length = 4;			//<<< Number of bytes to read
    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    }
    else
    {
        printf("Data read: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }

    data[0] = 0xA1;
    data[1] = 0x21;
    data[2] = 0x00;
    data[3] = 0x01;
    data[4] = CalcCheckByte(data, 0, 4);

    //----- WRITE BYTES -----
    length = 5;			//<<< Number of bytes to write
    if (write(file_i2c, data, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }

    usleep(1500);

    //----- READ BYTES -----
    length = 4;			//<<< Number of bytes to read
    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    }
    else
    {
        printf("Data read: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }

    usleep(2500000);

    data[0] = 0xA1;
    data[1] = 0x41;
    data[2] = CalcCheckByte(data, 0, 2);

    //----- WRITE BYTES -----
    length = 3;			//<<< Number of bytes to write
    if (write(file_i2c, data, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }

    usleep(1500);

    //----- READ BYTES -----
    length = 6;			//<<< Number of bytes to read
    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    }
    else
    {
        printf("Data read: %x %x %x %x %x %x\n", 
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
    }

    short encoder = 0;
    memcpy(&encoder, &buffer[2], sizeof(short));
    printf("LE:%d\n", encoder);

    usleep(2500000);

    data[0] = 0xA1;
    data[1] = 0x90;
    data[2] = 0x70;
    data[3] = 0x08;
    data[4] = 0x01;
    data[5] = 0x02;
    data[6] = 0x0c;
    data[7] = CalcCheckByte(data, 0, 7);

    //----- WRITE BYTES -----
    length = 8;			//<<< Number of bytes to write
    if (write(file_i2c, data, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }

    usleep(1500);

    //----- READ BYTES -----
    length = 4;			//<<< Number of bytes to read
    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    }
    else
    {
        printf("Data read: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }

    data[0] = 0xA1;
    data[1] = 0x21;
    data[2] = 0x00;
    data[3] = 0x00;
    data[4] = CalcCheckByte(data, 0, 4);

    //----- WRITE BYTES -----
    length = 5;			//<<< Number of bytes to write
    if (write(file_i2c, data, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
    }

    usleep(1500);

    //----- READ BYTES -----
    length = 4;			//<<< Number of bytes to read
    if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        //ERROR HANDLING: i2c transaction failed
        printf("Failed to read from the i2c bus.\n");
    }
    else
    {
        printf("Data read: %x %x %x %x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    }

    return 0;
}

#ifdef NONE
#define I2C_SMBUS       0x0720 

struct i2c_smbus_ioctl_data
{
    char read_write;
    char command;
    int size;
    uint8 *data;
};


static inline int i2c_smbus_access(int fd, char rw, char command, int size, uint8 *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = rw;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(fd, I2C_SMBUS, &args);
}


int main()
{
    int fd, result;

    // Initialize the interface by giving it an external device ID.
    // The MCP4725 defaults to address 0x60.   
    //
    // It returns a standard file descriptor.
    // 
    fd = wiringPiI2CSetup(0x08);

    printf("fd:%d\n", fd);

    uint8 data[8];

    data[0] = 0xA1;
    data[1] = 0x90;
    data[2] = 0x70;
    data[3] = 0x08;
    data[4] = 0x01;
    data[5] = 0x02;
    data[6] = 0x0c;
    data[7] = CalcCheckByte(data, 0, 7);

    for (int i = 0; i < 8; i++)
    {
        printf("%d ",wiringPiI2CWrite(fd, data[i]));
    }

    //return 0;

    usleep(1000);

    //i2c_smbus_access(fd, 1, 0, 4, data);

    printf("\n%d %d %d %d\n",
        data[0], data[1], data[2], data[3]);

    //return 0;

    data[0] = 0xA1;
    data[1] = 0x21;
    data[2] = 0x00;
    data[3] = 0x01;
    data[4] = CalcCheckByte(data, 0, 4);

    for (int i = 0; i < 5; i++)
    {
        printf("%d ", wiringPiI2CWrite(fd, data[i]));
    }

    //return 0;

    usleep(1000);

    //i2c_smbus_access(fd, 1, 0, 4, data);

    printf("\n%d %d %d %d\n",
        data[0], data[1], data[2], data[3]);

    return 0;
}
#endif