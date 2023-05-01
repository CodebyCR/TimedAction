
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

class Logger : public std::streambuf {

private:
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
