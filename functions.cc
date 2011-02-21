#include "functions.h"
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <algorithm>

using namespace std ;

unsigned char S[27] ;
int ii = 0, jj = 0 ;

// Function definition of method keygen
unsigned char *RC4_Init(){

	memset(S,'\0', 27) ;

	// Find the localtime
	struct timeval tv ;
	gettimeofday(&tv, NULL) ;

	char key[20] ;
	memset(key, '\0', 20) ;
	sprintf(key, "%ld%ld", tv.tv_sec, tv.tv_usec) ;


	// Find its MD5
	unsigned char MD5_key[16] ;
	MD5((unsigned char *)key, strlen(key), (unsigned char *)MD5_key) ;


	// Initialize RC4
	for (int i = 0; i < 26; i++)
		S[i] = i + 97;

	for (int i = 0 ,j = 0; i < 26; i++) {
		j = (j + MD5_key[i % 16] + S[i]) % 26 ;
		unsigned char temp = S[i] ;
		S[i] = S[j] ;
		S[j] = temp ;
	}
	return S ;


}


// Method to initialize RC4 and then to
// write the output in the file
void Keygen(int period){

	for (int i = 0; i < period ; ++i){
		RC4_Init() ;
		// Write this permutation in the file
		S[26] = '\0' ;
		printf("%s\n", S) ;

	}


}



void InvKeygen(char *inpF){
	unsigned char InvKey[27] ;
	unsigned char buffer[27] ;

	// Open the file
	FILE *fp ;
	fp = fopen(inpF, "rb") ;
	// Check if the file could be opened or not
	if (fp == NULL){
		fprintf(stderr, "File could not be open\n") ;
		return ;
	}

	// Loop thourgh the file and read all the lines
	while(!feof(fp)){

		// Clear out the buffers
		memset(buffer,'\0', 27) ;
		memset(InvKey, '\0', 27) ;

		// Read one line
		int bytes_read = fread(buffer,1 , 27, fp) ;
		if (bytes_read){

			// If last character is not a new line then report error and exit
			if(buffer[26] != '\n'){
				fprintf(stderr, "Malformed keyfile\n");
				return ;
			}
			for(int i=0 ; i < 26; ++i){
				InvKey[buffer[i] - 97] = i+97 ;
			}
			printf("%s\n", InvKey) ;

		}
	}

	fclose(fp) ;

}



void Crypt(char *keyfile, char *inpF){
	unsigned char buffer[27] ;
	unsigned char outBuf[27] ;

	// Find the period T
	FILE *fp ;
	fp = fopen(keyfile, "rb") ;
	// Check if hte file could be opened or not
	if (fp == NULL){
		fprintf(stderr, "File could not be opened\n") ;
		return ;
	}

	int period = 0 ;

	while(!feof(fp)){
		// Clear out the buffers
		memset(buffer,'\0', 27) ;
		int bytes_read = fread(buffer, 1, 27, fp) ;
		if (bytes_read){
			// If last character is not a new line then report error and exit
			if(buffer[26] != '\n'){
				fprintf(stderr, "Malformed keyfile\n");
				return ;
			}
			++period ;
		}
	}


	// Allocate 2D array to hold the key
	unsigned char **key ;
	key = (unsigned char **)malloc(period*sizeof(unsigned char *)) ;
	for (int i = 0; i < period; ++i){
		key[i] = (unsigned char *)malloc(27) ;
		memset(key[i], '\0', 27) ;
	}


	// Seek to the begining of the keyfile
	if (fseek(fp, 0, SEEK_SET) == -1) {
		fprintf(stderr, "FSEEK failed\n") ;
		return;
	}

	// Load the key in the buffer
	for(int i = 0; i < period; ++i)
	{
		int bytes_read = fread(key[i], 1, 27, fp) ;
		if (bytes_read){
			// If last character is not a new line then report error and exit
			if(key[i][26] != '\n'){
				fprintf(stderr, "Malformed keyfile\n");
				return ;
			}
			key[i][26] = '\0' ;
		}
	}

	// Open the crypt file
	FILE *ifp ;
	ifp = fopen(inpF, "rb") ;
	if (ifp == NULL){
		fprintf(stderr, "Crypt File could not be opened\n") ;
		return ;
	}

	int periodCtr = 0 ;
	int breakF = 0 ;

	// Loop over the file
	while(!feof(ifp)){
		// Clear out the buffers
		memset(buffer,'\0', 27) ;
		memset(outBuf,'\0', 27) ;
		int bytes_read = fread(buffer, 1, 26, ifp) ;
		if(bytes_read){
			buffer[bytes_read] = '\0' ;
			outBuf[bytes_read] = '\0' ;
			for(int i = 0; i < bytes_read; ++i){
				if((buffer[i] < 0x20 || buffer[i] > 0x7e) && (buffer[i] != '\n') ){
					fprintf(stderr, "Malformed plain text file\n") ;
					return;
				}
				if(buffer[i] == '\n'){
					// flush out the outbuf
					breakF = 1 ;
					break ;
				}
				if(buffer[i] >= 0x61 && buffer[i] <= 0x7a){
					outBuf[i] = key[periodCtr % period][buffer[i] - 97] ;
				}
				else{
					// Not a lowercase english alphabet
					outBuf[i] = buffer[i] ;
				}

				++periodCtr ;
			}
			// flush out the outBuf
			printf("%s", outBuf) ;
			if (breakF)
				break ;

		}

	}
	printf("\n") ;



}




void Histo(int period, int which, char *inpFile){
	double freq[26];

	for (int i = 0; i < 26; ++i)
		freq[i] = 0 ;

	unsigned char c;
	int L = 0;

	// Open the file
	FILE *fp;
	fp = fopen(inpFile, "rb") ;
	if (fp == NULL){
		fprintf(stderr, "File could not be opened\n") ;
		return;
	}

	// FSEEK to the which index
	if (fseek(fp, which-1, SEEK_SET) == -1){
		fprintf(stderr, "FSEEK failed\n") ;
		return;
	}

	while(!feof(fp)){
		int bytes_read = fread(&c, 1, 1, fp) ;

		if(bytes_read){
			if(c >= 0x61 && c <= 0x7a){
				++L ;
				++freq[c - 97] ;
			}
			if (fseek(fp, period-1, SEEK_CUR) == -1){
				fprintf(stderr, "FSEEK failed\n") ;
				return;
			}

		}
	}



	double tempFreq[26][2] ;
	int tempFreq1[26] ;
	for (int i =0; i < 26; ++i){
		tempFreq[i][0] = freq[i] ;
		tempFreq[i][1] = 0 ;
		tempFreq1[i] = 0 ;
	}

	double temp = L ;
	int tempI = 0 ;
	for (int j = 0; j < 26 ; ++j){
		temp = L ;
		for (int i = 0; i < 26; ++i){
			if (tempFreq[i][0] <= temp && !tempFreq[i][1]){
				temp = tempFreq[i][0] ;
				tempI = i ;
			}

		}
		tempFreq1[j] = tempI ;
		tempFreq[tempI][1] = 1 ;
	}

	sort(freq, freq + 26) ;

	printf("L=%d\n", L) ;
	for (int i =25; i >= 0; --i){
		printf("%c: %.0f (%.2f%%)\n", tempFreq1[i] + 97, freq[i], (freq[i]*100/L)) ;
	}


}

int checkStr(char *str){
	int i = 0 ;
	while(str[i] != '\0'){
		if(str[i] < 0x61 || str[i] > 0x7a )
			return 1;
		++i ;
	}
	return 0;
}

void Solve(int max_t, char *inpF){

	// Open the file
	FILE *fp = fopen(inpF, "rb") ;
	if (fp == NULL){
		fprintf(stderr, "File could not be opened\n") ;
		return;
	}

	// Load the file into a buffer
	unsigned char *buffer;

	// Allocate 256 bytes to the buffer
	buffer = (unsigned char *)malloc(257) ;
	memset(buffer, 0, 257) ;

	int ctr = 0 ;
	while(!feof(fp)){
		int bytes_read = fread(&buffer[ctr], 1, 256, fp) ;
		if (bytes_read){
			ctr = ctr + bytes_read ;
			// End of file has reached
			if (buffer[ctr-1] == '\n'){
				buffer[ctr-1] = '\0' ;
				break ;
			}
			// Still more data to come
			else{
				// Reallocate the buffer
				buffer = (unsigned char *)realloc(buffer, 257 +ctr ) ;
			}
		}

	}
	buffer[ctr] = '\0' ;

//	printf("%s\n", buffer) ;

	// Kasiski Method
	printf("Kasiski Method\n==============\n");
	int len = 4 ;
	char strI[ctr/2], strJ[ctr/2] ;
	memset(strI, '\0', ctr/2) ;
	memset(strJ, '\0', ctr/2) ;
	int foundF = 0 ;
	for (len = 4; len < ctr/2 ; ++len){
		foundF = 0 ;
		for(int i = 0; i < ctr - 2*len; ++i){
			strncpy(strI, (char *)&buffer[i], len) ;
			strI[len] = '\0' ;
			if(checkStr(strI))
				continue ;
			for(int j = i + len; j < ctr - len; ++j){
				strncpy(strJ, (char *)&buffer[j], len) ;
				strJ[len] = '\0' ;
				if(checkStr(strJ))
					continue;
				if(strcmp(strI, strJ) == 0){
					foundF = 1 ;
					printf("len=%d, i=%d, j=%d, j-i=%d, %s\n", len, i, j, j-i, strI) ;
				}

			}

		}
		if(!foundF){
			printf("len=%d, no more matches\n\n", len) ;
			break ;
		}
	}


	// Average Index of Coincidence
	printf("Average Index of Coincidence\n") ;
	printf("============================\n") ;
	double clen = 0 ;
	int freq[26] ;
	for (int i = 0; i < 26; ++i)
		freq[i] = 0 ;

	for (int i = 0; i < ctr; ++i){
		if(buffer[i] < 0x61 || buffer[i] > 0x7a)
			continue;
		++clen;
		++freq[buffer[i] - 97] ;
	}
	printf("L=%d\n", (int)clen) ;
	for(int i = 0; i < 26; ++i){
		printf("f('%c')=%d\n",i+97, freq[i] ) ;
	}

	long double ic = 0 ;
	for (int i = 0; i < 26; ++i){
		ic += freq[i] * (freq[i] - 1) ;
	}
	printf("IC=%0.9Lf\n", ic/(clen * (clen-1) )) ;

	for (double i = 1; i <= max_t; ++i){
		double eic = 0.0658/i *((clen-i)/(clen-1) ) + (i-1)/i * (clen/(clen-1))*(1.0/26) ;
		printf("t=%d, E(IC)=%.8g\n",(int)i, eic) ;
	}
	printf("\n") ;




	// Auto-Correlation method
	printf("Auto-correlation Method\n");
	printf("=======================\n") ;
	int cnt = 0 ;
	for(int i = 1; i <= max_t ; ++i){
		cnt = 0;
		for(int j = 0; j < ctr - i; ++j){
			if (buffer[j] >= 0x61 && buffer[j] <= 0x7a)
				if(buffer[j] == buffer[j+i])
					++cnt ;
		}
		printf("t=%d, count=%d\n",i, cnt) ;
	}

	printf("\n") ;

}



