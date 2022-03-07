#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main(int argc, char *argv[])
{
    uint8_t data = 0;
//    uint8_t index = 0;
    uint16_t filename_len = 0;
    uint32_t offset = 0;
    int8_t newfilename[1024];
    int8_t buffer[BUF_SIZE];
//    uint8_t dst_buf[LINE_BUF_SIZE];
    FILE *src = NULL, *dst = NULL;
    if(argc != 2)
    {
        //����̫��
        printf("����̫��");
        getchar();
        return -1;
    }

    filename_len = strlen(argv[1]);
//    printf("filename_len = %d", filename_len);
    if(strstr(argv[1], ".hexdump") != argv[1] + filename_len - 8)
    {
        //�ļ�������
        printf("�ļ�������");
        getchar();
        return -2;
    }

    strncpy(newfilename, argv[1], filename_len - 8);
    newfilename[filename_len - 8] = '\0';


    src = fopen(argv[1], "r");
    dst = fopen(newfilename, "wb");

//    src = fopen("1.hexdump", "r");
//    dst = fopen("1.pdf", "wb");

    if(src == NULL || dst == NULL)
    {
        printf("�ļ��򲻿�");
        getchar();
        return -3;
    }

    fgets(buffer, BUF_SIZE, src);

    if(strstr(buffer, "  Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F") != buffer)
    {
        printf("�ļ�ͷ���� %s", buffer);
        getchar();
        return -4;
    }

    //fgets(buffer, BUF_SIZE, src); //����
    //printf("%s\n", buffer);

    //sscanf(buffer,"%x: %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x", offset, )



    //while(fscanf(src, "%x", &offset) != EOF)
    while(!feof(src))
    {
		fgets(buffer, 9, src);
        sscanf(buffer, "%x", &offset);
        //printf("offset %d\n\n", offset);
        fseek(dst, offset, SEEK_SET);

        fgets(buffer, 3, src);
        if(strcmp(buffer, ": "))
        {
            if(feof(src))
            {
                fclose(src);
                fclose(dst);
                return 0;
            }
            else
            {
				fflush(dst);
                fclose(src);
                fclose(dst);
                printf("�ļ���ʽ����");
                getchar();
                return -4;
            }

        }
        while(1)
        {
            fgets(buffer, 4, src);

            //printf("%saaaa\n", buffer);

            if(!strcmp(buffer, "   "))
            {
                fgets(buffer, BUF_SIZE, src); //����
                break;
            }
            else
            {
                sscanf(buffer, "%x", &data);
                fwrite((const void *)(&data), sizeof(data), 1, dst);
                //fwrite((const void *)(buffer), sizeof(buffer), 1, dst);

            }
        }
    }

	fflush(dst);
    fclose(src);
    fclose(dst);


    printf("Hello world!\n");
    getchar();
    return 0;
}
