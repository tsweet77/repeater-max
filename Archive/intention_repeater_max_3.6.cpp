/*
    Intention Repeater MAX v3.6 (c)2020-2021 by Thomas Sweet aka Anthro Teacher
    Performance benchmark, enhancement and flags by Karteek Sheri.
    Updated 5/10/2021 by Thomas Sweet aka Anthro Teacher.
    Directions to compile on Windows: https://github.com/tsweet77/repeater-max/blob/main/Win_MAX_Compile_Directions.txt
    To compile on Linux (Requires 64-bit Compiler): g++ ./intention_repeater_max.cpp -O3 -o ./intention_repeater_max
    Repeats your intention up to 100 PHz to make things happen.
    For help: intention_repeater_max.exe --help
    Intention Repeater MAX is powered by a Servitor (20 Years / 2000+ hours in the making) [HR 6819 Black Hole System].
    Servitor Info: https://enlightenedstates.com/2017/04/07/servitor-just-powerful-spiritual-tool/
    Website: https://www.intentionrepeater.com/
    Forum: https://forums.intentionrepeater.com/
    Licensed under GNU General Public License v3.0
    This means you can modify, redistribute and even sell your own modified software, as long as it's open source too and released under this same license.
    https://choosealicense.com/licenses/gpl-3.0/
*/

#include <stdio.h>

#include <string>

#include <string.h>

#include <math.h>

#include <cmath>

#include <iostream>

#include <time.h>

#include <ctime>

#include <ratio>

#include <chrono>

#include <iomanip>

#include <locale.h>

#include <cstdint>

#include <vector>

#include <iterator>

#include <sstream>

#include <algorithm>

#include <ostream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
using namespace std::chrono;

#define ONE_MINUTE 60
#define ONE_HOUR 3600

//std::string PROCESS_STATEMENT = " REGULATE/INTEGRATE/OM";
std::string PROCESS_STATEMENT = ""; //Put whatever you want here. This will repeat with each iteration.

#ifndef _WIN32
#ifndef _COLORS_
#define _COLORS_

#define DEFAULT "\x1B[39m"
#define BLACK "\x1B[30m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"
#define LIGHTGRAY "\x1B[37m"
#define DARKGRAY "\x1B[90m"
#define LIGHTRED "\x1B[91m"
#define LIGHTGREEN "\x1B[92m"
#define LIGHTYELLOW "\x1B[93m"
#define LIGHTBLUE "\x1B[94m"
#define LIGHTMAGENTA "\x1B[95m"
#define LIGHTCYAN "\x1B[96m"
#define WHITE "\x1B[97m"

#endif // _COLORS_

#else
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum Color { BLACK=0, BLUE=1, GREEN=2, CYAN=3, RED=4, MAGENTA=5, BROWN=6, LIGHTGRAY=7, DARKGRAY=8, LIGHTBLUE=9, LIGHTGREEN=10, LIGHTCYAN=11, LIGHTRED=12, LIGHTMAGENTA=13, YELLOW=14, WHITE=15 };

const char* enum2str[] = {
  "BLACK",
  "BLUE",
  "GREEN",
  "CYAN",
  "RED",
  "MAGENTA",
  "BROWN",
  "LIGHTGRAY",
  "DARKGRAY",
  "LIGHTBLUE",
  "LIGHTGREEN",
  "LIGHTCYAN",
  "LIGHTRED",
  "LIGHTMAGENTA",
  "YELLOW",
  "WHITE"
};

#define BLACK			0
#define BLUE			1
#define GREEN			2
#define CYAN			3
#define RED				4
#define MAGENTA			5
#define BROWN			6
#define LIGHTGRAY		7
#define DARKGRAY		8
#define LIGHTBLUE		9
#define LIGHTGREEN		10
#define LIGHTCYAN		11
#define LIGHTRED		12
#define LIGHTMAGENTA	13
#define YELLOW			14
#define WHITE			15

#endif // _WIN32

class comma_numpunct: public std::numpunct < char > {
    protected: virtual char do_thousands_sep() const {
        return ',';
    }

    virtual std::string do_grouping() const {
        return "\03";
    }
};

std::string display_suffix(std::string num, int power, std::string designator) {
    std::string s;
    if (designator == "Iterations") {
        char iterations_suffix_array[] = {
            ' ',
            'k',
            'M',
            'B',
            'T',
            'q',
            'Q',
            's',
            'S'
        };
        //cout << "Power: " << power << endl;
        s = iterations_suffix_array[int(power / 3)];
    } else //designator == "Frequency"
    {
        char frequency_suffix_array[] = {
            ' ',
            'k',
            'M',
            'G',
            'T',
            'P',
            'E',
            'Z',
            'Y'
        };
        //cout << "Power: " << power << endl;
        s = frequency_suffix_array[int(power / 3)];
    }

    std::string str2 = num.substr(0, power % 3 + 1) + "." + num.substr(power % 3 + 1, 3) + s;

    return str2;
}

static
const char * short_scale[] = {
    "",
    "k",
    "M",
    "B",
    "T",
    "q",
    "Q",
    "s",
    "S"
};

static
const char * short_scale_hz[] = {
    "",
    "k",
    "M",
    "G",
    "T",
    "P",
    "E",
    "Z",
    "Y"
};

const char * scale(unsigned long long int n, int decimals = 1,
    const char * units[] = short_scale) {
    /*
     * Number of digits in n is given by
     * 10^x = n ==> x = log(n)/log(10) = log_10(n).
     *
     * So 1000 would be 1 + floor(log_10(10^3)) = 4 digits.
     */
    int digits = n == 0 ? 0 : 1 + floor(log10l(fabs(n)));

    // determine base 10 exponential
    int exp = digits <= 4 ? 0 : 3 * ((digits - 1) / 3);

    // normalized number
    double m = n / powl(10, exp);

    // no decimals? then don't print any
    if (m - static_cast < long > (n) == 0)
        decimals = 0;

    // don't print unit for exp<3
    static char s[64];
    static
    const char * fmt[] = {
        "%1.*lf%s",
        "%1.*lf"
    };
    sprintf(s, fmt[exp < 3], decimals, m, units[exp / 3]);
    return s;
}

const char * suffix(unsigned long long int n, int decimals = 1) {
    static char s[64];
    strcpy(s, scale(n, decimals, short_scale));
    return s;
}

const char * suffix_hz(unsigned long long int n, int decimals = 1) {
    static char s[64];
    strcpy(s, scale(n, decimals, short_scale_hz));

    return s;
}

std::string FormatTimeRun(int seconds_elapsed) {
    int hour, min, sec;

    std::string hour_formatted, min_formatted, sec_formatted;

    hour = seconds_elapsed / ONE_HOUR;
    seconds_elapsed -= hour * ONE_HOUR;
    min = seconds_elapsed / ONE_MINUTE;
    seconds_elapsed -= min * ONE_MINUTE;
    sec = seconds_elapsed;

    if (hour < 10) {
        hour_formatted = "0" + std::to_string(hour);
    } else {
        hour_formatted = std::to_string(hour);
    }

    if (min < 10) {
        min_formatted = "0" + std::to_string(min);
    } else {
        min_formatted = std::to_string(min);
    }

    if (sec < 10) {
        sec_formatted = "0" + std::to_string(sec);
    } else {
        sec_formatted = std::to_string(sec);
    }

    return hour_formatted + ":" + min_formatted + ":" + sec_formatted;
}

void print_color_help() {
    cout << "Color values for flag: --color [COLOR]" << endl << endl;

    #ifndef _WIN32
    cout << WHITE << "WHITE" << endl;
    cout << RED << "RED" << endl;
    cout << GREEN << "GREEN" << endl;
    cout << YELLOW << "YELLOW" << endl;
    cout << BLUE << "BLUE" << endl;
    cout << MAGENTA << "MAGENTA" << endl;
    cout << CYAN << "CYAN" << endl;
    cout << LIGHTGRAY << "LIGHTGRAY" << endl;
    cout << DARKGRAY << "DARKGRAY" << endl;
    cout << LIGHTRED << "LIGHTRED" << endl;
    cout << LIGHTGREEN << "LIGHTGREEN" << endl;
    cout << LIGHTYELLOW << "LIGHTYELLOW" << endl;
    cout << LIGHTBLUE << "LIGHTBLUE" << endl;
    cout << LIGHTMAGENTA << "LIGHTMAGENTA" << endl;
    cout << LIGHTCYAN << "LIGHTCYAN" << endl;
    cout << WHITE << endl;

    #else // Not _Win32
    
    for(int k = 1; k <= 15; k++)
  {
    // pick the colorattribute k you want
    SetConsoleTextAttribute(hConsole, k);
    cout << enum2str[k] << endl;
  }
    SetConsoleTextAttribute(hConsole, WHITE);
    #endif // _WIN32
}

void print_help() {
    cout << "Intention Repeater MAX v3.6 (c)2020-2021 by Thomas Sweet aka Anthro Teacher." << endl;
    cout << "Performance benchmark, exponents and flags by Karteek Sheri." << endl;
    cout << "Intention multiplying by Thomas Sweet aka Anthro Teacher." << endl << endl;

    cout << "Optional Flags:" << endl;
    cout << " a) --dur or -d" << endl;
    cout << " b) --imem or -m" << endl;
    cout << " c) --intent or -i" << endl;
    cout << " d) --suffix or -s" << endl;
    cout << " e) --timer or -t" << endl;
    cout << " f) --freq or -f" << endl;
    cout << " g) --color or -c" << endl;
    cout << " h) --colorhelp or -ch" << endl;
    cout << " i) --help or -h" << endl << endl;

    cout << "--dur = Duration in HH:MM:SS format. Example \"00:01:00\" to run for one minute. Default = \"UNTIL STOPPED\"." << endl;
    cout << "--imem = Specify how many GB of System RAM to use. Default = 1.0. Higher amount produces a faster repeat rate, but takes longer to load into memory." << endl;
    cout << "--intent = Intention. Default = Prompt the user for intention." << endl;
    cout << "--suffix = Specify HZ or EXP. EXP = Exponent (ex. 1.313x10^15). HZ (ex. 1.313PHz). Default = HZ" << endl;
    cout << "--timer = Specify INEXACT or EXACT. INEXACT Variance = 7.7ms/sec. EXACT variance = 0.04ms/sec. INEXACT = 6X Faster. Default = INEXACT." << endl;
    cout << "--freq = Specify repetition frequency in Hertz. Example: --freq 2817 for  2,817 Hz. Default = 0, or as fast as possible." << endl;
    cout << "--color = Set the color. Default = WHITE." << endl;
    cout << "--colorhelp = List all available colors for text using flag: --color." << endl;
    cout << "--help = Display this help." << endl << endl;

    cout << "Example automated usage: intention_repeater_max.exe --suffix Hz --dur \"00:01:00\" --imem 4.0 --intent \"I am calm.\"" << endl;
    cout << "Default usage: intention_repeater_max.exe" << endl << endl;

    cout << "gitHub Repository: https://github.com/tsweet77/repeater-max" << endl;
    cout << "Forum: https://forums.intentionrepeater.com" << endl;
    cout << "Website: https://www.intentionrepeater.com" << endl;
}

// Utility function to find the sum of two numbers represented as a string in CPP
std::string findsum(std::string a, std::string b) {

    std::vector < int > finalsum; // Stores the final sum of two number

    int carry = 0; // Stores carry at each stage of calculation

    /* Step 1 starts here */

    int i = a.size() - 1, j = b.size() - 1; // Start adding from lowest significant bit
    while ((i >= 0) && (j >= 0)) { // Loop until either of number exhausts first
        int x = (a[i] - '0') + (b[j] - '0') + carry; // Calculate the sum of digit in final sum by adding respective digits with previous carry.
        finalsum.push_back(x % 10); // Store the respective digit of the final sum in a vector.
        carry = x / 10; // update the carry. The carry for next step is the remaining number after forming the digit of final sum.
        i--; // Move one step towards the left in both the string(numbers)
        j--;
    }
    /*  Step 2 starts here */

    while (i >= 0) { // If the number 1 was greater than number 2, then there must be some digits to be taken care off.
        int x = (a[i] - '0') + carry; // Add the remaining digits to the carry one by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10; // update the carry from each step.
        i--;
    }
    /* Step 3 starts here */

    while (j >= 0) { // If the number 2 was greater than number 1, then there must be some digits to be taken care off.
        int x = (b[j] - '0') + carry; // Add the remaining digits to the carry one by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10; // update the carry from each step.
        j--;
    }
    /* Step 4 starts here */

    while (carry) { //If after finishing addition of the two numbers, if there is still carry/leftover then we need to take it into the final sum.
        finalsum.push_back(carry % 10); // Store digit one by one.
        carry = carry / 10; // Reduce carry
    }
    /* Step 5 starts here */
    std::stringstream final_iter;
    // Since vector pushes value at last, the most significant digits starts at the end of the vector. Thus print reverse.

    std::copy(finalsum.rbegin(), finalsum.rend(), std::ostream_iterator < int > (final_iter, ""));

    return final_iter.str();
}

int main(int argc, char ** argv)
{
    std::string intention, process_intention, intention_value, duration, param_duration, param_intention, param_timer, param_freq, param_color, runtime_formatted, ref_rate, suffix_value = "HZ";
    unsigned long long int iterations = 0, multiplier = 0;
    int seconds = 0, frequency_int = 0;
    float ram_size_value = 1;

    //parse command line arguments
    param_duration = "UNTIL STOPPED";
    param_timer = "INEXACT";
    param_freq = "0";
    param_intention = "";
    param_color = "DEFAULT";

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_help();
            exit(0);
        } else if (!strcmp(argv[i], "-ch") || !strcmp(argv[i], "--colorhelp")) {
            print_color_help();
            exit(0);
        } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dur")) {

            param_duration = argv[i + 1];

        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--timer")) {

            param_timer = argv[i + 1];

        } else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--imem")) {

            ram_size_value = atof(argv[i + 1]);

        } else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--rate")) {

            ref_rate = argv[i + 1];

        } else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--intent")) {

            param_intention = argv[i + 1];

        } else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq")) {

            param_freq = argv[i + 1];
            frequency_int = std::stoi (param_freq);

        } else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--color")) {

            param_color = argv[i + 1];
            std::transform(param_color.begin(), param_color.end(), param_color.begin(), ::toupper);

        } else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--suffix")) {

            suffix_value = argv[i + 1];
            std::transform(suffix_value.begin(), suffix_value.end(), suffix_value.begin(), ::toupper);

        } else {
            if (i == argc - 1) {
                break;
            }
            std::cout << "ERROR: Invalid Command Line Option Found: " << argv[i] << " Error " << std::endl;
        }
        ++i;
    }
    unsigned long long int INTENTION_MULTIPLIER = (ram_size_value * 1024 * 1024 * 512);

    #ifndef _WIN32
    //Set the terminal color based on the --color flag.
    if (param_color == "DEFAULT") cout << DEFAULT << std::flush;
    else if (param_color == "BLACK") cout << BLACK << std::flush;
    else if (param_color == "RED") cout << RED << std::flush;
    else if (param_color == "GREEN") cout << GREEN << std::flush;
    else if (param_color == "YELLOW") cout << YELLOW << std::flush;
    else if (param_color == "BLUE") cout << BLUE << std::flush;
    else if (param_color == "MAGENTA") cout << MAGENTA << std::flush;
    else if (param_color == "CYAN") cout << CYAN << std::flush;
    else if (param_color == "LIGHTGRAY") cout << LIGHTGRAY << std::flush;
    else if (param_color == "DARK_GRAY") cout << DARKGRAY << std::flush;
    else if (param_color == "LIGHTRED") cout << LIGHTRED << std::flush;
    else if (param_color == "LIGHTGREEN") cout << LIGHTGREEN << std::flush;
    else if (param_color == "LIGHTYELLOW") cout << LIGHTYELLOW << std::flush;
    else if (param_color == "LIGHTBLUE") cout << LIGHTBLUE << std::flush;
    else if (param_color == "LIGHTMAGENTA") cout << LIGHTMAGENTA << std::flush;
    else if (param_color == "LIGHTCYAN") cout << LIGHTCYAN << std::flush;
    else if (param_color == "WHITE") cout << WHITE << std::flush;
    
    #else
    if (param_color == "BLACK") SetConsoleTextAttribute(hConsole, BLACK);
    else if (param_color == "BLUE") SetConsoleTextAttribute(hConsole, BLUE);
    else if (param_color == "GREEN") SetConsoleTextAttribute(hConsole, GREEN);
    else if (param_color == "CYAN") SetConsoleTextAttribute(hConsole, CYAN);
    else if (param_color == "RED") SetConsoleTextAttribute(hConsole, RED);
    else if (param_color == "MAGENTA") SetConsoleTextAttribute(hConsole, MAGENTA);
    else if (param_color == "BROWN") SetConsoleTextAttribute(hConsole, BROWN);
    else if (param_color == "LIGHTGRAY") SetConsoleTextAttribute(hConsole, LIGHTGRAY);
    else if (param_color == "DARKGRAY") SetConsoleTextAttribute(hConsole, DARKGRAY);
    else if (param_color == "LIGHTBLUE") SetConsoleTextAttribute(hConsole, LIGHTBLUE);
    else if (param_color == "LIGHTGREEN") SetConsoleTextAttribute(hConsole, LIGHTGREEN);
    else if (param_color == "LIGHTCYAN") SetConsoleTextAttribute(hConsole, LIGHTCYAN);
    else if (param_color == "LIGHTRED") SetConsoleTextAttribute(hConsole, LIGHTRED);
    else if (param_color == "LIGHTMAGENTA") SetConsoleTextAttribute(hConsole, LIGHTMAGENTA);
    else if (param_color == "YELLOW") SetConsoleTextAttribute(hConsole, YELLOW);
    else if (param_color == "WHITE") SetConsoleTextAttribute(hConsole, WHITE);
    #endif //Not Windows

    std::locale comma_locale(std::locale(), new comma_numpunct());
    std::cout.imbue(comma_locale);

    cout << "Intention Repeater MAX v3.6 (c)2020-2021 by Thomas Sweet aka Anthro Teacher." << endl;
    cout << "This software comes with no guarantees or warranty of any kind and is for entertainment purposes only." << endl;
    cout << "Press Ctrl-C to quit." << endl << endl;

    if ((param_intention) == "") {
        cout << "Intention: ";
        std::getline(std::cin, intention);

    } else {
        intention = param_intention;
    }

    if (param_freq == "0") //Only use multiplier if --freq flag is not set.
    {
        cout << "LOADING INTENTION INTO MEMORY" << std::flush;
        //Repeat string till it is more than INTENTION_MULTIPLIER characters long.
        while (intention_value.length() < INTENTION_MULTIPLIER)
        {
            intention_value += intention;
            ++multiplier;
        }
        --multiplier; //Account for having to reduce at the end.

        //Now, remove enough characters at the end to account for the process statement to limit to less than 1024 characters.
        long long int intention_value_length = intention_value.length();
        long long int intention_length = intention.length();
        int process_statement_length = PROCESS_STATEMENT.length();
        long long int intention_length_val = intention_value_length - intention_length - process_statement_length;

        intention_value = intention_value.substr(0, intention_length_val);
        intention_value += PROCESS_STATEMENT;
    } //End Multiplier (when not using frequency)

    intention_value = intention + PROCESS_STATEMENT;

    duration = param_duration;

    cout << endl;

    std::string iterations_string = "0", iterations_string_freq = "0";
    int digits = 0, freq_digits = 0;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    if (param_freq == "0")
    {
        if (param_timer == "EXACT")
        {
            do
            {
                start = std::chrono::high_resolution_clock::now();
                end = std::chrono::high_resolution_clock::now();
                while ((chrono::duration_cast < chrono::seconds > (end - start).count() != 1))
                {
                    process_intention = intention_value; //This is the Intention Repeater call that actually does the work with the Servitor [HR6819].
                    ++iterations;
                    end = std::chrono::high_resolution_clock::now();
                }
                ++seconds;
                runtime_formatted = FormatTimeRun(seconds);
                iterations_string_freq = to_string(iterations * multiplier);
                iterations_string = findsum(iterations_string, iterations_string_freq);
                digits = iterations_string.length();

                freq_digits = iterations_string_freq.length();

                if (suffix_value == "EXP")
                {
                    std::cout << "[" + runtime_formatted + "]" << " (" << setprecision(3) << fixed << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / " << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1 << " Hz): " << intention << "     \r" << std::flush;
                } else //suffix_value = "HZ"
                {
                    std::cout << "[" + runtime_formatted + "]" << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / " << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency") << "Hz): " << intention << "     \r" << std::flush;
                }
                iterations = 0;
                if (runtime_formatted == duration) {
                    std::cout << endl << std::flush;
                    exit(0);
                }
            } while (1);
        } else //End param_timer = EXACT (6X slower, but 180X more accurate with variance [ms/sec] than INEXACT.)
        { //Begin param_timer = INEXACT
            do
            {
                //This code is to benchmark cpu iterations. By Karteek Sheri.
                auto b_start = std::chrono::high_resolution_clock::now();
                auto b_end = std::chrono::high_resolution_clock::now();
                unsigned long long int cpu_benchmark_count = 0;

                while ((std::chrono::duration_cast < std::chrono::seconds > (b_end - b_start).count() != 1))
                {
                    process_intention = intention_value; //The Intention Repeater Statement
                    ++cpu_benchmark_count;
                    b_end = std::chrono::high_resolution_clock::now();
                }
                //Benchmark ends here
                start = std::chrono::high_resolution_clock::now();
                end = std::chrono::high_resolution_clock::now();
                while ((std::chrono::duration_cast < std::chrono::seconds > (end - start).count() != 1))
                {
                    for (int i = 0; i < cpu_benchmark_count; ++i)
                    {
                        process_intention = intention_value; //This is the Intention Repeater call that actually does the work with the Servitor.
                        ++iterations;
                    }
                    end = std::chrono::high_resolution_clock::now();
                }
                ++seconds;
                runtime_formatted = FormatTimeRun(seconds);
                iterations_string_freq = to_string(iterations * multiplier);
                iterations_string = findsum(iterations_string, iterations_string_freq);
                digits = iterations_string.length();

                freq_digits = iterations_string_freq.length();

                if (suffix_value == "EXP")
                {
                    std::cout << "[" + runtime_formatted + "]" << " (" << setprecision(3) << fixed << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / " << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1 << " Hz): " << intention << "     \r" << std::flush;
                } else //suffix_value = "HZ"
                {
                    std::cout << "[" + runtime_formatted + "]" << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / " << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency") << "Hz): " << intention << "     \r" << std::flush;
                }
                iterations = 0;
                if (runtime_formatted == duration)
                {
                    std::cout << endl << std::flush;
                    exit(0);
                }
            } while (1);
        } //End param_timer INEXACT
    } else //End param_freq = 0
    { //Begin param_freq nonzero
        bool freq_sentinel = true; //Determins each second if the # of iterations has reached the frequency of repeating.
        do
        {
            start = std::chrono::high_resolution_clock::now();
            end = std::chrono::high_resolution_clock::now();
            freq_sentinel = true;
            while ((std::chrono::duration_cast < std::chrono::seconds > (end - start).count() != 1))
            {
                if (freq_sentinel)
                {
                    for (int i = 0; i < frequency_int; ++i)
                    {
                        process_intention = intention_value; //This is the Intention Repeater call that actually does the work with the Servitor [HR6819].
                        ++iterations;
                        end = std::chrono::high_resolution_clock::now();
                        if(std::chrono::duration_cast < std::chrono::seconds > (end - start).count() == 1)
                            break;
                    }
                    freq_sentinel = false;
                }
                end = std::chrono::high_resolution_clock::now();
            }
            ++seconds;
            runtime_formatted = FormatTimeRun(seconds);
            iterations_string_freq = to_string(iterations);
            iterations_string = findsum(iterations_string, iterations_string_freq);
            digits = iterations_string.length();

            freq_digits = iterations_string_freq.length();

            if (suffix_value == "EXP")
            {
                std::cout << "[" + runtime_formatted + "]" << " (" << setprecision(3) << fixed << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / " << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1 << " Hz): " << intention << "     \r" << std::flush;
            } else //suffix_value = "HZ"
            {
                std::cout << "[" + runtime_formatted + "]" << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / " << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency") << "Hz): " << intention << "     \r" << std::flush;
            }
            iterations = 0;
            if (runtime_formatted == duration)
            {
                std::cout << endl << std::flush;
                exit(0);
            }
        } while (1);
    } // End repetition_period nonzero

    #ifndef _WIN32
    cout << DEFAULT << std::flush;
    #else
    SetConsoleTextAttribute(hConsole, WHITE);
    #endif

    return 0;
}