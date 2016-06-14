#include <boost/program_options.hpp>

#include <cstdlib>
#include <ostream>
#include <iostream>
#include <vector>

#include "interface.hpp"

namespace po = boost::program_options;

std::vector< interface > parse_interfaces( const po::variables_map& variables )
{
    std::vector< interface > result;

    for ( const auto& config : variables["include-path"].as< std::vector< std::string > >() )
        result.push_back( interface( config ) );

    return result;
}

void log_interfaces( const std::vector< interface >& interfaces, std::ostream& output )
{
    if ( interfaces.empty() )
        throw std::runtime_error( "no interface defined" );
}

void usage( const po::options_description& options )
{
    std::cout << "usage: multi_serial_dump [options]" << std::endl;
    std::cout << options << std::endl;
}

int main( int argc, const char** argv )
{
    po::options_description options("options");
    options.add_options()
        ("help,h", "produce help message")
        ("interface,I", po::value< std::vector< std::string> >(), "definition of interface")
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
            const auto interfaces = parse_interfaces( variables );
            log_interfaces( interfaces, std::cout );

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