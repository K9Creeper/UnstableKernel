#include "../kernel/terminal/terminal.hpp"

extern "C" void userspace_entry(void){
    Kernel::Terminal::WriteString("Hey! I'm in ring3!\n");

    for(;;){

    }
}