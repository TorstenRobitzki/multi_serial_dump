#include "hex_dump.hpp"
#include <cassert>
#include <ostream>

hex_dump::hex_dump( std::ostream& output )
    : output_( output )
{
}

static std::uint8_t nibble( std::uint8_t val )
{
    assert(val < 16);
    return val < 10 ? '0' + val : 'a' + (val - 10);
}

static void print_hex(std::ostream& out, std::uint8_t value)
{
    out << nibble(value >> 4) << nibble(value & 0xf);
}

static std::uint8_t as_printable(std::uint8_t maybe_printable)
{
    return maybe_printable > 31 && maybe_printable < 127
        ? maybe_printable
        : '.';
}

void hex_dump::dump( const interface& config, const std::uint8_t* begin, std::size_t size )
{
    static const std::size_t page_width = 16;
    std::size_t              char_cnt   = page_width;
    const std::uint8_t*      line_start = begin;

    line_header( config );

    while ( size )
    {
        print_hex( output_, *begin );
        output_ << ' ';

        --size;
        ++begin;
        --char_cnt;

        if ( char_cnt == 0 || size == 0 )
        {
            // fill rest of the line
            for ( ; char_cnt != 0; --char_cnt)
                output_ << "   ";

            for ( ; line_start != begin; ++line_start )
                output_ << as_printable( *line_start );

            output_ << '\n';

            if ( size )
                line_header( config );

            char_cnt   = page_width;
        }
    }
}

void hex_dump::line_header( const interface& config )
{
    output_ << config.alias() << ": ";
}
