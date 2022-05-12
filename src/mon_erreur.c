/**
 * @file mon_erreur.c
 * @author A. Habrard
 * @brief Recopie du fichier
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "../inc/mon_erreur.h"

void mon_erreur (char *format,...)
{
  va_list arg;
  
  va_start(arg,format);

  fprintf(stderr,"Erreur - ");
  
  vfprintf(stderr,format,arg);

  va_end(arg);

  exit(EXIT_FAILURE);
}
