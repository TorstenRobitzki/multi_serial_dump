#define BOOST_TEST_MODULE
#include <boost/test/included/unit_test.hpp>

#include "interface.hpp"
#include <stdexcept>

std::function< bool( const std::invalid_argument& error ) > check_error_text( const std::string& text )
{
    return [text]( const std::invalid_argument& error ) -> bool {
        return error.what() == text;
    };
}

BOOST_AUTO_TEST_CASE( error_if_no_device_is_given )
{
    BOOST_CHECK_EXCEPTION( interface(""), std::invalid_argument, check_error_text("device name required.") );
}

BOOST_AUTO_TEST_CASE( parse_device_name )
{
    BOOST_CHECK_EQUAL( interface("com42").device(), "com42" );
}

BOOST_AUTO_TEST_CASE( parse_device_name_followed_by_communication_parameters )
{
    BOOST_CHECK_EQUAL( interface("com42:19200,8n1").device(), "com42" );
    BOOST_CHECK_EQUAL( interface("com42:8n1").device(), "com42" );
    BOOST_CHECK_EQUAL( interface("com42:19200").device(), "com42" );
}

BOOST_AUTO_TEST_CASE( baud_rate_is_given )
{
    BOOST_CHECK_EQUAL( interface("com42:19200,8n1").baud_rate().value(), 19200 );
    BOOST_CHECK_EQUAL( interface("com42:9200").baud_rate().value(), 9200 );
}

BOOST_AUTO_TEST_CASE( baud_rate_is_not_given )
{
    BOOST_CHECK_EQUAL( interface("com42").baud_rate().value(), 9800 );
}

BOOST_AUTO_TEST_CASE( character_size_is_given )
{
    BOOST_CHECK_EQUAL( interface("com42:19200,7n1").character_size().value(), 7 );
    BOOST_CHECK_EQUAL( interface("com42:9200,8n1").character_size().value(), 8 );
    BOOST_CHECK_EQUAL( interface("com42:7n1").character_size().value(), 7 );
    BOOST_CHECK_EQUAL( interface("com42:8n1").character_size().value(), 8 );
}

BOOST_AUTO_TEST_CASE( character_size_is_not_given )
{
    BOOST_CHECK_EQUAL( interface("com42:19200").character_size().value(), 8 );
    BOOST_CHECK_EQUAL( interface("com42").character_size().value(), 8 );
}

BOOST_AUTO_TEST_CASE( parity_is_given )
{
    using type = boost::asio::serial_port_base::parity::type;

    BOOST_CHECK_EQUAL( interface("com42:8n1").parity().value(), type::none );
    BOOST_CHECK_EQUAL( interface("com42:8N1").parity().value(), type::none );
    BOOST_CHECK_EQUAL( interface("com42:8o1").parity().value(), type::odd );
    BOOST_CHECK_EQUAL( interface("com42:8O1").parity().value(), type::odd );
    BOOST_CHECK_EQUAL( interface("com42:8e1").parity().value(), type::even );
    BOOST_CHECK_EQUAL( interface("com42:8E1").parity().value(), type::even );
    BOOST_CHECK_EQUAL( interface("com42:14400,8n1").parity().value(), type::none );
    BOOST_CHECK_EQUAL( interface("com42:14400,8N1").parity().value(), type::none );
    BOOST_CHECK_EQUAL( interface("com42:14400,8o1").parity().value(), type::odd );
    BOOST_CHECK_EQUAL( interface("com42:14400,8O1").parity().value(), type::odd );
    BOOST_CHECK_EQUAL( interface("com42:14400,8e1").parity().value(), type::even );
    BOOST_CHECK_EQUAL( interface("com42:14400,8E1").parity().value(), type::even );
}

BOOST_AUTO_TEST_CASE( parity_is_not_given )
{
    using type = boost::asio::serial_port_base::parity::type;

    BOOST_CHECK_EQUAL( interface("com42").parity().value(), type::none );
    BOOST_CHECK_EQUAL( interface("com42:1200").parity().value(), type::none );
}

BOOST_AUTO_TEST_CASE( stop_bit_are_given )
{
    using type = boost::asio::serial_port_base::stop_bits::type;

    BOOST_CHECK_EQUAL( interface("com42:8n1").stop_bits().value(), type::one );
    BOOST_CHECK_EQUAL( interface("com42:8n1.5").stop_bits().value(), type::onepointfive );
    BOOST_CHECK_EQUAL( interface("com42:8n2").stop_bits().value(), type::two );
    BOOST_CHECK_EQUAL( interface("com42:14400,8n1").stop_bits().value(), type::one );
    BOOST_CHECK_EQUAL( interface("com42:14400,8n1.5").stop_bits().value(), type::onepointfive );
    BOOST_CHECK_EQUAL( interface("com42:14400,8n2").stop_bits().value(), type::two );
}

BOOST_AUTO_TEST_CASE( stop_bit_are_not_given )
{
    using type = boost::asio::serial_port_base::stop_bits::type;

    BOOST_CHECK_EQUAL( interface("com42").stop_bits().value(), type::one );
    BOOST_CHECK_EQUAL( interface("com42:14400").stop_bits().value(), type::one );
}

BOOST_AUTO_TEST_CASE( interface_with_alias )
{
    BOOST_CHECK_EQUAL( interface("com42(42)").alias(), "42" );
    BOOST_CHECK_EQUAL( interface("com42(42)").device(), "com42" );
    BOOST_CHECK_EQUAL( interface("com42(42):14400").alias(), "42" );
    BOOST_CHECK_EQUAL( interface("com42(42):14400").device(), "com42" );
    BOOST_CHECK_EQUAL( interface("com42(42):8n1").alias(), "42" );
    BOOST_CHECK_EQUAL( interface("com42(42):14400,8n1").device(), "com42" );
}

BOOST_AUTO_TEST_CASE( interface_without_alias )
{
    BOOST_CHECK_EQUAL( interface("com42").alias(), "com42" );
    BOOST_CHECK_EQUAL( interface("com42").device(), "com42" );
}