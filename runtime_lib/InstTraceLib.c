/************
/instTraceLib.c
/  This library is part of the greater LLFI framework
/  This library should be linked against programs that have had the LLFI instTrace LLVM
/  pass performed on them
*************/

#include <stdio.h>
#include <stdlib.h>

#include "Utils.h"
#include "unistd.h"

//Open a file (once) for writing. This file is not explicitly closed, must flush often!
static FILE* ofile = NULL;

extern  long g_flag;
FILE* OutputFile() {
  if (ofile == NULL) {
    ofile = fopen("llfi.stat.trace.txt", "w");
  }
  return ofile;
}



static long instCount = 0;
static long hitcount = 0;
static long cutOff = 0;
void printInstTracer(long instID, char *opcode, int size, char* ptr, int maxPrints) {
  int i;
  instCount++;

  
  if (start_tracing_flag == TRACING_FI_RUN_FAULT_INSERTED) {
    start_tracing_flag = TRACING_FI_RUN_START_TRACING;
    cutOff = instCount + maxPrints;
    //fprintf(TempFile(), "2333333\n");
    //Print faulty trace header (for analysis by traceDiff script)
    fprintf(OutputFile(), "#TraceStartInstNumber: %ld\n", instCount);
  }
  
  //These flags are set by faultinjection_lib.c (Faulty Run) or left 
  // initialized in utils.c and left unchanged (Golden run)
  if ((start_tracing_flag == TRACING_GOLDEN_RUN) || 
      ((start_tracing_flag == TRACING_FI_RUN_START_TRACING) && 
       (instCount < cutOff))) {
    //fprintf(TempFile(), "start_tracing_flag%d\n",start_tracing_flag);
    //fprintf(TempFile(), "TRACING_FI_RUN_START_TRACING%d\n",TRACING_FI_RUN_START_TRACING);
    //fprintf(TempFile(), "start_tracing_flag%d\n",start_tracing_flag);
    //fprintf(TempFile(), "TRACING_GOLDEN_RUN%d\n",TRACING_GOLDEN_RUN);
    
    fprintf(OutputFile(), "ID: %ld\tOPCode: %s\tValue: ", instID, opcode);
    
    //Handle endian switch
    if (isLittleEndian()) {
      for (i = size - 1; i >= 0; i--) {
        fprintf(OutputFile(), "%02hhx", ptr[i]);
      }
    } else {
      for (i = 0; i < size; i++) {
        fprintf(OutputFile(), "%02hhx", ptr[i]);
      }
    }
    fprintf(OutputFile(), "\n");

    fflush(OutputFile()); 

  }
  if ((start_tracing_flag != TRACING_GOLDEN_RUN) && instCount >= cutOff )
  {
	start_tracing_flag = TRACING_FI_RUN_END_TRACING;
  }

}

void postTracing() {
  if (ofile != NULL)
    fclose(ofile);
}

