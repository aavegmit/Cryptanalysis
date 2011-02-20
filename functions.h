#include <stdio.h>
#include <iostream>

using namespace std ;

// Function declaration goes here
void Keygen(int period) ;	// Generates a keyfile with specified period to
				// be used by the full Vigenere Cipher


void InvKeygen(char *inpF) ;	// Reads the input key file and generates
				// its inverse

void Crypt(char *, char *) ;	// Encrypts (or decrypts) an input ASCII text 
				// file with the keys specified in keyfile. 
				// The output is an ASCII text file encrypted 
				// (or decrypted) using a full Vigenère cipher 
				// (with polyalphabetic substitution).
 
