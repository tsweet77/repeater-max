//Code by Anthro Teacher (5/20/2021) to demonstrate
//how to repeat your intention 1,000 times to a file.
//https://forums.intentionrepeater.com

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
  std::string file_to_write =
      "repeated_intention.txt"; // This is the file you wish to write to.

  std::string intention =
      "It feels like I am at Disneyland."; // This is our intention we wish to
                                           // repeat.
  std::string intention_value = ""; // This is the variable we are appending our
                                    // intention to, in order to multiply it.

  for (int a = 1; a <= 1E3; a += 1) { // Repeat a thousand times.
    intention_value = intention_value + intention + "\r\n";
  }
  
  ofstream myfile;
  myfile.open (file_to_write);
  myfile << intention_value;
  myfile.close();

  return 0;
}
