/*
    Intention Repeater MAX v2.2 created by Thomas Sweet.
    Created 10/25/2020 for C++.
	Compile on Windows using: g++ .\intention_repeater_max.cpp -O3 -o .\intention_repeater_max.exe
	And Compile on Linux using: g++ ./intention_repeater_max.cpp -O3 -o ./intention_repeater_max
	On 64-bit: MingW-W64-builds: http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download
    On 32-bit (or PCs with only 4GB or less of RAM: https://osdn.net/projects/mingw/releases/
    Repeats your intention up to 30+ million times per second to make things happen.
    When compiled, this is more powerful than the Python version.
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

#include <iostream>

#include <time.h>

#include <ctime>

#include <ratio>

#include <chrono>

#include <iomanip>

#include <locale.h>

using namespace std;
using namespace std::chrono;

std::string FormatTimeRun(int seconds_elapsed);

#define ONE_MINUTE 60
#define ONE_HOUR 3600
#define PROCESS_STATEMENT ": ONE INFINITE CREATOR. REQUESTING AID FROM ALL BEINGS WHO ARE WILLING TO ASSIST. METATRON'S CUBE. ALL AVAILABLE BENEFICIAL ENERGY GRIDS, ORGONE AETHER RESONATORS, & ORGONE BUBBLES. CREATE MANIFESTATION ZONE. ASCENSION PYRAMID. USE EVERY AVAILABLE RESOURCE. MANIFEST ASAP. CREATE STRUCTURE. CANCEL DESTRUCTIVE OR FEARFUL INTENTIONS, OR INTENTIONS THAT CONFLICT WITH THE HIGHEST AND GREATEST GOOD OF THE USER. REGULATE AND BALANCE THE ENERGY. USE THE MOST EFFECTIVE PATH IN THE MOST EFFICIENT WAY. OPTIMAL ENERGY. INTEGRATE THE ENERGY IN THE MOST EFFECTIVE AND PLEASANT WAY POSSIBLE. PROCESS THE CHANGES. GUIDED BY THE USER'S HIGHER SELF. CONNECTED TO SOURCE. ENABLE AND UTILIZE THE SACRED HEART, QUANTUM HEART, AND QUANTUM MIND. MANIFEST ALL SPECIFIED INTENTIONS AND/OR DESIRES, OR BETTER. IF IT WOULD AID IN THE MANIFESTATION PROCESS, PLEASE HELP USER TO SENSE AND EMOTIONALLY FEEL WHAT IT WOULD BE LIKE TO ALREADY BE EXPERIENCING THEIR SPECIFIED INTENTIONS AND/OR DESIRES NOW. PLEASE HELP USER TO RAISE THEIR VIBRATION TO THE LEVEL REQUIRED TO MAKE THEIR SPECIFIED INTENTIONS AND/OR DESIRES MANIFEST. ASSIST THE USER WITH ACHIEVING OPTIMAL GUT/HEART/MIND COHERENCE WITH THEIR SPECIFIED INTENTIONS AND/OR DESIRES. IF IT WOULD BENEFIT THE USER, ASSIST THEM WITH CLEARING & RELEASING ANY/ALL INTERNAL OR EXTERNAL INTERFERENCE OR BLOCKAGES TO THEIR SPECIFIED INTENTIONS AND/OR DESIRES. IT IS DONE. NOW RETURN A PORTION OF THE LOVE/LIGHT RECEIVED AND ACTIVATED BACK INTO THE HIGHER REALMS OF CREATION. I LOVE YOU. OM."

class comma_numpunct: public std::numpunct < char > {
    protected: virtual char do_thousands_sep() const {
        return ',';
    }

    virtual std::string do_grouping() const {
        return "\03";
    }
};

int main(int argc, char ** argv) {
    std::string intention, intention_value, process_intention, duration, param_duration, param_intention, runtime_formatted;
    unsigned long long int iterations = 0;
    int seconds = 0;

    if (argc == 3) {
        param_duration = argv[1];
        param_intention = argv[2];

    } else {
        param_duration = "Until Stopped";
        param_intention = "";
    }

    std::locale comma_locale(std::locale(), new comma_numpunct());
    std::cout.imbue(comma_locale);

    cout << "Intention Repeater MAX v2.2 created by Thomas Sweet." << endl;
    cout << "This software comes with no guarantees or warranty of any kind and is for entertainment purposes only." << endl;
    cout << "Press Ctrl-C to quit." << endl << endl;

    if ((param_intention) == "") {
        cout << "Intention: ";
        std::getline(std::cin, intention);

    } else {
        intention = param_intention;
    }

    intention_value = intention + PROCESS_STATEMENT;

    duration = param_duration;

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    do {
        start = std::chrono::system_clock::now();
        while ((std::chrono::duration_cast < std::chrono::seconds > (end - start).count() != 1)) {
            process_intention = intention_value; //This is the Intention Repeater call that actually does the work with the Servitor.
            iterations += 1;
            end = std::chrono::system_clock::now();
        }
        seconds += 1;
        runtime_formatted = FormatTimeRun(seconds);
        std::cout << "[" + runtime_formatted + "]" << " (" << std::setprecision(0) << std::fixed << iterations << "): " << intention << "\r" << std::flush;

        if (runtime_formatted == duration) {
			std::cout << endl << std::flush;
            exit(0);
        }
    } while (1);
	std::cout << endl << std::flush;
    return 0;
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