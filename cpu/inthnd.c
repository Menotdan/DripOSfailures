#include "../drivers/screen.h"

void exception_handler() {
    //A basic exception handler for every exception
    //TODO: Add a different handler for each exception
    kprint("An exception has occured");
}