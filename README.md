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