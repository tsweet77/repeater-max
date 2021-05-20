//Code by Anthro Teacher (5/20/2021) to demonstrate
//how to multiply your intention in a variable, up to 1 GB in size.
//https://forums.intentionrepeater.com

#include <stdio.h>
#include <string>

using namespace std;

int main() {
  std::string intention =
      "It feels like I am at Disneyland."; // This is our intention we wish to
                                           // repeat.
  std::string intention_value = ""; // This is the variable we are appending our
                                    // intention to, in order to multiply it.
  std::string repeat_intention =
      ""; // This calls the Servitor implicitly. It doesn't matter the name of
          // the variable, only that you are assigning to it repeatedly.

  int ram_size_value = 1; // Set how many GB of RAM to allocate
  unsigned long long int INTENTION_MULTIPLIER =
      (ram_size_value * 1024 * 1024 *
       512); /* We use 512 in here, because we have two variables,
                repeat_intention and intention_value, that hold the full
                intention values, and we want total memory used to be 1GB.
                Therefore, with these two variables, they both need to fit in
                the 1GB. */

  while (intention_value.length() < INTENTION_MULTIPLIER) {
    intention_value += intention; // Append our intention into the
                                  // intention_value variable, to multiply it.
  }

  for (int a = 1; a <= 1E6; a += 1) { // Repeat our 1GB string a million times.
    repeat_intention = intention_value;
  }

  return 0;
}
