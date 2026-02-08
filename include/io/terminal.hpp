#ifndef IO_TERMINAL_HPP
#define IO_TERMINAL_HPP

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#elif _WIN32
#include <windows.h>
#endif

namespace alt {
    namespace io {

        #pragma region Enums
        // Just a way to express your intent.
        // @note They do not point to actual streams
        enum stream_ {
            in,
            out,
            err
        };


        // Local terminal modes
        enum modes_ {
            #ifdef __unix__
            echo = ECHO,
            canon = ICANON
            #elif _WIN32
            echo = ENABLE_ECHO_INPUT,
            canon = ENABLE_LINE_INPUT
            #endif
        };


        // Colors
        enum colors_ {
            reset = 0,

            black   = 30,
            red     = 31,
            green   = 32,
            yellow  = 33,
            blue    = 34,
            magenta = 35,
            cyan    = 36,
            white   = 37,

            bright_black   = 90,
            bright_red     = 91,
            bright_green   = 92,
            bright_yellow  = 93,
            bright_blue    = 94,
            bright_magenta = 95,
            bright_cyan    = 96,
            bright_white   = 97
        };

        #pragma endregion




        // A wrapper around a terminal
        class terminal final {
        private:
            FILE* _in;
            FILE* _out;
            FILE* _err;

            #ifdef __unix__
            termios original_;
            termios modified_;
            #elif _WIN32
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

            DWORD hOriginal_;
            DWORD hModified_;
            #endif
        
        public:

            #pragma region Setups

            // Wraps around the current terminal
            terminal() : _in(stdin), _out(stdout), _err(stderr) {
                #ifdef __unix__
                tcgetattr(STDIN_FILENO, &original_);
                modified_ = original_;
                #elif _WIN32
                GetConsoleMode(hIn, &hOriginal_);
                hModified_ = hOriginal_;
                #endif
            }



            // Wraps around a specific PTY (create if not present)
            terminal(const std::string_view pty_path) {
                // TODO: Initialize file descriptors of a PTY
            }



            // Nothing to see with the destructor, lol
            ~terminal() = default;

            #pragma endregion





            #pragma region Out / Err

            // Write to the terminal
            // @param texts The texts to write.
            // @param stream The stream to write to (default: output, uh-oh: no input stream)
            terminal& write(const std::string_view texts, const stream_ stream = out) {
                if (stream == in) throw std::runtime_error("alt::io::terminal::write(): Cannot write to input stream.");
                fwrite(texts.data(), sizeof(char), texts.size(), stream == out ? _out : _err);
                return *this;
            }



            // Flush output / error
            // @param stream The stream to flush (default: output stream, uh-oh: no input stream)
            terminal& flush(const stream_ stream = out) {
                if (stream == in) throw std::runtime_error("alt::io::terminal::flush(): Cannot flush an input stream.\nUse `alt::io::terminal::discard_pending_input()` instead.");
                fflush(stream == out ? _out : _err);
                return *this;
            }



            // Newline and flush output / error
            // @param stream The stream to flush (default: output stream, uh-oh: no input stream)
            terminal& endline(const stream_ stream = out) {
                terminal::write("\n", stream);
                terminal::flush(stream);
                return *this;
            }

            #pragma endregion





            #pragma region In

            // Read from a stream until newline
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Return empty string if `stream` is EOF
            std::string read_line(const stream_ stream = in) noexcept {
                std::string buffer = "";
                FILE* that_stream = _in;
                
                if (stream == err) that_stream = _err;
                else if (stream == out) that_stream = _out;

                int ch = '\0';
                while ((ch = fgetc(that_stream)) != EOF && ch != '\n')
                    buffer.push_back(static_cast<char>(ch));

                return buffer;
            }



            // Read from a stream until newline
            // @param buffer The buffer
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Does nothing if `stream` is EOF
            terminal& read_into(std::string& buffer, const stream_ stream = in) noexcept {
                FILE* that_stream = _in;

                if (stream == err) that_stream = _err;
                else if (stream == out) that_stream = _out;

                int ch = '\0';
                while ((ch = fgetc(that_stream)) != EOF && ch != '\n')
                    buffer.push_back(static_cast<char>(ch));

                return *this;
            }



            // Read one single character from a stream
            // @param stream The stream to read (default: input stream, fun-fact: can do `out / err` stream)
            // @note Returns '\0' if `stream` is EOF
            char read_key(const stream_ stream = in) noexcept {
                int ch = fgetc(
                    stream == err ? _err :
                    stream == out ? _out :
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
                std::cout << "\x1b[" << rep << "C" << std::flush;
                return *this;
            }

            #pragma endregion





            // Discard or "Flush" all pending input
            terminal& discard_pending_input() noexcept {
                #ifdef __unix__
                tcflush(fileno(_in), TCIFLUSH);
                #elif _WIN32
                FlushConsoleInputBuffer(hIn);
                #endif
                return *this;
            }





            #pragma region Colors

            // Set foreground color
            // @param known_color You knew this
            terminal& foreground_color(const colors_ color) noexcept {
                std::cout << "\x1b[" << color << "m" << std::flush;
                return *this;
            }

            // Set background color
            // @param known_color You knew this
            terminal& background_color(const colors_ color) noexcept {
                std::cout << "\x1b[" << color + 10 << "m" << std::flush;
                return *this;
            }

            #pragma endregion



            #pragma region Modes

            // Enable local terminal modes
            // @param mode Use `operator|` to enable more modes
            terminal& enable_modes(const unsigned int mode) noexcept {
                #ifdef __unix__
                modified_.c_lflag |= mode;
                tcsetattr(STDIN_FILENO, TCSANOW, &modified_);
                #elif _WIN32
                hModified_ |= mode;
                SetConsoleMode(hIn, hModified_);
                #endif

                return *this;
            }

            // Disable local terminal modes
            // @param modes Use `operator|` to disable more modes
            terminal& disable_modes(const unsigned int mode) noexcept {
                #ifdef __unix__
                modified_.c_lflag &= ~mode;
                tcsetattr(STDIN_FILENO, TCSANOW, &modified_);
                #elif _WIN32
                hModified_ &= ~mode;
                SetConsoleMode(hIn, hModified_);
                #endif

                return *this;
            }

            // Restore modes
            terminal& restore() noexcept {
                #ifdef __unix__
                tcsetattr(STDIN_FILENO, TCSANOW, &original_);
                #elif _WIN32
                SetConsoleMode(hIn, hOriginal_);
                #endif

                return *this;
            }

            #pragma endregion
        };
    }
}

#endif