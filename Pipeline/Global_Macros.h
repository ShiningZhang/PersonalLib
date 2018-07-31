#ifndef GLOBAL_MACROS_H
#define GLOBAL_MACROS_H

#pragma once

#define LOGI(X) printf(X)
#define LOGD(X) printf(X)
#define SP_TRACE(X) printf(X)


// ----------------------------------------------------------------

#    define SP_NEW_RETURN(POINTER,CONSTRUCTOR,RET_VAL) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { return RET_VAL; } \
   } while (0)
#    define SP_NEW(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { return; } \
   } while (0)
#    define SP_NEW_NORETURN(POINTER,CONSTRUCTOR) \
   do { POINTER = new CONSTRUCTOR; \
     if (POINTER == 0) { } \
   } while (0)

// ----------------------------------------------------------------

# define SP_DES(POINTER) \
   do { \
        if (POINTER) \
          { \
            delete (POINTER); \
            POINTER = 0; \
          } \
      } \
   while (0)

# define SP_DES_FREE(POINTER,DEALLOCATOR,CLASS) \
   do { \
        if (POINTER) \
          { \
            (POINTER)->~CLASS (); \
            DEALLOCATOR (POINTER); \
          } \
      } \
   while (0)



#endif // !GLOBAL_MACROS_H


