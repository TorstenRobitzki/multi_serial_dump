#ifndef MULTI_SERIAL_DUMP_INTERFACE_HPP
#define MULTI_SERIAL_DUMP_INTERFACE_HPP

#include <string>
#include <boost/asio/serial_port_base.hpp>

class interface
{
public:

    /**
     * @brief constructs an interface from configuration string
     *
     * Syntax: <name>[:<parameter-list>]
     * parameter-list: [<baud-rate>] | [<char-config>] | [<baud-rate><char-config>]
     */
    explicit interface( const std::string& config_text );

    boost::asio::serial_port_base::baud_rate baud_rate() const;

    boost::asio::serial_port_base::parity parity() const;

    boost::asio::serial_port_base::stop_bits stop_bits() const;

    boost::asio::serial_port_base::character_size character_size() const;

    const std::string& device() const;

private:
    void parse_communication_parameters( const std::string& parameters );

    boost::asio::serial_port_base::baud_rate        baud_rate_;
    boost::asio::serial_port_base::parity           parity_;
    boost::asio::serial_port_base::stop_bits        stop_bits_;
    boost::asio::serial_port_base::character_size   character_size_;
    std::string                                     device_;
};

#endif
