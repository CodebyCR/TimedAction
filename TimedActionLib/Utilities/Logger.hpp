
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

class Logger : public std::streambuf {

private:
    /// Map key
    static constexpr std::string_view LOG = "Log";

    /// Sub keys
    static constexpr std::string_view LOG_FOLDER = "Log folder";
    static constexpr std::string_view LOG_LEVEL = "Log level";
    static constexpr std::string_view LOG_TO_COUT = "Log to cout";

    /// Attributes
    Dispatcher dispatcher;
    bool _log_to_cout = Dispatcher::to_bool_or_else(
            dispatcher.env_lookup(LOG, LOG_TO_COUT), true);

    std::filesystem::path _log_folder = dispatcher.env_lookup(LOG, LOG_FOLDER)
                                                .value_or("../../logs");

    bool m_useCout;
    std::ostream* m_coutStream;
    std::ofstream m_logfile{"log.txt"};
    std::stringstream m_buffer;
    std::streambuf* m_coutbuf;


protected:
    int_type overflow(int_type c) override {
        if (c == '\n') {
            // write the current line to the log file
            m_logfile << m_buffer.str() << std::endl;

            // write the current line to the output stream (if set)
            if (m_useCout && m_coutStream != nullptr) {
                (*m_coutStream) << m_buffer.str() << std::endl;
            }

            // reset the buffer
            m_buffer.str("");
        } else {
            m_buffer.put(c);
        }


        return c;
    }

public:
    explicit Logger(bool use_cout = false)
            : m_useCout(use_cout), m_coutStream(nullptr) {
        if (m_useCout) {
            // save the original cout streambuf
            m_coutbuf = std::cout.rdbuf();
            // set cout to use this Logger's streambuf instead
            std::cout.rdbuf(this);
        }

        if (!exists(_log_folder)){
            create_directory(_log_folder);
        }

    }

    ~Logger() override {
        if (m_useCout) {
            // restore the original cout streambuf
            std::cout.rdbuf(m_coutbuf);
        }
    }

    auto operator<<(std::ostream& (*pf)(std::ostream&)) -> Logger& {
        pf(m_buffer);
        return *this;
    }

    auto flush() -> void {
        // write any remaining data in the buffer to the log file
        m_logfile << m_buffer.str() << std::flush;

        // write any remaining data in the buffer to the output stream (if set)
        if (m_useCout && m_coutStream != nullptr) {
            (*m_coutStream) << m_buffer.str() << std::flush;
        }

        // reset the buffer
        m_buffer.str("");
    }


    auto toggle_cout() -> void {
        m_useCout = !m_useCout;
    }

};

// a methode to toggle the output also to cout
// a methode to set a logfile path

