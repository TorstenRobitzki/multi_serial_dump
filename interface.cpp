#include "interface.hpp"
#include <regex>

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
    alias_  = device_;

    if ( device_.back() == ')' )
    {
        auto alias_pos = device_.find( '(' );
        if ( alias_pos == std::string::npos )
            throw std::invalid_argument( "alias begin not found: \"" + device_ + "\"" );

        alias_  = std::string( device_, alias_pos + 1, device_.size() - 2 - alias_pos );
        device_ = std::string( device_, 0, alias_pos );
    }

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

const std::string& interface::alias() const
{
    return alias_;
}

char parity_to_text( boost::asio::serial_port_base::parity p )
{
    using type = boost::asio::serial_port_base::parity::type;

    switch ( p.value() )
    {
        case type::none:
            return 'N';
        case type::odd:
            return 'O';
        case type::even:
            return 'E';
    }

    assert( !"can't be" );
}

std::string stop_bits_to_text( boost::asio::serial_port_base::stop_bits b )
{
    using type = boost::asio::serial_port_base::stop_bits::type;

    switch ( b.value() )
    {
        case type::one:
            return "1";
        case type::onepointfive:
            return "1.5";
        case type::two:
            return "2";
    }

    assert( !"can't be" );
}

void interface::print( std::ostream& o ) const
{
    o << device_ << ":" << baud_rate_.value() << ","
        << character_size_.value() << parity_to_text( parity_ ) << stop_bits_to_text( stop_bits_ );
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

std::ostream& operator<<( std::ostream& o, const interface& i )
{
    i.print( o );

    return o;
}
