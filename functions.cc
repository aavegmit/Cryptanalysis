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
		int num_bytes = fwrite(S, 27, 1, fp) ;
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
	if (fp == NULL){
		fprintf(stderr, "File could not be open\n") ;
		return ;
	}

	while(!feof(fp)){
		memset(buffer,'\0', 27) ;
		memset(InvKey, '\0', 27) ;
		int bytes_read = fread(buffer, 27, 1, fp) ;
		if (bytes_read){
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
