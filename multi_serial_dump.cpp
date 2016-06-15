#include <boost/program_options.hpp>
#include <boost/asio/serial_port.hpp>

#include <cstdlib>
#include <ostream>
#include <iostream>
#include <vector>
#include <memory>

#include "interface.hpp"
#include "hex_dump.hpp"

namespace po = boost::program_options;

std::vector< interface > parse_interfaces( const po::variables_map& variables )
{
    std::vector< interface > result;

    for ( const auto& config : variables[ "interface" ].as< std::vector< std::string > >() )
        result.push_back( interface( config ) );

    if ( result.empty() )
        throw std::runtime_error( "no interface defined" );

    return result;
}

void print_interfaces( const std::vector< interface >& interfaces, std::ostream& output )
{
    output << "configured interfaces:\n";
    for ( const auto& i : interfaces )
        output << " * " << i << "\n";

    output << std::endl;
}

struct serial_port
{
    interface                   configuration;
    boost::asio::serial_port    port;
    std::vector< std::uint8_t > read_buffer;

    serial_port( const interface& c, boost::asio::io_service& q )
        : configuration( c )
        , port( q )
        , read_buffer( 1024 )
    {}
};

template < class O >
void set_port_option( serial_port& port, const O& option,
    const std::string& device_name, const std::string& name )
{
    boost::system::error_code ec;
    port.port.set_option( option, ec );

    if ( ec )
        throw std::runtime_error( "error setting \"" + name + "\" on \"" + device_name + "\": " + ec.message() );
}

std::vector< serial_port > open_ports(
    const std::vector< interface >& interfaces, boost::asio::io_service& queue )
{
    std::vector< serial_port > ports;

    std::transform( interfaces.begin(), interfaces.end(), back_inserter( ports ),
        [&queue]( const interface& config )
        {
            serial_port port( config, queue );

            boost::system::error_code ec;
            port.port.open( config.device(), ec );

            if ( ec )
                throw std::runtime_error( "error opening interface \"" + config.device() + "\": " + ec.message() );

            set_port_option( port, config.baud_rate(), config.device(), "baud_rate" );
            set_port_option( port, config.parity(), config.device(), "parity" );
            set_port_option( port, config.stop_bits(), config.device(), "stop_bits" );
            set_port_option( port, config.character_size(), config.device(), "character_size" );

            return port;
        }
    );

    return ports;
}

void log_interfaces( const std::vector< interface >& interfaces, hex_dump& output )
{
    using namespace std::placeholders;

    boost::asio::io_service queue;
    std::vector< serial_port > ports = open_ports( interfaces, queue );

    auto read_cb = std::function< void(
                        serial_port& port,
                        const boost::system::error_code& error,
                        std::size_t bytes_transferred ) >();

    read_cb = [&output, &read_cb]( serial_port& port, const boost::system::error_code& error, std::size_t bytes_transferred )
    {
        if ( error )
            throw std::runtime_error(
                "error reading from \"" + port.configuration.device() + "\": " + error.message() );

        output.dump( port.configuration, &port.read_buffer[ 0 ], bytes_transferred );

        port.port.async_read_some(
            boost::asio::buffer( port.read_buffer ), std::bind( read_cb, std::ref( port ), _1, _2 ) );
    };

    for ( auto& port : ports )
    {
        port.port.async_read_some(
            boost::asio::buffer( port.read_buffer ),
            std::bind( read_cb, std::ref( port ), _1, _2 ) );
    }

    for ( ; ; )
        queue.run();
}

void usage( const po::options_description& options )
{
    std::cout << "usage: multi_serial_dump [options]" << std::endl;
    std::cout << options << std::endl;

    std::cout << "interface-defintion:\n";
    std::cout << "  <name>[:(<baudrate>,<bits><parity><stop-bits>|<baudrare>|<bits><parity><stop-bits>]\n";
    std::cout << "  <name> : <device-name>[(<alias>)]\n";

    std::cout << "\nexamples:\n";
    std::cout << "  multi_serial_dump --interface com1:14404,8n1\n";
    std::cout << "  multi_serial_dump -I /dev/cu.1233(com2):14404,7o1.5\n";
    std::cout << "  multi_serial_dump -I com1 -I com2\n";

    std::cout << std::endl;
}

int main( int argc, const char** argv )
{
    po::options_description options("options");
    options.add_options()
        ( "help,h", "produce help message" )
        ( "interface,I", po::value< std::vector< std::string> >(), "definition of interface" )
        ( "verbose,v", "verbose output" )
    ;

    po::variables_map variables;

    try
    {
        po::store( po::parse_command_line( argc, argv, options ), variables );
        po::notify(variables);

        if ( variables.count( "help" ) )
        {
            usage( options );
        }
        else
        {
            const bool verbose    = variables.count( "verbose" );
            const auto interfaces = parse_interfaces( variables );

            if ( verbose )
                print_interfaces( interfaces, std::cout );

            hex_dump output( std::cout );
            log_interfaces( interfaces, output );

            return EXIT_SUCCESS;
        }
    }
    catch ( const std::exception& error )
    {
        std::cerr << "error: " << error.what() << std::endl;
    }
    catch ( ... )
    {
        std::cerr << "unknow error!" << std::endl;
    }

    return EXIT_FAILURE;
}