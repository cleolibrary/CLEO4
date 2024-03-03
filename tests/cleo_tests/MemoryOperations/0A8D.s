�0A8D� !0A8D (read_memory)   �
����    ����  ����  ����
     � 9  ����9   9  ����M O���!&~g~~h~~h~0A8D (read_memory), #0 PASSED  ���� !i~r~~h~~h~~h~0A8D (read_memory), #0 FAILED!~n~cccccccc 00000000 eeeeeeee Expected~n~%08x %08x %08x Occured      �
����    ����  ����  ����
    � 9  ����9  D9  ����M H���!&~g~~h~~h~0A8D (read_memory), #1 PASSED  ���� !i~r~~h~~h~~h~0A8D (read_memory), #1 FAILED!~n~cccccccc 00000044 eeeeeeee Expected~n~%08x %08x %08x Occured      �
����    ����  ����  ����
    � 9  ����9  D39  ����M @���!&~g~~h~~h~0A8D (read_memory), #2 PASSED  ���� !i~r~~h~~h~~h~0A8D (read_memory), #2 FAILED!~n~cccccccc 00003344 eeeeeeee Expected~n~%08x %08x %08x Occured      �
����    ����  ����  ����
    � 9  ����9  D3" 9  ����M 6���!&~g~~h~~h~0A8D (read_memory), #3 PASSED  ���� !i~r~~h~~h~~h~0A8D (read_memory), #3 FAILED!~n~cccccccc 00223344 eeeeeeee Expected~n~%08x %08x %08x Occured      �
����    ����  ����  ����
    � 9  ����9  D3"9  ����M ,���!&~g~~h~~h~0A8D (read_memory), #4 PASSED  ���� !i~r~~h~~h~~h~0A8D (read_memory), #4 FAILED!~n~cccccccc 11223344 eeeeeeee Expected~n~%08x %08x %08x Occured           �B   �
   �
   �  C    �BM E���!&~g~~h~~h~0A8D (read_memory), #5 PASSED  ���� !D~r~~h~~h~~h~0A8D (read_memory), #5 FAILED! Expected 125.0, Actual %f  �
D3"some longer testing text HEX    	     FLAG   SRC h  {$CLEO .s}
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

script_name "0A8D" // read_memory
debug_on

trace "0A8D (read_memory)"

wait 0
// read 0 bytes
0AC6: get_label_pointer @DATA {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8D: read_memory {address} 0@ {size} 0 {vp} false {result} 2@ // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x00000000
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8D (read_memory), #0 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #0 FAILED!~n~cccccccc 00000000 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// read 0 bytes
0AC6: get_label_pointer @DATA {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8D: read_memory {address} 0@ {size} 1 {vp} false {result} 2@ // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x00000044
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8D (read_memory), #1 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #1 FAILED!~n~cccccccc 00000044 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// read 2 bytes
0AC6: get_label_pointer @DATA {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8D: read_memory {address} 0@ {size} 2 {vp} false {result} 2@ // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x00003344
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8D (read_memory), #2 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #2 FAILED!~n~cccccccc 00003344 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// read 3 bytes
0AC6: get_label_pointer @DATA {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8D: read_memory {address} 0@ {size} 3 {vp} false {result} 2@ // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x00223344
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8D (read_memory), #3 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #3 FAILED!~n~cccccccc 00223344 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end


wait 0
// read 4 bytes
0AC6: get_label_pointer @DATA {store_to} 0@
1@ = 0xcccccccc
2@ = 0xdddddddd
3@ = 0xeeeeeeee

0A8D: read_memory {address} 0@ {size} 4 {vp} false {result} 2@ // tested opcode

if and
    1@ == 0xcccccccc
    2@ == 0x11223344
    3@ == 0xeeeeeeee
then
    trace "~g~~h~~h~0A8D (read_memory), #4 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #4 FAILED!~n~cccccccc 11223344 eeeeeeee Expected~n~%08x %08x %08x Occured" 1@ 2@ 3@
end

wait 0
// read float value
0@ = 125.0
2@ = 0
get_var_pointer 0@ {store_to} 1@
0A8D: read_memory {address} 1@ {size} 4 {vp} false {result} 2@

if 2@ == 125.0
then
    trace "~g~~h~~h~0A8D (read_memory), #5 PASSED"
else
    breakpoint "~r~~h~~h~~h~0A8D (read_memory), #5 FAILED! Expected 125.0, Actual %f" 2@
end

terminate_this_custom_script


:DATA
hex
    44 33 22 11
    "some longer testing text" 00
end
&  __SBFTR 