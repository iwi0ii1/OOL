#ifndef IO_TERMINAL_HPP
#define IO_TERMINALL_HPP
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>

#ifdef __unix__
#include <unistd.h>
#endif

namespace alt {
    namespace io {

        // A wrapper around a terminal
        class terminal final {
        private:
            FILE* _in;
            FILE* _out;
            FILE* _err;
        
        public:
            // Wraps around the current terminal
            terminal() : _in(stdin), _out(stdout), _err(stderr) {}



            // Wraps around a specific PTY (create if not present)
            terminal(const std::string_view pty_path) {
                // TODO: Initialize file descriptors of a PTY
            }



            // Nothing to see with the destructor, lol
            ~terminal() = default;



            // Write to the terminal
            // @param texts The texts to write.
            terminal& write(const std::string_view texts) {
                fwrite(texts.data(), sizeof(char), texts.size(), _out);
                return *this;
            }



            // Error to the terminal
            // @param texts The texts to error out.
            terminal& error(const std::string_view texts) {
                fwrite(texts.data(), sizeof(char), texts.size(), _err);
                return *this;
            }



            // Flush the buffer
            // @param stream The stream to flush (default: output stream, advanced)
            terminal& flush(FILE* const stream = nullptr) noexcept {
                if (!stream) fflush(_out);
                else fflush(stream);
                return *this;
            }



            // Newline and flush the buffer
            // @param stream The stream to flush (default: output stream, advanced)
            terminal& endline(FILE* const stream = nullptr) noexcept {
                if (!stream) {
                    fwrite("\n", sizeof(char), 1, _out);
                    fflush(_out);
                } else {
                    fwrite("\n", sizeof(char), 1, stream);
                    fflush(stream);
                }
                return *this;
            }
            


            // Read from a stream until newline (excluded '\n')
            // @param stream The stream to read (default: input stream, advanced)
            // @note Return empty string if `stream` is EOF
            std::string read_line(FILE* stream = nullptr) noexcept {
                std::string buffer = "";
                if (stream == nullptr) stream = _in;

                int ch = '\0';
                while ((ch = fgetc(stream)) != EOF) {
                    if (ch == '\n') break;
                    buffer.push_back(static_cast<char>(ch));
                }
                return buffer;
            }



            // Read from a stream until newline (excluded '\n')
            // @param buffer The buffer
            // @param stream The stream to read (default: input stream, advanced)
            // @note Does nothing if `stream` is EOF
            terminal& read_into(std::string& buffer, FILE* stream = nullptr) noexcept {
                if (stream == nullptr) stream = _in;
                
                int ch;
                while ((ch = fgetc(stream)) != EOF) {
                    if (ch == '\n') break;
                    buffer.push_back(static_cast<char>(ch));
                }
                return *this;
            }



            // Read one single character from a stream
            // @param The stream to read (default: input stream, advanced)
            // @note Returns '\0' if `stream` is EOF
            char read_key(FILE* stream = nullptr) noexcept {
                if (stream == nullptr) stream = _in;
                int ch = fgetc(stream);
                return ch == EOF ? '\0' : static_cast<char>(ch);
            }
        };
    }
}