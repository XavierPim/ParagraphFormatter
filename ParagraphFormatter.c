
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prints out words with as close to even spacing in between each word.
void printSpacedOutWords(char *charBuffer, int readToIndex, int baseSpaces, int extraSpaces) {
  for (int i = 0; i < readToIndex; i++) {
    if (charBuffer[i] == ' ') {
      for (int j = 0; j < baseSpaces; j++) {
        printf(" ");
      }
      if (extraSpaces != 0) {
        printf(" ");
        extraSpaces--;
      }
    }
    if (charBuffer[i] != ' ') {
      printf("%c", charBuffer[i]);
    }
  }
}

// Prints out a word with as close to even spacing on both sides
void printWordCentred(char *charBuffer, int readToIndex, int extraSpaces) {
  int spaceBefore = extraSpaces / 2;
  int spaceAfter = extraSpaces / 2;

  //Checks if there are extra spaces
  if (extraSpaces % 2) {
    spaceBefore++;
  }
 //Counts if spaces before
  for (int i = 0; i < spaceBefore; i++) {
    printf(" ");
  }

  for (int i = 0; i < readToIndex; i++) {
    printf("%c", charBuffer[i]);
  }

  for (int i = 0; i < spaceAfter; i++) {
    printf(" ");
  }
}

// Do NOT change this function's signature including spaces
void formatAndPrintParagraph(FILE *fptr, int lineLength){
//Checks if file is empty
  if (fptr == NULL) {
    printf("ERROR: File does not exist");
    exit(1);
  }
//detects if lineLength is lower than 0
  if (lineLength <= 0) {
    printf("ERROR: invalid line length, must be greater than 0");
    exit(1);
  }

  char c = fgetc(fptr);

  char charBuffer[lineLength];
  memset(charBuffer, '\0', lineLength);
  int bufferIdx = 0;
  int wordCount = 0;
  int currWordLength = 0;
  int charCounter = 0;
  char prevChar = ' ';
  int newlineFlag = 0;
  int baseSpaces = 0;
  int extraSpaces = 0;

  while (c != EOF) {

    if (bufferIdx < lineLength) {
      // check if previous char was a space
      if (c == ' ' && (c == prevChar || prevChar == '\n')) {
        //Skip
      } else if (c == '\n' && prevChar == ' '){
        newlineFlag = 1;
        bufferIdx--;
      } else if (c == ' ') {
        charBuffer[bufferIdx] = c;
        bufferIdx++;
        wordCount++;
        currWordLength = 0;
      } else if (c == '\n') {
        newlineFlag = 1;
        wordCount++;
        currWordLength = 0;
      }else {
        charBuffer[bufferIdx] = c;
        bufferIdx++;
        currWordLength++;
        charCounter++;
      }
    }

    // If buffer is full or a newline is hit
    if (bufferIdx == lineLength || newlineFlag) {
      c = fgetc(fptr);

      if (charCounter == lineLength && (c != ' ' && c != '\n')) {
        printf("ERROR: word length is longer than line length");
        exit(1);
      }

      if (charCounter == 0) {
        fseek(fptr, -(currWordLength + 1), SEEK_CUR);
        if (prevChar == '\n') {
          printf("\n");
        }
      }
      else if (c != ' ' && c != '\n') {
        // point back to the start of the current word
        fseek(fptr, -(currWordLength + 1), SEEK_CUR);

        charCounter -= currWordLength;
        extraSpaces = (lineLength - charCounter) % (wordCount - 1);

        if (wordCount == 1) {
          printWordCentred(charBuffer, bufferIdx - currWordLength, extraSpaces);
        } else {
          baseSpaces = (lineLength - charCounter) / (wordCount - 1);
          printSpacedOutWords(charBuffer, bufferIdx - currWordLength - 1, baseSpaces, extraSpaces);
        }
        printf("\n");
      } else {
        extraSpaces = (lineLength - charCounter) % (wordCount - 1);
        if (wordCount == 1) {
          printWordCentred(charBuffer, bufferIdx - currWordLength, extraSpaces);
        } else {
          for (int i = 0; i < lineLength; i++) {
            printf("%c", charBuffer[i]);
          }
        }
        printf("\n");
      }

      memset(charBuffer, '\0', lineLength);
      bufferIdx = 0;
      newlineFlag = 0;
      currWordLength = 0;
      charCounter = 0;
      wordCount = 0;
    }

    prevChar = c;
    c = fgetc(fptr);
  }
//counts words in each line
  wordCount++;
  extraSpaces = (lineLength - charCounter) % (wordCount - 1);

  if (wordCount == 1) {
    printWordCentred(charBuffer, bufferIdx, extraSpaces);
  } else {
    baseSpaces = (lineLength - charCounter) / (wordCount - 1);
    printSpacedOutWords(charBuffer, bufferIdx, baseSpaces, extraSpaces);
  }
}

// Do NOT touch main function no matter what
int main(int argc, char **argv){
  FILE *fp;

  fp = fopen(argv[1], "r");

  formatAndPrintParagraph(fp, atoi(argv[2]));

  fclose(fp);
  return 0;
}