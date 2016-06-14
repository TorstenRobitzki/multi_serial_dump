#include "interface.hpp"
#include <regex>

#include <iostream>

interface::interface( const std::string& config_text )
    : baud_rate_( 9800 )
{
    auto pos = config_text.find_first_not_of( " \t\n" );

    if ( pos == std::string::npos )
        pos = config_text.size();

    auto end = config_text.find( ':', pos );

    if ( end == std::string::npos )
        end = config_text.size();

    if ( pos == config_text.size() || pos == end )
        throw std::invalid_argument( "device name required." );

    device_ = std::string( config_text, pos, end - pos );

    if ( end != config_text.size() )
        parse_communication_parameters( std::string( config_text, end +1, config_text.size() - end ) );
}

boost::asio::serial_port_base::baud_rate interface::baud_rate() const
{
    return baud_rate_;
}

boost::asio::serial_port_base::parity interface::parity() const
{
    return parity_;
}

boost::asio::serial_port_base::stop_bits interface::stop_bits() const
{
    return stop_bits_;
}

boost::asio::serial_port_base::character_size interface::character_size() const
{
    return character_size_;
}

const std::string& interface::device() const
{
    return device_;
}

static boost::asio::serial_port_base::parity parity_from_text( const std::string& s )
{
    assert( s.size() == 1 );

    switch ( s[ 0 ] )
    {
        case 'n':
        case 'N':
            return boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::none );

        case 'o':
        case 'O':
            return boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::odd );

        case 'e':
        case 'E':
            return boost::asio::serial_port_base::parity( boost::asio::serial_port_base::parity::even );
    }

    assert( !"can't be" );
}

static boost::asio::serial_port_base::stop_bits stop_bits_from_text( const std::string& s )
{
    if ( s == "1" )
    {
        return boost::asio::serial_port_base::stop_bits(
            boost::asio::serial_port_base::stop_bits::one );
    }
    else if ( s == "1.5" )
    {
        return boost::asio::serial_port_base::stop_bits(
            boost::asio::serial_port_base::stop_bits::onepointfive );
    }

    assert( s == "2" );

    return boost::asio::serial_port_base::stop_bits(
        boost::asio::serial_port_base::stop_bits::two );
}

void interface::parse_communication_parameters( const std::string& parameters )
{
    static const std::regex baud_rate( "^(\\d+)" );
    static const std::regex baud_rate_and_params( "^(\\d+)\\,([78])([noeNOE])(1\\.5|1|2)" );
    static const std::regex params_only( "([78])([noeNOE])(1\\.5|1|2))" );

    std::smatch match;
    if ( std::regex_match( parameters, match, baud_rate ) )
    {
        baud_rate_ = boost::asio::serial_port_base::baud_rate( stoul( match[ 1 ] ) );
    }
    else if ( std::regex_match( parameters, match, baud_rate_and_params ) )
    {
        baud_rate_      = boost::asio::serial_port_base::baud_rate( stoul( match[ 1 ] ) );
        character_size_ = boost::asio::serial_port_base::character_size( stoul( match[ 2 ] ) );
        parity_         = parity_from_text( match[ 3 ] );
        stop_bits_      = stop_bits_from_text( match[ 4 ] );
    }
    else if ( std::regex_match( parameters, match, params_only ) )
    {
        character_size_ = boost::asio::serial_port_base::character_size( stoul( match[ 1 ] ) );
        parity_         = parity_from_text( match[ 2 ] );
        stop_bits_      = stop_bits_from_text( match[ 3 ] );
    }
    else
    {
        throw std::invalid_argument( "unable to parse communication parameters: \"" + parameters + "\"" );
    }
}
