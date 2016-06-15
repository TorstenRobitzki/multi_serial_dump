#ifndef MULTI_SERIAL_DUMP_HEX_DUMP_HPP
#define MULTI_SERIAL_DUMP_HEX_DUMP_HPP

#include <iosfwd>
#include <cstdint>
#include <cstdlib>
#include "interface.hpp"

class hex_dump
{
public:
    explicit hex_dump( std::ostream& output );

    void dump( const interface& config, const std::uint8_t*, std::size_t );
private:
    void line_header( const interface& config );

    std::ostream& output_;
};

#endif
