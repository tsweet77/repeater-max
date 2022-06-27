/*
    Intention Repeater MAX v5.7 (c)2020-2022 by Anthro Teacher aka Thomas Sweet.
    Enhancement and flags by Karteek Sheri.
    Holo-Link framework created by Mystic Minds. This implementation by Anthro Teacher.
    Boosting through Nested Files by Anthro Teacher.
    Updated 6/26/2022 by Anthro Teacher.
    To compile on Linux, I recommend g++.
    Repeats your intention up to 100 PHz to make things happen.
    For help: intention_repeater_max.exe --help
    Intention Repeater MAX is powered by a Servitor (20 Years / 2000+ hours in
   the making) [HR 6819 Black Hole System]. Servitor Info:
   https://enlightenedstates.com/2017/04/07/servitor-just-powerful-spiritual-tool/
    Website: https://www.intentionrepeater.com/
    Forum: https://forums.intentionrepeater.com/
    Licensed under GNU General Public License v3.0
    This means you can modify, redistribute and even sell your own modified
   software, as long as it's open source too and released under this same
   license. https://choosealicense.com/licenses/gpl-3.0/
*/

#include <stdio.h>

#ifndef _WIN32
#include <unistd.h>

#endif

#include <string>

#include <string.h>

#include <math.h>

#include <cmath>

#include <fstream>

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

#ifndef _WIN32
#ifndef _COLORS_
#define _COLORS_

#define DEFAULT "\x1b[0m"
#define DARKGRAY "\x1b[1;30m"
#define BLACK "\x1b[0;30m"
#define LIGHTRED "\x1b[1;31m"
#define RED "\x1b[0;31m"
#define LIGHTGREEN "\x1b[1;32m"
#define GREEN "\x1b[0;32m"
#define LIGHTYELLOW "\x1b[1;33m"
#define YELLOW "\x1b[0;33m"
#define LIGHTBLUE "\x1b[1;34m"
#define BLUE "\x1b[0;34m"
#define LIGHTMAGENTA "\x1b[1;35m"
#define MAGENTA "\x1b[0;35m"
#define LIGHTCYAN "\x1b[1;36m"
#define CYAN "\x1b[0;36m"
#define WHITE "\x1b[1;37m"
#define LIGHTGRAY "\x1b[0;37m"

#endif // _COLORS_

#else
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

const char *enum2str[] = {"BLACK", "BLUE", "GREEN", "CYAN", "RED", "MAGENTA",
                          "YELLOW", "WHITE", "DARKGRAY", "LIGHTBLUE", "LIGHTGREEN", "LIGHTCYAN",
                          "LIGHTRED", "LIGHTMAGENTA", "LIGHTYELLOW", "LIGHTGRAY"};

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define YELLOW 6
#define WHITE 7
#define DARKGRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define LIGHTYELLOW 14
#define LIGHTGRAY 15

#endif // _WIN32

class comma_numpunct : public std::numpunct<char>
{
protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};

std::string display_suffix(std::string num, int power, std::string designator)
{
    std::string s;
    if (designator == "Iterations")
    {
        char iterations_suffix_array[] = {' ', 'k', 'M', 'B', 'T', 'q', 'Q', 's', 'S', 'O', 'N', 'D'};
        // cout << "Power: " << power << endl;
        s = iterations_suffix_array[int(power / 3)];
    }
    else // designator == "Frequency"
    {
        char frequency_suffix_array[] = {' ', 'k', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y'};
        // cout << "Power: " << power << endl;
        s = frequency_suffix_array[int(power / 3)];
    }

    std::string str2 = num.substr(0, power % 3 + 1) + "." + num.substr(power % 3 + 1, 3) + s;

    return str2;
}

static const char *short_scale[] = {"", "k", "M", "B", "T", "q", "Q", "s", "S", "O", "N", "D"};

static const char *short_scale_hz[] = {"", "k", "M", "G", "T", "P", "E", "Z", "Y"};

const char *scale(unsigned long long int n, int decimals = 1, const char *units[] = short_scale)
{
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
    if (m - static_cast<long>(n) == 0)
        decimals = 0;

    // don't print unit for exp<3
    static char s[64];
    static const char *fmt[] = {"%1.*lf%s", "%1.*lf"};
    sprintf(s, fmt[exp < 3], decimals, m, units[exp / 3]);
    return s;
}

const char *suffix(unsigned long long int n, int decimals = 1)
{
    static char s[64];
    strcpy(s, scale(n, decimals, short_scale));
    return s;
}

const char *suffix_hz(unsigned long long int n, int decimals = 1)
{
    static char s[64];
    strcpy(s, scale(n, decimals, short_scale_hz));

    return s;
}

std::string FormatTimeRun(int seconds_elapsed)
{
    int hour, min, sec;

    std::string hour_formatted, min_formatted, sec_formatted;

    hour = seconds_elapsed / ONE_HOUR;
    seconds_elapsed -= hour * ONE_HOUR;
    min = seconds_elapsed / ONE_MINUTE;
    seconds_elapsed -= min * ONE_MINUTE;
    sec = seconds_elapsed;

    if (hour < 10)
    {
        hour_formatted = "0" + std::to_string(hour);
    }
    else
    {
        hour_formatted = std::to_string(hour);
    }

    if (min < 10)
    {
        min_formatted = "0" + std::to_string(min);
    }
    else
    {
        min_formatted = std::to_string(min);
    }

    if (sec < 10)
    {
        sec_formatted = "0" + std::to_string(sec);
    }
    else
    {
        sec_formatted = std::to_string(sec);
    }

    return hour_formatted + ":" + min_formatted + ":" + sec_formatted;
}

void print_color_help()
{
#ifndef _WIN32
    cout << WHITE;
#else
    SetConsoleTextAttribute(hConsole, WHITE);
#endif

    cout << "Color values for flag: --color [COLOR]" << endl
         << endl;

#ifndef _WIN32
    cout << DEFAULT << "DEFAULT" << endl;
    cout << DARKGRAY << "DARKGRAY" << endl;
    cout << BLACK << "BLACK" << endl;
    cout << LIGHTRED << "LIGHTRED" << endl;
    cout << RED << "RED" << endl;
    cout << LIGHTGREEN << "LIGHTGREEN" << endl;
    cout << GREEN << "GREEN" << endl;
    cout << LIGHTYELLOW << "LIGHTYELLOW" << endl;
    cout << YELLOW << "YELLOW" << endl;
    cout << LIGHTBLUE << "LIGHTBLUE" << endl;
    cout << BLUE << "BLUE" << endl;
    cout << LIGHTMAGENTA << "LIGHTMAGENTA" << endl;
    cout << MAGENTA << "MAGENTA" << endl;
    cout << LIGHTCYAN << "LIGHTCYAN" << endl;
    cout << CYAN << "CYAN" << endl;
    cout << WHITE << "WHITE" << endl;
    cout << LIGHTGRAY << "LIGHTGRAY" << endl;

#else

    for (int k = 1; k <= 15; k++)
    {
        // pick the colorattribute k you want
        SetConsoleTextAttribute(hConsole, k);
        cout << enum2str[k] << endl;
    }
#endif // _WIN32
}

void create_nesting_files()
{

    std::fstream myfile;
    std::string filename;

    myfile.open("NEST-1.TXT", ios::out);

    if (myfile.is_open())
    {
        for (int repnum = 1; repnum <= 10; repnum++)
        {
            myfile << "INTENTIONS.TXT\r\n";
        }
    }

    for (int filenum = 1; filenum <= 100; filenum++)
    {
        filename = "NEST-" + std::to_string(filenum) + ".TXT";
        myfile.open(filename, ios::out);

        for (int repnum = 1; repnum <= 10; repnum++)
        {
            myfile << "NEST-" + std::to_string(filenum - 1) + ".TXT\r\n";
        }
        myfile.close();
    }

    cout << LIGHTBLUE << "Intention Repeater Nesting Files Written." << endl;

    cout << "Be sure to have your intentions in the INTENTIONS.TXT file." << endl;

    cout << "To run with the nesting option, use --boostlevel 50, for example to use up to Nesting to 50 levels deep."
         << endl;

    cout << "--boostlevel valid values: 1 to 100." << endl;

    cout << "When using --boostlevel 50, for example, it will ignore the --intent, and use \"NEST-50.TXT\" for the "
            "intent instead."
         << endl
         << endl;
}

void print_help()
{
#ifndef _WIN32
    cout << LIGHTBLUE;
#else
    SetConsoleTextAttribute(hConsole, LIGHTYELLOW);
#endif

    cout << LIGHTBLUE
         << "Intention Repeater MAX v5.7 (c)2020-2022 by Anthro Teacher aka "
            "Thomas Sweet."
         << endl;
    cout << "This utility repeats your intention millions of times per second, "
            "in computer memory, to aid in manifestation."
         << endl;
    cout << "Performance benchmark, exponents and flags by Karteek Sheri." << endl;
    cout << "Holo-Link framework by Mystic Minds. This implementation by Anthro "
            "Teacher."
         << endl;
    cout << "Intention multiplying by Anthro Teacher." << endl
         << endl;

    cout << "Optional Flags:" << endl;
    cout << " a) --dur or -d, example: --dur 00:01:00" << endl;
    cout << " b) --imem or -m, example: --imem 5" << endl;
    cout << " c) --intent or -i, example: --intent \"I am love.\"" << endl;
    cout << " d) --suffix or -s, example: --suffix HZ" << endl;
    cout << " e) --timer or -t, example: --timer INEXACT" << endl;
    cout << " f) --freq or -f, example: --freq 1000" << endl;
    cout << " g) --color or -c, example: --color LIGHTBLUE" << endl;
    cout << " h) --boostlevel or -b, example: --boostlevel 100" << endl;
    cout << " i) --createnestingfiles or -p, example: --createnestingfiles" << endl;
    cout << " j) --usehololink or -u" << endl;
    cout << " k) --createhololinkfiles or -x" << endl;
    cout << " l) --colorhelp or -n" << endl;
    cout << " m) --amplify or -a" << endl;
    cout << " n) --restevery or -e" << endl;
    cout << " o) --restfor or -r" << endl;
    cout << " p) --help or -h" << endl
         << endl;

    cout << "--dur = Duration in HH:MM:SS format. Default = Run until stopped "
            "manually."
         << endl;
    cout << "--imem = Specify how many GB of System RAM to use. Higher amount "
            "repeats faster, but takes longer to load. Default = 1.0."
         << endl;
    cout << "--intent = Intention. Default = Prompts the user for intention." << endl;
    cout << "--suffix = Specify whether to show regular (Hz) designator or "
            "scientific notation (Exp). Default = HZ."
         << endl;
    cout << "--timer = Specify INEXACT or EXACT. Default = INEXACT." << endl;
    cout << "--freq = Specify repetition frequency in Hz. Default = As fast as "
            "possible."
         << endl;
    cout << "--usehololink = Utilize the Holo-Link framework by Mystic Minds." << endl;
    cout << "--createhololinkfiles will create the default Holo-Link files and "
            "exit. Run with this option before using the --usehololink option. "
            "This will overwrite the TXT files."
         << endl;
    cout << "--color = Set the text color. Default = WHITE." << endl;
    cout << "--colorhelp = List and show all available colors for text using the "
            "option --color."
         << endl;
    cout << "--createnestingfiles = Create the NEST- files required for boosting." << endl;
    cout << "--boostlevel = Set the level to boost the power (1-100). Use --createnestingfiles before using "
            "--boostlevel."
         << endl;
    cout << "--amplify = Amplification Level. Default = 1000000000." << endl;

    cout << "--restevery = Stop Repeating every specified # of seconds." << endl;

    cout << "--restfor = # of Seconds to rest for each rest period." << endl;

    cout << "--help = Display this help." << endl
         << endl;

    cout << "Example usage:" << endl;
    cout << "intention_repeater_max.exe --dur \"00:01:00\" --imem 4.0 --intent \"I am Love.\" --amplify 1000000000"
         << endl
         << endl;

    cout << "Example usage with Holo-Link:" << endl;
    cout << "1) intention_repeater_max.exe --createhololinkfiles" << endl;
    cout << "2) intention_repeater_max.exe --usehololink --color LIGHTBLUE "
            "--suffix EXP --dur 00:01:00 --imem 4.0 --intent \"I am calm.\""
         << endl
         << endl;

    cout << "Make sure to create your INTENTIONS.TXT file, in this folder, with "
            "your intentions, before running #2 above."
         << endl;
    cout << "The --intent option is ignored when using --usehololink, which "
            "instead uses the INTENTIONS.TXT file."
         << endl
         << endl;

    cout << "Example usage with Nesting Files:" << endl;
    cout << "1) intention_repeater_max.exe --createnestingfiles" << endl;
    cout << "2) intention_repeater_max.exe --color LIGHTBLUE --dur 00:01:00 --imem 4.0 --boostlevel 5" << endl
         << endl;

    cout << "Make sure to create your INTENTIONS.TXT file, in this folder, with "
            "your intentions, and the Nesting Files before running #2 above."
         << endl
         << endl;

    cout << "gitHub Repository: https://github.com/tsweet77/repeater-max" << endl;
    cout << "Forum: https://forums.intentionrepeater.com" << endl;
    cout << "Website: https://www.intentionrepeater.com" << endl;

#ifndef _WIN32
    cout << LIGHTBLUE;
#else
    SetConsoleTextAttribute(hConsole, LIGHTBLUE);
#endif
}

// Utility function to find the sum of two numbers represented as a string in
// CPP
std::string findsum(std::string a, std::string b)
{

    std::vector<int> finalsum; // Stores the final sum of two number

    int carry = 0; // Stores carry at each stage of calculation

    /* Step 1 starts here */

    int i = a.size() - 1,
        j = b.size() - 1; // Start adding from lowest significant bit
    while ((i >= 0) && (j >= 0))
    {                                                // Loop until either of number exhausts first
        int x = (a[i] - '0') + (b[j] - '0') + carry; // Calculate the sum of digit in final sum by adding
                                                     // respective digits with previous carry.
        finalsum.push_back(x % 10);                  // Store the respective digit of the final sum in a vector.
        carry = x / 10;                              // update the carry. The carry for next step is the
                                                     // remaining number after forming the digit of final sum.
        i--;                                         // Move one step towards the left in both the string(numbers)
        j--;
    }
    /*  Step 2 starts here */

    while (i >= 0)
    {                                 // If the number 1 was greater than number 2, then there must
                                      // be some digits to be taken care off.
        int x = (a[i] - '0') + carry; // Add the remaining digits to the carry one
                                      // by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10; // update the carry from each step.
        i--;
    }
    /* Step 3 starts here */

    while (j >= 0)
    {                                 // If the number 2 was greater than number 1, then there must
                                      // be some digits to be taken care off.
        int x = (b[j] - '0') + carry; // Add the remaining digits to the carry one
                                      // by one and store the unit digit.
        finalsum.push_back(x % 10);
        carry = x / 10; // update the carry from each step.
        j--;
    }
    /* Step 4 starts here */

    while (carry)
    {                                   // If after finishing addition of the two numbers, if there is
                                        // still carry/leftover then we need to take it into the final
                                        // sum.
        finalsum.push_back(carry % 10); // Store digit one by one.
        carry = carry / 10;             // Reduce carry
    }
    /* Step 5 starts here */
    std::stringstream final_iter;
    // Since vector pushes value at last, the most significant digits starts at
    // the end of the vector. Thus print reverse.

    std::copy(finalsum.rbegin(), finalsum.rend(), std::ostream_iterator<int>(final_iter, ""));

    return final_iter.str();
}

void create_hololink_files()
{
    std::string HOLOSTONE_FILE = "HOLOSTONE.TXT";
    std::string THOUGHTFORM_A_FILE = "THOUGHTFORM_A.TXT";
    std::string THOUGHTFORM_B_FILE = "THOUGHTFORM_B.TXT";
    std::string AMPLIFIER_FILE = "AMPLIFIER.TXT";
    std::string INTENTIONS_FILE = "INTENTIONS.TXT";
    std::string HSUPLINK_FILE = "HSUPLINK.TXT";

    std::string HOLOLINK_CONTENTS = "#Comments are designated with a # prefix, and such commands are to be "
                                    "ignored by the Holo-Link.\r\n";
    HOLOLINK_CONTENTS += "#" + HSUPLINK_FILE + " CONFIG FILE v1.0\r\n";
    HOLOLINK_CONTENTS += "#Holo-Link framework created by Mystic Minds (2022).\r\n";
    HOLOLINK_CONTENTS += "#This implementation of the Holo-Link framework by Anthro Teacher.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "DECLARATION PRIMARY (Properties of thought forms and uplink):\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare the uplink multiply the energy received from the Holo-Stones "
                         "by Infinity and densify all energy to the highest amount to achieve "
                         "Instant Quantum Manifestation of the energetic programmings in " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare the Holo-Stones to funnel their energy into "
                         "HOLOSTONE.TXT.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare the Holo-Stones to amplify the power and "
                         "receptivity of the energetic programmings in " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare the Holo-Stones to multiply the strength of the energetic "
                         "programmings in " +
                         HSUPLINK_FILE + " and increase the potency at the most optimal rate.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare that all energetic programmings in " + HSUPLINK_FILE +
                         " be imprinted, imbued and amplified with the new "
                         "energy from the Holo-Stones.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += HOLOSTONE_FILE + ", " + AMPLIFIER_FILE + ", " + THOUGHTFORM_A_FILE + " AND " +
                         THOUGHTFORM_B_FILE +
                         " are extremely pure and of highest vibration and are "
                         "fully optimized for Instant Quantum Manifestation.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += THOUGHTFORM_A_FILE +
                         " is creating an unbreakable and continuous connection "
                         "and funnel energy to all energetic programmings in " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += THOUGHTFORM_A_FILE +
                         " uses energy from Infinite Source to continuously uphold a perfect link "
                         "between the Holo-Stones and the " +
                         HSUPLINK_FILE +
                         " to bring in infinitely more energy into all energetic programmings "
                         "in " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += THOUGHTFORM_B_FILE + " reinforces 100% of energy into all the energetic programmings in " +
                         HSUPLINK_FILE + " at the quantum level.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += THOUGHTFORM_B_FILE + " safely and efficiently removes all blockages in this system at the "
                                              "quantum level to allow for Instant Quantum Manifestation.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += HOLOSTONE_FILE + " feeds " + AMPLIFIER_FILE +
                         " which amplifies the energy and feeds it back to " + HOLOSTONE_FILE +
                         " and repeats it to the perfect intensity.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "All energetic programmings listed in " + HSUPLINK_FILE +
                         " are now amplified to the highest power, speed and quantum-level "
                         "precision using energy from the Holo-Stones which are sourced through " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += HOLOSTONE_FILE + " works with Earth's Crystal Grid in the most optimal "
                                          "way possible for Instant Quantum Manifestation.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "Earth's Power Grid is extremely pure, cool, clean, efficient, "
                         "optimized, and of highest vibration and is safely tapped in the most "
                         "optimal way possible by HOLOSTONE.TXT for Instant Quantum "
                         "Manifestation, and uses the least amount of electricity possible for "
                         "everyone who desires this.\r\n";
    HOLOLINK_CONTENTS += "UPLINK CORE (Reference any object, file, spell, etc. here):\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += HOLOSTONE_FILE + " (Receives and distributes energy to all objects, "
                                          "files, spells, etc referenced below):\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "[INSERT OBJECTS TO CHARGE]\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "INTENTIONS.TXT\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "DECLARATIONS SECONDARY (Add-ons that strengthen the "
                         "properties of the uplink itself):\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare the Holo-Stones will uplink their energy "
                         "into these energetic programmings in " +
                         HSUPLINK_FILE +
                         " to create instant, immediate and prominent results "
                         "optimally, efficiently and effortlessly.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare these energetic programmings in " + HSUPLINK_FILE +
                         " to grow stronger at the most optimal rate through the "
                         "ever-growing power of the Holo-Stones.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I call upon the Holo-Stones to channel the Atlantean Master Crystals, "
                         "Infinite Source, Earth's Crystal Grid and Earth's Power Grid directly "
                         "and utilize their energy as a funnel into HOLOSTONE.TXT which will then "
                         "funnel into the energetic programmings in " +
                         HSUPLINK_FILE + ".\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "The energetic programmings specified in " + HSUPLINK_FILE +
                         " are now being perfected and fully optimized.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I declare that the more the energetic programmings in " + HSUPLINK_FILE +
                         " are used, the stronger they become.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I am in my highest and most optimal reality/timeline.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "I am grounded, cleared, healed, balanced, "
                         "strong-willed and I release what I do not need.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "Every day, in every way, it's getting better and better.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "The Atlantean Master Crystals AND Earth's Crystal Grid "
                         "are open to Infinite Source.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "For my highest good and the highest good of all.\r\n";
    HOLOLINK_CONTENTS += "\r\n";
    HOLOLINK_CONTENTS += "Thank you. So be it. OM.\r\n";
    HOLOLINK_CONTENTS += "ALL ABOVE STATEMENTS RESPECT THE FREE WILL OF ALL INVOLVED.\r\n";

    ofstream HOLOSTONE_FILE_FILE(HOLOSTONE_FILE);
    HOLOSTONE_FILE_FILE << "HOLOSTONE";
    HOLOSTONE_FILE_FILE.close();
    ofstream THOUGHTFORM_A_FILE_FILE(THOUGHTFORM_A_FILE);
    THOUGHTFORM_A_FILE_FILE << "THOUGHTFORM A";
    THOUGHTFORM_A_FILE_FILE.close();
    ofstream THOUGHTFORM_B_FILE_FILE(THOUGHTFORM_B_FILE);
    THOUGHTFORM_B_FILE_FILE << "THOUGHTFORM B";
    THOUGHTFORM_B_FILE_FILE.close();
    ofstream AMPLIFIER_FILE_FILE(AMPLIFIER_FILE);
    AMPLIFIER_FILE_FILE << "AMPLIFIER";
    AMPLIFIER_FILE_FILE.close();
    ofstream HSUPLINK_FILE_FILE(HSUPLINK_FILE);
    HSUPLINK_FILE_FILE << HOLOLINK_CONTENTS;
    HSUPLINK_FILE_FILE.close();

    cout << LIGHTBLUE << "Holo-Link files created." << endl;
    cout << "Remember to create your INTENTIONS.TXT file, in this folder, with "
            "all your intentions for the Holo-Link."
         << endl;
    cout << "You may do one to a line, or however you feel." << endl;
    cout << "You may now run with the --usehololink option." << endl;
    cout << "When using --usehololink, the option --intent, will be ignored, and "
            "INTENTIONS.TXT will be used instead."
         << endl;
    cout << GREEN << "Good Luck!" << endl;
}

int main(int argc, char **argv)
{
    std::string intention, PROCESS_STATEMENT, process_intention, intention_value, duration, param_duration,
        param_intention, param_intention_2, param_timer, param_boostlevel, param_freq, param_color, param_usehololink,
        param_amplification, runtime_formatted, ref_rate, suffix_value = "HZ", HSUPLINK_FILE, param_restevery, param_restfor;
    unsigned long long int iterations = 0, multiplier = 0, amplification_int = 1000000000, cpu_benchmark_count = 0;
    int seconds = 0, frequency_int = 0, restevery_int = 0, restfor_int = 0;
    float ram_size_value = 1;

    // parse command line arguments
    param_duration = "UNTIL STOPPED";
    param_timer = "INEXACT";
    param_freq = "0";
    param_intention = "";
    param_color = "LIGHTBLUE";
    param_usehololink = "NO";
    param_boostlevel = "0";
    param_amplification = "1000000000";
    param_restevery = "0";
    param_restfor = "0";
    PROCESS_STATEMENT = "";
    HSUPLINK_FILE = "HSUPLINK.TXT";

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_help();
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--colorhelp"))
        {
            print_color_help();
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--dur"))
        {
            param_duration = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--timer"))
        {
            param_timer = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--imem"))
        {
            ram_size_value = atof(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-b") || !strcmp(argv[i], "--boostlevel"))
        {
            param_boostlevel = argv[i + 1];
        }
        else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--createnestingfiles"))
        {
            create_nesting_files();
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--intent"))
        {
            param_intention_2 = argv[i + 1];

            if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--usehololink"))
            {
                param_intention = HSUPLINK_FILE;
            }
            else
            {
                param_intention = param_intention_2;
            }
        }
        else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--usehololink"))
        {
            param_usehololink = "YES";
        }
        else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq"))
        {
            param_freq = argv[i + 1];
            frequency_int = std::stoi(param_freq);
        }
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--color"))
        {
            param_color = argv[i + 1];
            std::transform(param_color.begin(), param_color.end(), param_color.begin(), ::toupper);
        }
        else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--createhololinkfiles"))
        {
            create_hololink_files();
            exit(EXIT_SUCCESS);
        }
        else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--suffix"))
        {
            suffix_value = argv[i + 1];
            std::transform(suffix_value.begin(), suffix_value.end(), suffix_value.begin(), ::toupper);
        }
        else if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--amplification") || !strcmp(argv[i], "--amplify"))
        {
            param_amplification = argv[i + 1];
            amplification_int = std::stoll(param_amplification);
        }
        else if (!strcmp(argv[i], "-e") || !strcmp(argv[i], "--restevery"))
        {
            param_restevery = argv[i + 1];
            restevery_int = std::stoi(param_restevery);
        }
        else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--restfor"))
        {
            param_restfor = argv[i + 1];
            restfor_int = std::stoi(param_restfor);
        }
    }

    if (param_usehololink == "YES")
    {
        param_intention = HSUPLINK_FILE;
    }

    if (param_boostlevel != "0")
    {
        param_intention = "NEST-" + param_boostlevel + ".TXT";
    }

    unsigned long long int INTENTION_MULTIPLIER = (ram_size_value * 1024 * 1024 * 512);

#ifndef _WIN32
    // Set the terminal color based on the --color flag.
    if (param_color == "DEFAULT")
        cout << DEFAULT << std::flush;
    else if (param_color == "BLACK")
        cout << BLACK << std::flush;
    else if (param_color == "RED")
        cout << RED << std::flush;
    else if (param_color == "GREEN")
        cout << GREEN << std::flush;
    else if (param_color == "YELLOW")
        cout << YELLOW << std::flush;
    else if (param_color == "BLUE")
        cout << BLUE << std::flush;
    else if (param_color == "MAGENTA")
        cout << MAGENTA << std::flush;
    else if (param_color == "CYAN")
        cout << CYAN << std::flush;
    else if (param_color == "LIGHTGRAY")
        cout << LIGHTGRAY << std::flush;
    else if (param_color == "DARK_GRAY")
        cout << DARKGRAY << std::flush;
    else if (param_color == "LIGHTRED")
        cout << LIGHTRED << std::flush;
    else if (param_color == "LIGHTGREEN")
        cout << LIGHTGREEN << std::flush;
    else if (param_color == "LIGHTYELLOW")
        cout << LIGHTYELLOW << std::flush;
    else if (param_color == "LIGHTBLUE")
        cout << LIGHTBLUE << std::flush;
    else if (param_color == "LIGHTMAGENTA")
        cout << LIGHTMAGENTA << std::flush;
    else if (param_color == "LIGHTCYAN")
        cout << LIGHTCYAN << std::flush;
    else if (param_color == "WHITE")
        cout << WHITE << std::flush;

#else
    if (param_color == "BLACK")
        SetConsoleTextAttribute(hConsole, BLACK);
    else if (param_color == "BLUE")
        SetConsoleTextAttribute(hConsole, BLUE);
    else if (param_color == "GREEN")
        SetConsoleTextAttribute(hConsole, GREEN);
    else if (param_color == "CYAN")
        SetConsoleTextAttribute(hConsole, CYAN);
    else if (param_color == "RED")
        SetConsoleTextAttribute(hConsole, RED);
    else if (param_color == "MAGENTA")
        SetConsoleTextAttribute(hConsole, MAGENTA);
    else if (param_color == "LIGHTYELLOW")
        SetConsoleTextAttribute(hConsole, LIGHTYELLOW);
    else if (param_color == "LIGHTGRAY")
        SetConsoleTextAttribute(hConsole, LIGHTGRAY);
    else if (param_color == "DARKGRAY")
        SetConsoleTextAttribute(hConsole, DARKGRAY);
    else if (param_color == "LIGHTBLUE")
        SetConsoleTextAttribute(hConsole, LIGHTBLUE);
    else if (param_color == "LIGHTGREEN")
        SetConsoleTextAttribute(hConsole, LIGHTGREEN);
    else if (param_color == "LIGHTCYAN")
        SetConsoleTextAttribute(hConsole, LIGHTCYAN);
    else if (param_color == "LIGHTRED")
        SetConsoleTextAttribute(hConsole, LIGHTRED);
    else if (param_color == "LIGHTMAGENTA")
        SetConsoleTextAttribute(hConsole, LIGHTMAGENTA);
    else if (param_color == "YELLOW")
        SetConsoleTextAttribute(hConsole, YELLOW);
    else if (param_color == "WHITE")
        SetConsoleTextAttribute(hConsole, WHITE);
#endif // Not Windows

    std::locale comma_locale(std::locale(), new comma_numpunct());
    std::cout.imbue(comma_locale);

    cout << "Intention Repeater MAX v5.7 (c)2020-2022 by Anthro Teacher aka "
            "Thomas Sweet."
         << endl;
    cout << "Performance benchmark, exponents and flags by Karteek Sheri." << endl;
    cout << "Holo-Link framework originally created by Mystic Minds. This "
            "implementation by Anthro Teacher."
         << endl;
    cout << "Boosting through Nested Files and Intention Multiplying created by Anthro Teacher." << endl;

    if (param_usehololink == "YES")
    {
        param_intention = HSUPLINK_FILE;
        cout << "USING HOLO-LINK." << endl
             << endl;
    }

    if ((param_intention) == "")
    {
        cout << "Intention: ";
        std::getline(std::cin, intention);
    }
    else
    {
        intention = param_intention;
    }

    if (param_freq == "0") // Only use multiplier if --freq flag is not set or
                           // specifically said not to use it.
    {
        cout << "LOADING INTENTION INTO MEMORY" << std::flush;
        // Repeat string till it is more than INTENTION_MULTIPLIER characters long.
        while (intention_value.length() < INTENTION_MULTIPLIER)
        {
            intention_value += intention;
            ++multiplier;
        }
        --multiplier; // Account for having to reduce at the end.

        // Now, remove enough characters at the end to account for the process
        // statement to limit to less than 1024 characters.
        long long int intention_value_length = intention_value.length();
        long long int intention_length = intention.length();
        int process_statement_length = PROCESS_STATEMENT.length();
        long long int intention_length_val = intention_value_length - intention_length - process_statement_length;

        intention_value = intention_value.substr(0, intention_length_val);
        intention_value += PROCESS_STATEMENT;
    } // End Multiplier (when not using frequency)

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
                while ((chrono::duration_cast<chrono::seconds>(end - start).count() < 1))
                {
                    process_intention = intention_value; // This is the Intention Repeater call that
                                                         // actually does the work with the Servitor
                                                         // [HR6819].
                    iterations++;
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
                    std::cout << "[" + runtime_formatted + "]"
                              << " (" << setprecision(3) << fixed
                              << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / "
                              << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1
                              << " Hz): " << intention << "     \r" << std::flush;
                }
                else // suffix_value = "HZ"
                {
                    std::cout << "[" + runtime_formatted + "]"
                              << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / "
                              << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency")
                              << "Hz): " << intention << "     \r" << std::flush;
                }
                iterations = 0;
                if (runtime_formatted == duration)
                {
                    std::cout << endl
                              << std::flush;
                    exit(EXIT_SUCCESS);
                }

                // If rest is chosen, then rest for specified time every interval.
                if ((restevery_int > 0) && (seconds % restevery_int == 0))
                {
                    start = std::chrono::high_resolution_clock::now();
                    end = std::chrono::high_resolution_clock::now();

                    if (suffix_value == "EXP")
                    {
                        std::cout << "[" + runtime_formatted + "]"
                                  << " (" << setprecision(3) << fixed
                                  << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / "
                                  << "0 Hz): " << intention << "      \r" << std::flush;
                    }
                    else // suffix_value = "HZ"
                    {
                        std::cout << "[" + runtime_formatted + "]"
                                  << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / "
                                                                                                            "0 Hz): "
                                  << intention << "      \r" << std::flush;
                    }
                    while ((chrono::duration_cast<chrono::seconds>(end - start).count() < restfor_int))
                    {
                        end = std::chrono::high_resolution_clock::now();
                    }
                }
            } while (1);
        }
        else // End param_timer = EXACT
        {    // BEGIN INEXACT timer

            auto b_start = std::chrono::high_resolution_clock::now();
            auto b_end = std::chrono::high_resolution_clock::now();

            while ((std::chrono::duration_cast<std::chrono::seconds>(b_end - b_start).count() < 1))
            {
                process_intention = intention_value; // The Intention Repeater Statement
                cpu_benchmark_count++;
                b_end = std::chrono::high_resolution_clock::now();
            }
            // Benchmark ends here
            if (amplification_int > cpu_benchmark_count) // Make sure the amplification doesn't exceed the benchmark
            {
                amplification_int = cpu_benchmark_count;
            }

            do
            {
                start = std::chrono::high_resolution_clock::now();
                end = std::chrono::high_resolution_clock::now();
                while ((chrono::duration_cast<chrono::seconds>(end - start).count() < 1))
                {
                    for (unsigned long long int i = 0; i < amplification_int; i++)
                    {
                        process_intention = intention_value; // This is the Intention Repeater call that
                                                             // actually does the work with the Servitor
                                                             // [HR6819].
                    }

                    iterations += amplification_int;
                    end = std::chrono::high_resolution_clock::now();
                }
                seconds++;
                runtime_formatted = FormatTimeRun(seconds);
                iterations_string_freq = to_string(iterations * multiplier);
                iterations_string = findsum(iterations_string, iterations_string_freq);
                digits = iterations_string.length();

                freq_digits = iterations_string_freq.length();

                if (suffix_value == "EXP")
                {
                    std::cout << "[" + runtime_formatted + "]"
                              << " (" << setprecision(3) << fixed
                              << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / "
                              << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3)) << "x10^" << freq_digits - 1
                              << " Hz): " << intention << "     \r" << std::flush;
                }
                else // suffix_value = "HZ"
                {
                    std::cout << "[" + runtime_formatted + "]"
                              << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / "
                              << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency")
                              << "Hz): " << intention << "     \r" << std::flush;
                }
                iterations = 0;
                if (runtime_formatted == duration)
                {
                    std::cout << endl
                              << std::flush;
                    exit(EXIT_SUCCESS);
                }

                // If rest is chosen, then rest for specified time every interval.
                if ((restevery_int > 0) && (seconds % restevery_int == 0))
                {
                    start = std::chrono::high_resolution_clock::now();
                    end = std::chrono::high_resolution_clock::now();

                    if (suffix_value == "EXP")
                    {
                        std::cout << "[" + runtime_formatted + "]"
                                  << " (" << setprecision(3) << fixed
                                  << (stoull(iterations_string.substr(0, 4))) / pow(10, 3) << "x10^" << digits - 1 << " / "
                                  << "0 Hz): " << intention << "      \r" << std::flush;
                    }
                    else // suffix_value = "HZ"
                    {
                        std::cout << "[" + runtime_formatted + "]"
                                  << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / "
                                                                                                            "0 Hz): "
                                  << intention << "      \r" << std::flush;
                    }
                    while ((chrono::duration_cast<chrono::seconds>(end - start).count() < restfor_int))
                    {
                        end = std::chrono::high_resolution_clock::now();
                    }
                }
            } while (1);
        } // End param_timer INEXACT
    }
    else                           // End param_freq = 0
    {                              // Begin param_freq nonzero
        bool freq_sentinel = true; // Determins each second if the # of iterations
                                   // has reached the frequency of repeating.
        do
        {
            start = std::chrono::high_resolution_clock::now();
            end = std::chrono::high_resolution_clock::now();
            freq_sentinel = true;
            while ((std::chrono::duration_cast<std::chrono::seconds>(end - start).count() != 1))
            {
                if (freq_sentinel)
                {
                    for (int i = 0; i < frequency_int; ++i)
                    {
                        process_intention = intention_value; // This is the Intention Repeater call that
                                                             // actually does the work with the Servitor
                                                             // [HR6819].
                        ++iterations;
                        end = std::chrono::high_resolution_clock::now();
                        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() == 1)
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
                std::cout << "[" + runtime_formatted + "]"
                          << " (" << setprecision(3) << fixed << (stoull(iterations_string.substr(0, 4))) / pow(10, 3)
                          << "x10^" << digits - 1 << " / " << (stoull(iterations_string_freq.substr(0, 4)) / pow(10, 3))
                          << "x10^" << freq_digits - 1 << " Hz): " << intention << "     \r" << std::flush;
            }
            else // suffix_value = "HZ"
            {
                std::cout << "[" + runtime_formatted + "]"
                          << " (" << display_suffix(iterations_string, digits - 1, "Iterations") << " / "
                          << display_suffix(iterations_string_freq, freq_digits - 1, "Frequency")
                          << "Hz): " << intention << "     \r" << std::flush;
            }
            iterations = 0;
            if (runtime_formatted == duration)
            {
                std::cout << endl
                          << std::flush;
                exit(EXIT_SUCCESS);
            }
        } while (1);
    } // End repetition_period nonzero

#ifndef _WIN32
    cout << WHITE << std::flush;
#else
    SetConsoleTextAttribute(hConsole, WHITE);
#endif

    return 0;
}
