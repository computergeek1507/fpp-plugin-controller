#pragma once

#include <stdlib.h>
#include <list>
#include <string>
#include <atomic>

//#include "common.h"
#include "log.h"

#include <curl/curl.h>

class ControllerBase {
public:
    ControllerBase(std::string const& ip, unsigned int output_count );
    virtual ~ControllerBase();

    std::string GetIPAddress() const { return m_ipAddress; }
    virtual std::string GetType() const = 0;

    virtual bool setTestModeOn( )const = 0;
    virtual bool setTestModeOff( )const = 0;

    virtual std::string GetConfigString() const {
        return "IP: " + GetIPAddress() + " Device Type: " + GetType();
    }

    static size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {

        if (data == nullptr) return 0;
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

protected:
    std::string m_ipAddress;
    uint16_t m_outputs;
    CURL *m_curl;

    bool postData(std::string const& url, std::string const& data, std::string const& contentType ) const;
    bool getData(std::string const& url, std::string& response_string, std::string const& contentType ) const;

private:

};