�0A8C� !0A8C (write_memory)   �
     ����  ����  ����
   D3" � 9  ����9  ����9  ����M J���!'~g~~h~~h~0A8C (write_memory), #0 PASSED  ���� !j~r~~h~~h~~h~0A8C (write_memory), #0 FAILED!~n~cccccccc dddddddd eeeeeeee Expected~n~%08x %08x %08x Occured      �
     ����  ����  ����
  D3" � 9  ����9  D���9  ����M >���!'~g~~h~~h~0A8C (write_memory), #1 PASSED  ���� !j~r~~h~~h~~h~0A8C (write_memory), #1 FAILED!~n~cccccccc dddddd44 eeeeeeee Expected~n~%08x %08x %08x Occured      �
     ����  ����  ����
  D3" � 9  ����9  D3��9  ����M 2���!'~g~~h~~h~0A8C (write_memory), #2 PASSED  ���� !j~r~~h~~h~~h~0A8C (write_memory), #2 FAILED!~n~cccccccc dddd3344 eeeeeeee Expected~n~%08x %08x %08x Occured      �
     ����  ����  ����
  D3" � 9  ����9  DDD�9  ����M &���!'~g~~h~~h~0A8C (write_memory), #3 PASSED  ���� !j~r~~h~~h~~h~0A8C (write_memory), #3 FAILED!~n~cccccccc dd444444 eeeeeeee Expected~n~%08x %08x %08x Occured      �
     ����  ����  ����
  D3" � 9  ����9  D3"9  ����M ���!'~g~~h~~h~0A8C (write_memory), #4 PASSED  ���� !j~r~~h~~h~~h~0A8C (write_memory), #4 FAILED!~n~cccccccc 11223344 eeeeeeee Expected~n~%08x %08x %08x Occured      �
     ����  ����  ����
  D3" � 9  ����9  DDDD9  D���M ���!'~g~~h~~h~0A8C (write_memory), #5 PASSED  ���� !i~r~~h~~h~~h~0A8C (write_memory), #5 FAILED! Expected: cccccccc, 44444444, eeeeee44 Actual: %08x %08x %08x      �
     ����  ����  ����
  D3" � 9  ����9  DDDD9  DDD�M ���!'~g~~h~~h~0A8C (write_memory), #6 PASSED  ���� !i~r~~h~~h~~h~0A8C (write_memory), #6 FAILED! Expected: cccccccc, 44444444, ee444444 Actual: %08x %08x %08x      �
       �B�
   �  C     �BM "���!'~g~~h~~h~0A8C (write_memory), #7 PASSED  ���� !H~r~~h~~h~~h~0A8C (write_memory), #7 FAILED!~n~ Expected~n~100.0 Found %f   �
FLAG   SRC   {$CLEO .s}
{$USE debug}
{$USE memory}
var 0@ : Integer
var 1@ : Integer
var 2@ : Integer
var 3@ : Integer
var 4@ : Integer
var 5@ : Integer
var 6@ : Integer
var 7@ : Integer
var 8@ : Integer
var 9@ : Integer
var 10@ : Integer

script_name "0A8C" // write_memory
debug_on

trace "0A8C (write_memory)"

wait 0
// write 0 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 0 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0xdddddddd
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8C (write_memory), #0 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #0 FAILED!~n~cccccccc dddddddd eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// write 1 byte
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 1 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0xdddddd44
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8C (write_memory), #1 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #1 FAILED!~n~cccccccc dddddd44 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// write 2 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 2 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0xdddd3344
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8C (write_memory), #2 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #2 FAILED!~n~cccccccc dddd3344 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// write 3 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 3 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0xdd444444 // memset behavior
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8C (write_memory), #3 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #3 FAILED!~n~cccccccc dd444444 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// write 4 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 4 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x11223344
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8C (write_memory), #4 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #4 FAILED!~n~cccccccc 11223344 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// write 5 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 5 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x44444444
    3@ == 0xeeeeee44
then
    trace "~g~~h~~h~0A8C (write_memory), #5 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #5 FAILED! Expected: cccccccc, 44444444, eeeeee44 Actual: %08x %08x %08x" 1@ 2@ 3@
end


wait 0
// write 7 bytes
get_var_pointer 2@ {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8C: write_memory {address} 0@ {size} 7 {value} 0x11223344 {vp} false // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x44444444
    3@ == 0xee444444
then
    trace "~g~~h~~h~0A8C (write_memory), #6 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #6 FAILED! Expected: cccccccc, 44444444, ee444444 Actual: %08x %08x %08x" 1@ 2@ 3@
end


wait 0

// write float
get_var_pointer 0@ {store_to} 1@
2@ = 100.0

0A8C: write_memory {address} 1@ {size} 4 {value} 2@ {vp} false // tested opcode

if and
    0@ == 100.0
then
    trace "~g~~h~~h~0A8C (write_memory), #7 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8C (write_memory), #7 FAILED!~n~ Expected~n~100.0 Found %f" 0@
end


terminate_this_custom_script
0  __SBFTR 