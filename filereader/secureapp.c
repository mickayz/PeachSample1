#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void usage(){
  printf("Usage:\n");
  printf("./secureapp FILENAME\n\n");
  printf("Prints up to length bytes of ascii data\n");
  printf("Securely designed to not print more data than exists :)\n\n");
  printf("FILE STRUCTURE:\n\n");
  printf("-------------------------------\n");
  printf("|----0B-----|--1B----|---2B---|\n");
  printf("-------------------------------\n");
  printf("|Magic 0x4D | Length(of Data) |\n");
  printf("-------------------------------\n");
  printf("|        DATA (ascii)         |\n");
  printf("|            ....             |\n");
  printf("-------------------------------\n");

  exit(1);
}

main(int argc, const char* argv[])
{
	FILE *file;
	char *buffer;
        char *tempbuf;
        long fileLen;
        char magic;
        short maxlen;
        unsigned int copyamount;

        if (argc != 2){
          usage();
        }

	file = fopen(argv[1], "rb");
	if (!file)
	{
		fprintf(stderr, "can't open file %s", argv[1]);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

        // File must be a minimum of 3 bytes to satisfy header
	if (fileLen<3)
	{
		fprintf(stderr, "Invalid File Format!\n");
                fclose(file);
		exit(1);
	}

        // Temporary buffer for file contents
	tempbuf=(char *)malloc(fileLen);
	if (!tempbuf)
	{
		fprintf(stderr, "Memory error!\n");
                fclose(file);
		exit(1);
	}
	fread(tempbuf, fileLen, 1, file);
	fclose(file);


        magic = (char)tempbuf[0];
        memcpy(&maxlen, &(tempbuf[1]), 2);

        // Magic Number must be set to 0x4D
	if (magic!='M')
	{
		fprintf(stderr, "Invalid magic number!\n");
		exit(1);
	}


        // Data buffer must have room for the size of data,
        // plus one extra byte for the trailing null.
	buffer=(char *)malloc(fileLen-2);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!\n");
		exit(1);
	}

        // If maxlen defined in the file structure
        // is less than the size of the data
        // only copy up to maxlen
        if (fileLen-3>maxlen){
          copyamount = maxlen;
        }
        else{
          copyamount = fileLen-3;
        }

        memcpy(buffer, &(tempbuf[3]), copyamount);
        buffer[copyamount] = '\0';

	printf ("value is %s \n", buffer);
	free(buffer);
        free(tempbuf);
}

