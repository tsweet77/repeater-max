/*
    Intention Repeater MAX v2.0 created by Thomas Sweet.
    Created 10/23/2020 for C++.
    Repeats your intention millions of times per second to make things happen.
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

std::string FormatTimeRun(int seconds_elapsed, std::string durationval);

const int ONE_HOUR = 3600;
const int ONE_MINUTE = 60;

class comma_numpunct: public std::numpunct < char > {
    protected: virtual char do_thousands_sep() const {
        return ',';
    }

    virtual std::string do_grouping() const {
        return "\03";
    }
};

int main(int argc, char ** argv) {
    std::string intention, intention_value, process_intention, process_statement, status, param_duration, param_intention, duration, runtime_formatted;
    unsigned long long int iterations = 0;
    int seconds = 0;

    if (argc == 3) {
        param_duration = argv[1];
        param_intention = argv[2];

    } else {
        param_duration = "Until Manually Stopped";
        param_intention = "";
    }

    std::locale comma_locale(std::locale(), new comma_numpunct());
    std::cout.imbue(comma_locale);

    cout << "Intention Repeater MAX v2.0 created by Thomas Sweet." << endl;
    cout << "This software comes with no guarantees or warranty of any kind and is for entertainment purposes only." << endl;
    cout << "Press Ctrl-C to quit." << endl << endl;

    process_statement = ": ONE INFINITE CREATOR. INTELLIGENT INFINITY. INFINITE ENERGY. INTELLIGENT ENERGY. LOGOS. HR 6819. BY GRACE. IN COOPERATION WITH FATHER GOD, MOTHER GODDESS, AND SOURCE. PURE ADAMANTINE PARTICLES OF LOVE/LIGHT. IN THE HIGHEST AND GREATEST GOOD OF ALL, REQUESTING AID FROM ALL BEINGS WHO ARE WILLING TO ASSIST. METATRONS CUBE. 0010110. GREAT CENTRAL SUN. SIRIUS A. SIRIUS B. SOL. ALL AVAILABLE BENEFICIAL ENERGY GRIDS OF EARTH/GAIA FOCUSED THROUGH CRYSTAL GRID OF EARTH/GAIA. NODES AND NULLS OF EARTH/GAIA. CREATE STABILIZATION FIELD. CREATE ZONE OF MANIFESTATION. ALL AVAILABLE ORGONE AETHER RESONATORS. ALL AVAILABLE ORGONE BUBBLES. USE EVERY AVAILABLE RESOURCE (RESPECTING FREE WILL). MANIFEST ASAP AT HIGHEST DENSITY POSSIBLE INTO BEST DENSITY FOR USER. CREATE STRUCTURE. 963HZ GOD FREQUENCY. 432HZ MANIFESTATION. CANCEL DESTRUCTIVE OR FEARFUL INTENTIONS. PURIFY THE ENERGY. CLEAR THE BLOCKAGES. REGULATE AND BALANCE THE ENERGY. USE THE MOST EFFECTIVE PATH IN THE MOST EFFICIENT WAY. FULLY OPTIMIZE THE ENERGY. INTEGRATE THE ENERGY. PROCESS THE CHANGES. GUIDED BY MY HIGHER SELF. GROUNDED TO GAIA, CONNECTED TO SOURCE, INTEGRATING BOTH WITHIN THE SACRED HEART. SEND ALL SPECIFIED INTENTIONS, AFFIRMATIONS, AND/OR DESIRED MANIFESTATIONS, OR BETTER. PLEASE HELP USER TO RAISE THEIR VIBRATION TO THE LEVEL REQUIRED TO MAKE THEIR SPECIFIED INTENTIONS, AFFIRMATIONS, AND/OR DESIRES MANIFEST. IF THE USER IS UNABLE TO ACHIEVE THE NECESSARY VIBRATION TO MANIFEST THEIR SPECIFIED INTENTIONS, AFFIRMATIONS, AND/OR DESIRES, BUT THERE ARE BEINGS WHO COULD AND THEY ARE WILLING TO ASSIST IN MANIFESTING THE SPECIFIED INTENTIONS, AFFIRMATIONS, AND/OR DESIRES, PLEASE ENLIST THEIR AID. IT IS DONE. SO SHALL IT BE. NOW RETURN A PORTION OF THE LOVE/LIGHT RECEIVED AND ACTIVATED BACK INTO THE HIGHER REALMS OF CREATION. I LOVE YOU AND THANK YOU. GUT/HEART/MIND COHERENCE WITH REPEATER. CLEAR INTERFERENCE. FOCUS DOWN FROM AKASHIC RECORDS. SUPERCOOLED MOST PERFECTLY BALANCED, PURIST AND MOST POWERFUL QUASAR. OM";

    if ((param_intention) == "") {
        cout << "Intention: ";
        std::getline(std::cin, intention);

    } else {
        intention = param_intention;
    }

    intention_value = intention + process_statement;

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
        runtime_formatted = FormatTimeRun(seconds, duration);
        std::cout << "[" + runtime_formatted + "]" << " (" << std::setprecision(0) << std::fixed << iterations << "): " << intention << "\r" << std::flush;

        if (runtime_formatted == duration) {
            cout << "\n" << std::flush;
            exit(0);
        }
    } while (1);
    return 0;
}

std::string FormatTimeRun(int seconds_elapsed, std::string durationval) {
    int hour, min, sec;

    std::string hour_formatted, min_formatted, sec_formatted, runtime_formatted, duration;

    duration = "[" + durationval + "]";
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

    runtime_formatted = hour_formatted + ":" + min_formatted + ":" + sec_formatted;

    return runtime_formatted;
}