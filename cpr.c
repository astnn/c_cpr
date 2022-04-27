#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <math.h>

#define MAX_CPR 1000
#define MIN_YEAR 1858
#define MAX_YEAR 2057


// int verMod11(char *cpr); // Maybe this one.
uint8_t sexCheck(uint8_t sex, char *cpr);
uint8_t genMod11Cipher(uint8_t *cpr9ciphers);
uint8_t gen7CipherList(uint16_t birthYear, uint8_t *poss7thCipher);
uint16_t generateCprs(char *birthDateAndSex, uint32_t *cprList);
uint8_t verifyInput(char *str);
void input2birthdateAndSex(char *str, char *birthDateAndSex);
uint8_t extractData(char *line, uint8_t);
uint32_t powUint(uint8_t base, uint8_t exp);
uint32_t cprVect2uint32(uint8_t *cprVect);


int main(int argc, char const *argv[]) {
  uint32_t linenumber, cprList[MAX_CPR];
  uint16_t n_cpr;
  uint8_t inputError, sex;
  

  char birthDateAndSex[10];
  char *line;
  size_t lineBufsize = 32;
  size_t len;


  line = (char *)malloc(lineBufsize * sizeof(char));
  if( line == NULL) {
      perror("Unable to allocate buffer for line");
      exit(1);
  }
  
  linenumber = 0;

  while((len = getline(&line,&lineBufsize,stdin)) != -1) {
    linenumber++;
    /* Check formatting of input. Should be DDMMYYYYS */
    if(verifyInput(line) != 0) {
      // Todo: Change to an error?
      printf("Input on line %d could not be read. Input should be of format DDMMYYYYS\n", linenumber);
    }
    
    
    /* Generate possible CPRs */
    n_cpr = generateCprs(line, cprList);
    
    /* Print possible CPRs to stdout */
    for(uint16_t i = 0; i < n_cpr; i++) {
      printf("%010d ",cprList[i]);
    }
    printf("\n");

    /* Print for debugging */
    //printf("%zu characters were read.\n",len);
    //printf("You typed: '%s'\n",buffer);
  }
  
  
  return 0;
}

uint16_t generateCprs(char *dstr, uint32_t *cprList) {
  uint8_t cprVect[10], poss7thCipher[6], len7Cipher, sex;
  uint16_t birthYear, cprCount;
  
  /* Determine sex. 0 for female, 1 for male*/
  if(dstr[8] == 'f' || dstr[8] == 'F') {
    sex = 0;
  } else {
    sex = 1;
  }
  
  /* Extract birthyear */
  birthYear = 1000*(dstr[4]-'0') + 100*(dstr[5]-'0') + 10*(dstr[6]-'0') + (dstr[7]-'0');
  
  /* Convert char str to uint8 vect */
  cprVect[0] = dstr[0]-'0';
  cprVect[1] = dstr[1]-'0';
  cprVect[2] = dstr[2]-'0';
  cprVect[3] = dstr[3]-'0';
  cprVect[4] = dstr[6]-'0';
  cprVect[5] = dstr[7]-'0';
  
  len7Cipher = gen7CipherList(birthYear, poss7thCipher);
  
  cprCount = 0;
  for(uint8_t i = 0; i < len7Cipher; i++) {
    cprVect[6] = poss7thCipher[i];
    for(uint8_t j = 0; j < 10; j++) {
      cprVect[7] = j;
      for(uint8_t k = 0; k < 10; k++) {
        cprVect[8] = k;
        cprVect[9] = genMod11Cipher(cprVect);
        if(cprVect[9] != -1 && sexCheck(sex, cprVect)) {
          cprList[cprCount++] = cprVect2uint32(cprVect);
        }
      }
    }
  }
  
  return cprCount;
}

/* Checks if the known sex matches the last cipher of the CPR-number. Returns
1 on a match and 0 on no match */
uint8_t sexCheck(uint8_t sex, char *cprVect) {
  return (cprVect[9] % 2) == sex; 
}

/* genMod11Cipher: Returns the modulus 11 control cipher as calculated from
the previous 9 ciphers. Returns -1 (i.e. 255) if no cipher could be constructed
*/
uint8_t genMod11Cipher(uint8_t *cpr9ciphers) {
  uint32_t sum;
  uint8_t rem, res;
  // Make this static and give it as function input? Global?
  uint8_t ctrlVect[10] = {4,3,2,7,6,5,4,3,2,1};
  
  sum = 0;
  for(uint i = 0; i < 9; i++) {
    sum += cpr9ciphers[i]*ctrlVect[i];
  }
  
  rem = sum % 11;
  
  if(rem == 0) {
    res = 0;
  } else {
    res = 11 - rem; // How much should be added to make rem 0?
  }
  
  return (res < 10) ? res : -1;
}

/* cprVect2uint32: Returns the content of the cpr vector as a uint32 */
uint32_t cprVect2uint32(uint8_t *cprVect) {
  uint32_t sum = 0;
  for(uint8_t i = 0; i < 10; i++) {
    sum += cprVect[i]*powUint(10,9-i);
  }
  
  return sum;
}


/* powUint: Power function for unsigned integers. Inputs are interpreted
as base^{exp}. In the implementation below 0^0=1 is defined.
NOTICE: No warning, or error handling, on overflow! */
uint32_t powUint(uint8_t base, uint8_t exp) {
  uint32_t res = 1;
  
  while(exp > 0) {
    res *= base;
    exp--;
  }
  
  return res;
}

uint8_t gen7CipherList(uint16_t birthYear, uint8_t *poss7thCipher) {
  if(birthYear >= MIN_YEAR && birthYear < 1900){
    poss7thCipher[0]=5;
    poss7thCipher[1]=6;
    poss7thCipher[2]=7;
    poss7thCipher[3]=8;
    return 4;
  } else if(birthYear >= 1900 && birthYear < 1937){
    poss7thCipher[0]=0;
    poss7thCipher[1]=1;
    poss7thCipher[2]=2;
    poss7thCipher[3]=3;
    return 4;
  } else if(birthYear >= 1937 && birthYear < 2000){
    poss7thCipher[0]=0;
    poss7thCipher[1]=1;
    poss7thCipher[2]=2;
    poss7thCipher[3]=3;
    poss7thCipher[4]=4;
    poss7thCipher[5]=9;
    return 6;
  } else if(birthYear >= 2000 && birthYear < 2037){
    poss7thCipher[0]=4;
    poss7thCipher[1]=5;
    poss7thCipher[2]=6;
    poss7thCipher[3]=7;
    poss7thCipher[4]=8;
    poss7thCipher[5]=9;
    return 6;
  } else if(birthYear >= 2037 && birthYear < MAX_YEAR){
    poss7thCipher[0]=5;
    poss7thCipher[1]=6;
    poss7thCipher[2]=7;
    poss7thCipher[3]=8;
    return 4;
  } else {
    // Unreachable
    return 0;
  }
}

uint8_t verifyInput(char *str) {
  uint8_t day, month, i;
  uint16_t year;
  for(i = 0; i < 8 && isdigit(str[i]); i++) {
    // No action
  }
  if(i != 8) {
    return 1;
  }
  if( !(str[i] == 'm' || str[i] == 'M' || str[i] == 'f' || str[i] == 'F')) {
    return 2;
  }
  /* Convert to numbers */
  day = 10*(str[0]-'0') + (str[1]-'0');
  month = 10*(str[2]-'0') + (str[3]-'0');
  year = 1000*(str[4]-'0') + 100*(str[5]-'0') + 10*(str[6]-'0') + (str[7]-'0');

  /* Check if range is reasonable */
  if(day > 31 || month > 12 || year < MIN_YEAR || year >= MAX_YEAR) {
    return 3;
  }
  
  return 0;
  
}
