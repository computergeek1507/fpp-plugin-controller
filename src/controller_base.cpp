#include "controller_base.h"

#include <iostream>
#include <istream>
#include <ostream>

ControllerBase::ControllerBase(std::string const& ip, unsigned int output_count) :
    m_ipAddress(ip),
    m_outputs(output_count)
{
    m_curl = curl_easy_init();
}

ControllerBase::~ControllerBase() {
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
}

bool ControllerBase::postData(std::string const& url, std::string const& data, std::string const& contentType) const {
    try {
        curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, 1L);

        std::string const content = "Content-Type: " + contentType;
        struct curl_slist *hs = NULL;
        hs = curl_slist_append(hs, content.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, hs);

        LogInfo(VB_PLUGIN, "Data '%s'\n",data.c_str());
        curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());       

        std::string const repURL = "http://" + m_ipAddress + url;
        LogInfo(VB_PLUGIN, "URL '%s'\n",repURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(m_curl, CURLOPT_MAXAGE_CONN, 120L);

        CURLcode status = curl_easy_perform(m_curl);
        if (status != CURLE_OK) {
            return false;
            LogInfo(VB_PLUGIN, "failed to send curl command\n");
        }
        return true;
    }
    catch(std::exception const& ex) {
        LogInfo(VB_PLUGIN, "Error %s \n", ex.what());
    }
    return false;
}

bool ControllerBase::getData(std::string const& url, std::string& response_string, std::string const& contentType) const {
    try {
        curl_easy_setopt(m_curl, CURLOPT_HTTPGET, 1L);

        std::string const content = "Content-Type: " + contentType;
        struct curl_slist *hs = NULL;
        hs = curl_slist_append(hs, content.c_str());
        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, hs);

        std::string const repURL = "http://" + m_ipAddress + url;
        LogInfo(VB_PLUGIN, "URL '%s'\n",repURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_URL, repURL.c_str());
        curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(m_curl, CURLOPT_MAXAGE_CONN, 120L);

        //std::string response_string;

        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode status = curl_easy_perform(m_curl);
        if (status != CURLE_OK) {
            return false;
            LogInfo(VB_PLUGIN, "failed to send curl command\n");
        }
        return true;
    }
    catch(std::exception const& ex) {
        LogInfo(VB_PLUGIN, "Error %s \n", ex.what());
    }
    return false;
}


