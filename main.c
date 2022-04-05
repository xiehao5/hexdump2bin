#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int atohex(const char *str, int len)
{
	const char *origin_str = str;
	int ret = 0;
	int sign = 1;

	if(str == NULL || len == 0)
	{
		return 0;
	}

	while(*str == ' ' && str - origin_str <= len)
	{
		str++;
	}

	if(*str == '-')
	{
		sign = -1;
		str++;
	}

	while(((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f')) && str - origin_str <= len)
	{
		if(*str >= '0' && *str <= '9')
		{
			ret = ( ret << 4 ) + *str - '0';	//ret = ret * 16 + *str - '0';
		}
		else if(*str >= 'A' && *str <= 'F')
		{
			ret = ( ret << 4 ) + *str - 'A' + 10;	//ret = ret * 16 + *str - 'A' + 10;
		}
		else if(*str >= 'a' && *str <= 'f')
		{
			ret = ( ret << 4 ) + *str - 'a' + 10;	//ret = ret * 16 + *str - 'a' + 10;
		}
		else
		{

		}
		str++;
	}

	return ret * sign;
}

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
        //参数太少
        printf("参数太少");
        getchar();
        return -1;
    }

    filename_len = strlen(argv[1]);
//    printf("filename_len = %d", filename_len);
    if(strstr(argv[1], ".hexdump") != argv[1] + filename_len - 8)
    {
        //文件名不对
        printf("文件名不对");
        getchar();
        return -2;
    }

    strncpy(newfilename, argv[1], filename_len - 8);
    newfilename[filename_len - 8] = '\0';


    src = fopen(argv[1], "r");
    dst = fopen(newfilename, "wb");





    //src = fopen("1.hexdump", "r");
    //dst = fopen("1.pdf", "wb");

    if(src == NULL || dst == NULL)
    {
        printf("文件打不开");
        getchar();
        return -3;
    }

    fgets(buffer, BUF_SIZE, src);

    if(strstr(buffer, "  Offset: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F") != buffer)
    {
        printf("文件头不对 %s", buffer);
        getchar();
        return -4;
    }

    while(!feof(src))
    {
		fgets(buffer, 9, src);
        //_snscanf_s(buffer, 9, "%x", &offset);
        offset = atohex(buffer, 9);
        //printf("offset 0x%x\n\n", offset);
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
                printf("文件格式错误");
                getchar();
                return -4;
            }

        }
        while(1)
        {
            fgets(buffer, 4, src);

            //printf("%send\n", buffer);

            if(!strcmp(buffer, "   "))
            {
                fgets(buffer, BUF_SIZE, src); //换行
                break;
            }
            else
            {
                //_snscanf_s(buffer, 2, "%x", &data);
                data = atohex(buffer, 3);
                //printf("data: 0x%x\n\n", data);
                //sscanf(buffer, "%x", &data);
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
