# multi_serial_dump

Simple tool to trace from multiple serial interfaces into a single output

## Usage:

    multi_serial_dump [options]
    options:
      -h [ --help ]          produce help message
      -I [ --interface ] arg definition of interface
      -v [ --verbose ]       verbose output

    interface-defintion:
      <name>[:(<baudrate>,<bits><parity><stop-bits>|<baudrare>|<bits><parity><stop-bits>]
      <name> : <device-name>[(<alias>)]

    examples:
      multi_serial_dump --interface com1:14404,8n1
      multi_serial_dump -I /dev/cu.1233(com2):14404,7o1.5
      multi_serial_dump -I com1 -I com2

## build

    mkdir build
    cd build
    cmake ..
    make multi_serial_dump

## Example Output

    $ multi_serial_dump -I "/dev/tty.usbserial-FTXKC7C3(L4 ):115200" -I "/dev/tty.usbserial-FTXO2T47(BCM):115200"
    L4 : 51 2c d2 3a 00 00 00 00 00 00 00 00 a8 51 ab d2 Q,.:.........Q..
    L4 : 3a 00 00 00 00 00 00 00 00 14 51 2a d3 3a 00 00 :.........Q*.:..
    L4 : 00 00 00 00 00 00 10 51 a4 d3 3a 00 00 00 00 00 .......Q..:.....
    L4 : 00 00 00 d0 51 23 d4 3a 00 00 00 00 00 00       ....Q#.:......
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60 00 51 00 60 .Q.`.Q.`.Q.`.Q.`
    BCM: 00 51 00 60 00 51 00 60 00 51 00 60             .Q.`.Q.`.Q.`
    L4 : 51 be 5f 45 00 00 00 00 00 00 00 00 c0          Q._E.........
    BCM: 00 51 00 60                                     .Q.`
    L4 : 51 39 60 45 00 00 00 00 00 00 00 00 38          Q9`E........8
    ^C

