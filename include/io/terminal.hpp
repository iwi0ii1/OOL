#ifndef IO_TERMINAL_HPP
#define IO_TERMINALL_HPP
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>

#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#elif _WIN32
#include <windows.h>
#endif

namespace alt {
    namespace io {
        // Just a way to express your intent.
        // @note They do not point to actual streams
        enum class stream_ : uint8_t {
            in,
            out,
            err
        };

        // Terminal modes, lol
        enum class modes : uint8_t {
            canon,
            echo
        };

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



            #pragma region Out / Err

            // Write to the terminal
            // @param texts The texts to write.
            // @param stream The stream to write to (default: output, uh-oh: no input stream)
            terminal& write(const std::string_view texts, const stream_ stream = stream_::out) {
                if (stream == stream_::in) throw std::runtime_error("alt::io::terminal::write(): Cannot write to input stream.");
                fwrite(texts.data(), sizeof(char), texts.size(), stream == stream_::out ? _out : _err);
                return *this;
            }



            // Flush output / error
            // @param stream The stream to flush (default: output stream, uh-oh: no input stream)
            terminal& flush(const stream_ stream = stream_::out) {
                if (stream == stream_::in) throw std::runtime_error("alt::io::terminal::flush(): Cannot flush an input stream.\nUse `alt::io::terminal::discard_pending_input()` instead.");
                fflush(stream == stream_::out ? _out : _err);
                return *this;
            }



            // Newline and flush output / error
            // @param stream The stream to flush (default: output stream, uh-oh: no input stream)
            terminal& endline(const stream_ stream = stream_::out) {
                terminal::write("\n", stream);
                terminal::flush(stream);
            }

            #pragma endregion

            #pragma region In

            // Read from a stream until newline
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Return empty string if `stream` is EOF
            std::string read_line(const stream_ stream = stream_::in) noexcept {
                std::string buffer = "";
                FILE* that_stream = _in;
                
                if (stream == stream_::err) that_stream = _err;
                else if (stream == stream_::out) that_stream = _out;

                int ch = '\0';
                while ((ch = fgetc(that_stream)) != EOF && ch != '\n')
                    buffer.push_back(static_cast<char>(ch));

                return buffer;
            }



            // Read from a stream until newline
            // @param buffer The buffer
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Does nothing if `stream` is EOF
            terminal& read_into(std::string& buffer, const stream_ stream = stream_::in) noexcept {
                FILE* that_stream = _in;

                if (stream == stream_::err) that_stream = _err;
                else if (stream == stream_::out) that_stream = _out;

                int ch = '\0';
                while ((ch = fgetc(that_stream)) != EOF && ch != '\n')
                    buffer.push_back(static_cast<char>(ch));

                return *this;
            }



            // Read one single character from a stream
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Returns '\0' if `stream` is EOF
            char read_key(const stream_ stream = stream_::in) noexcept {
                int ch = fgetc(
                    stream == stream_::err ? _err :
                    stream == stream_::out ? _out :
                    _in
                );
                return ch == EOF ? '\0' : static_cast<char>(ch);
            }
            
            #pragma endregion

            #pragma region Clear

            // Clear the terminal (including scrollback buffer)
            terminal& clear() noexcept {
                terminal::write("\x1b[3J\x1b[2J\x1b[H");
                return *this;
            }

            #pragma endregion


            #pragma region Cursors

            // Move cursor up
            // @param rep Repetition (default: 1)
            terminal& cursor_up(const uint16_t rep = 1) noexcept {
                std::cout << "\x1b[" << rep << "A" << std::flush;
                return *this;
            }

            // Move cursor down
            // @param rep Repetition (default: 1)
            terminal& cursor_down(const uint16_t rep = 1) noexcept {
                std::cout << "\x1b[" << rep << "B" << std::flush;
                return *this;
            }


            // Move cursor left
            // @param rep Repetition (default: 1)
            terminal& cursor_left(const uint16_t rep = 1) noexcept {
                std::cout << "\x1b[" << rep << "D" << std::flush;
                return *this;
            }

            // Move cursor right
            // @param rep Repetition (default: 1)
            terminal& cursor_right(const uint16_t rep = 1) noexcept {
                std::cout << "\x1b[" << rep << "D" << std::flush;
                return *this;
            }

            #pragma endregion


            // Discard or "Flush" all pending input
            terminal& discard_pending_input() noexcept {

                return *this;
            }


            #pragma region Colors

            // Set foreground color
            // @param known_color You knew this
            terminal& foreground_color() noexcept {

                return *this;
            }

            // Set background color
            // @param known_color You knew this
            terminal& background_color() noexcept {

                return *this;
            }

            #pragma endregion



            #pragma region Modes

            // Enable modes
            // @param modes Modes, use `operator+` to enable another modes
            terminal& enable_modes() noexcept {

                return *this;
            }

            // Disable modes
            // @param modes Modes, use `operator+` to disable another modes
            terminal& disable_modes() noexcept {

                return *this;
            }

            #pragma endregion
        };
    }
}