#ifndef ERROR_H
#define ERROR_H

#define EXIT_ON_ERROR(cond, args...)  \
if ((cond))                           \
{                                     \
  fprintf(stderr, args);              \
  exit(1);                            \
}                                     \

#endif
