#include "common/tboxes.h"
#include "common/ty.h"
#include "common/txor.h"
#include "common/tyboxes.h"
#include <jni.h>
#include <string.h>
#include <stdio.h>

void ShiftRows(unsigned char out[16])
{
    // +----+----+----+----+
    // | 00 | 04 | 08 | 12 |
    // +----+----+----+----+
    // | 01 | 05 | 09 | 13 |
    // +----+----+----+----+
    // | 02 | 06 | 10 | 14 |
    // +----+----+----+----+
    // | 03 | 07 | 11 | 15 |
    // +----+----+----+----+
    unsigned char tmp1, tmp2;

    // 8-bits left rotation of the second line
    tmp1 = out[1];
    out[1] = out[5];
    out[5] = out[9];
    out[9] = out[13];
    out[13] = tmp1;

    // 16-bits left rotation of the third line
    tmp1 = out[2];
    tmp2 = out[6];
    out[2] = out[10];
    out[6] = out[14];
    out[10] = tmp1;
    out[14] = tmp2;

    // 24-bits left rotation of the last line
    tmp1 = out[3];
    out[3] = out[15];
    out[15] = out[11];
    out[11] = out[7];
    out[7] = tmp1;
}



void aes128_enc_wb_final(unsigned char in[16], unsigned char out[16])
{
    unsigned char out2[16] = { 0 };
    int k;
    /*for ( k = 0; k < 16; ++k) {
        out[k] = in[k];
    }*/
    memcpy(out, in, 16);

    int i, j;

    /// Let's start the encryption process now
    for (i = 0; i < 9; ++i)
    {
        ShiftRows(out);

        for (j = 0; j < 4; ++j)
        {
            unsigned int aa = Tyboxes[i][j * 4 + 0][out[j * 4 + 0]];
            unsigned int bb = Tyboxes[i][j * 4 + 1][out[j * 4 + 1]];
            unsigned int cc = Tyboxes[i][j * 4 + 2][out[j * 4 + 2]];
            unsigned int dd = Tyboxes[i][j * 4 + 3][out[j * 4 + 3]];

            out[j * 4 + 0] = (Txor[Txor[(aa >>  0) & 0xf][(bb >>  0) & 0xf]][Txor[(cc >>  0) & 0xf][(dd >>  0) & 0xf]]) | ((Txor[Txor[(aa >>  4) & 0xf][(bb >>  4) & 0xf]][Txor[(cc >>  4) & 0xf][(dd >>  4) & 0xf]]) << 4);
            out[j * 4 + 1] = (Txor[Txor[(aa >>  8) & 0xf][(bb >>  8) & 0xf]][Txor[(cc >>  8) & 0xf][(dd >>  8) & 0xf]]) | ((Txor[Txor[(aa >> 12) & 0xf][(bb >> 12) & 0xf]][Txor[(cc >> 12) & 0xf][(dd >> 12) & 0xf]]) << 4);
            out[j * 4 + 2] = (Txor[Txor[(aa >> 16) & 0xf][(bb >> 16) & 0xf]][Txor[(cc >> 16) & 0xf][(dd >> 16) & 0xf]]) | ((Txor[Txor[(aa >> 20) & 0xf][(bb >> 20) & 0xf]][Txor[(cc >> 20) & 0xf][(dd >> 20) & 0xf]]) << 4);
            out[j * 4 + 3] = (Txor[Txor[(aa >> 24) & 0xf][(bb >> 24) & 0xf]][Txor[(cc >> 24) & 0xf][(dd >> 24) & 0xf]]) | ((Txor[Txor[(aa >> 28) & 0xf][(bb >> 28) & 0xf]][Txor[(cc >> 28) & 0xf][(dd >> 28) & 0xf]]) << 4);
        }
    }

    /// Last round which is a bit different
    ShiftRows(out);

    for ( j = 0; j < 16; ++j)
    {
        unsigned char x = Tboxes_[j][out[j]];
        out[j] = x;
    }
    memcpy(out2, out, 16);
}



int main(int argc, char *argv[])
{
	int i;
	unsigned char out[16] = { 0 };
	unsigned char out2[16] = { 0 };
	unsigned char plain[16];

	char *m ="7768617464757020666F6C6B733F3F3F";// "whatdup folks???";//just some default text in case no input is provided";

	int count;


	if( argc == 2 ) {

		//assuming the input is always 32 characters representing 16 bytes
		m = argv[1];

	   }

	unsigned char val[16]; //assuming the input is 16 bytes

	char * pos= m;
	//let copy into the actual encryption buffer
	for(count = 0; count < 16; count++) {
		sscanf(pos, "%2hhx", &plain[count]);
		pos += 2;
    	}

    // the magic happens here
    aes128_enc_wb_final(plain, out);

//LETS PRINT THE OUTPUT
    for (i = 0; i < 16; i++)
        printf("%02X ", out[i]);
    return 1;
}


void encrypt(char input[], char output[])
{
   

    aes128_enc_wb_final(input, output);


}
