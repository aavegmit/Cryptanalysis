#include "functions.h"
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <openssl/md5.h>

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
	FILE *fp ;

	// Open the file to write
	fp = fopen("keygen.key", "wb") ;
	if(fp == NULL){
		fprintf(stderr, "File could not be opened\n") ;
		return ;
	}
	
	
	for (int i = 0; i < period ; ++i){
		RC4_Init() ;
		// Write this permutation in the file
		S[26] = '\n' ;
		int num_bytes = fwrite(S, 1, 27, fp) ;
		if (num_bytes < 0){
			fprintf(stderr, "File write error\n") ;
			return ;
		}

	}

	fclose(fp) ;

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










