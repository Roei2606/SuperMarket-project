#pragma once

#ifndef MYMACROS
#define MYMACROS


#define CHECK_RETURN_0(ptr) ((ptr) == NULL ? 0 : (ptr))
#define CHECK_MSG_RETURN_0(ptr) ((ptr) == NULL ? (printf("Pointer is NULL!\n"), 0) : (ptr))
#define FREE_CLOSE_FILE_RETURN_0(ptr, file) { free(ptr); fclose(file); return 0; }
#define CLOSE_RETURN_0(file) {fclose(file); return 0; }






#endif  !MYMACROS

