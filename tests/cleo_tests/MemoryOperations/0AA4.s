�0AA4� !"0AA4 get_dynamic_library_procedure �
p��    �
   kernel32.dll  �  �
Sleep  M H���!8~g~~h~~h~0AA4 (get_dynamic_library_procedure), #0 PASSED  ���� !Q~r~~h~~h~~h~0AA4 (get_dynamic_library_procedure), #0 FAILED! Can't find procedure �
FLAG   SRC �  {$CLEO .s}
{$USE debug, memory}

script_name "0AA4"
debug_on

trace "0AA4 get_dynamic_library_procedure"

const LOAD_LIBRARY = 0x00858070
int load_library_addr = read_memory LOAD_LIBRARY 4 false

int kernel_dll_addr = call_function_return {address} load_library_addr {numParams} 1 {pop} 0 {funcParams} "kernel32.dll" // tested opcode
if
    int sleep_addr = get_dynamic_library_procedure {procName} "Sleep" {DynamicLibrary} kernel_dll_addr
then
    trace "~g~~h~~h~0AA4 (get_dynamic_library_procedure), #0 PASSED"
else
    breakpoint "~r~~h~~h~~h~0AA4 (get_dynamic_library_procedure), #0 FAILED! Can't find procedure"
end



terminate_this_custom_script
  __SBFTR 