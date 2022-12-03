#include<stdio.h>
#include<stdint.h>

typedef uint8_t bool;
#define true 1
#define false 0
typedef struct 
{
    uint8_t BootJumpInstruction[3];
    uint8_t OemIdentifier[8];
    uint16_t BytesPerSector;
    uint8_t SectorsPerCluster;
    uint16_t ReservedSectors;
    uint8_t FatCount;
    uint16_t DirEntryCount;
    uint16_t TotalSectors;
    uint8_t MediaDescriptorType;
    uint16_t SectorsPerFat;
    uint16_t SectorsPerTrack;
    uint16_t Heads;
    uint32_t HiddenSectors;
    uint32_t LargeSectorCount;

    // extended boot record
    uint8_t DriveNumber;
    uint8_t _Reserved;
    uint8_t Signature;
    uint32_t VolumeId;          // serial number, value doesn't matter
    uint8_t VolumeLabel[11];    // 11 bytes, padded with spaces
    uint8_t SystemId[8];

    // ... we don't care about code ...

} __attribute__((packed)) BootSector;
//上面这句话告诉编译器取消结构在编译过程中的优化对齐,按照实际占用字节数进行对齐
//因为bootsector
BootSector g_BootSector;
bool readBootSector(FILE* disk)
{
    //fread(buffer, size, count, fp);
    //从fp所指向的文件中读取数据块，读取的字节数为size*count
    //读取来的数据存放在buffer为起始地址的内存中。
    //若fread函数的返回值等于count，则执行本函数读取数据成功；
    //若文件结束或发生错误，返回值等于0。
    return fread(&g_BootSector, sizeof((g_BootSector)),1,disk)>0;//
}

bool readSector(FILE* disk,uint32_t lba,uint32_t count,void* bufferOut)
{
    bool ok=true;
    //int fseek(FILE *stream, long offset, int fromwhere);
    //函数设置文件指针stream的位置。
    //如果执行成功，stream将指向以fromwhere为基准，偏移offset（指针偏移量）个字节的位置，函数返回0。
    //如果执行失败(比如offset取值大于等于2*1024*1024*1024，即long的正数范围2G)，
    //则不改变stream指向的位置，函数返回一个非0值。
    ok=ok&&(fseek(disk,lba*g_BootSector.BytesPerSector,SEEK_SET)==0);
    ok = ok && (fread(bufferOut, g_BootSector.BytesPerSector, count, disk) == count);
    return ok;
}
int main(int argc,char** argv)
{
    if(argc < 3)
    {
        printf("Syntax:%s<disk image> <file name>\n",argv[0]);
        return -1;
    }
    FILE* disk=fopen(argv[1], "rb");
    if(!disk)
    {
        fprintf(stderr,"Cannot open disk imag %s!",argv[1]);
    }

    if(!readBootSector(disk))
    {
        fprintf(stderr,"Cannot read boot sector \n");
        return -2;
    }
    return 0;
}