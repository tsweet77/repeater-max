// Intention Repeater Nesting Utility created by Anthro Teacher aka Thomas
// Sweet. May 22, 2021. Version 2.0

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

int main() {
  std::string num_files, num_repetitions, create_filename, archive_7zip;
  char command[128];
  char filename[] = "NEST-FULLPOWER.ZIP";

  int repnum, filenum, current_filenum;
  std::fstream myfile;

  cout << "Intention Repeater Nesting File Creation Utility v2.0." << endl;
  cout << "Created by Anthro Teacher (5/22/2021)." << endl;
  cout << "Note: 10 reps per file X 100 files = 1 Googol Repetitions per "
          "iteration!"
       << endl;
  cout << "Number of total Reps = (# reps per file) ^ (# files)." << endl
       << endl;

  cout << "Archive into 7zip? (y/N): ";
  std::getline(std::cin, archive_7zip);

  if (archive_7zip != "Y" && archive_7zip != "y") {
    cout << "Filename you would like to use in the Repeater for Nesting (Not "
            "INTENTIONS.TXT): ";
    std::getline(std::cin, create_filename);
    archive_7zip = "N";

  } else {
    archive_7zip = "Y";
  }

  cout << endl;

  cout << "How many nesting files to create: ";
  std::getline(std::cin, num_files);

  cout << endl;

  cout << "How many repetitions in each file: ";
  std::getline(std::cin, num_repetitions);

  cout << endl;

  if (archive_7zip == "N") {
    myfile.open("NEST-0.TXT", ios::out);
  } else {
    myfile.open("NEST-0.TXT", ios::out);
  }

  if (myfile.is_open()) {
    for (repnum = 1; repnum <= stoi(num_repetitions); repnum++) {
      myfile << "INTENTIONS.TXT\r\n";
    }
  }

  myfile.close();

  if (archive_7zip == "Y") {
    cout << "Adding File # 1 / " << num_files << "\r\n";
    snprintf(command, 128, "7z a -mx=9 NEST-FULLPOWER.ZIP NEST-0.TXT >> logfile.txt",
             filename);
    system(command);
    system("rm NEST-0.TXT");
  }

  if (stoi(num_files) > 1) {
    for (filenum = 1; filenum <= stoi(num_files); filenum++) {
      create_filename = "NEST-" + std::to_string(filenum) + ".TXT";
      myfile.open(create_filename, ios::out);

      for (repnum = 1; repnum <= stoi(num_repetitions); repnum++) {
        myfile << "NEST-" + std::to_string(filenum - 1) + ".TXT\r\n";
      }
      myfile.close();

      if (archive_7zip == "Y") {
        cout << "Adding File # " << std::to_string(filenum) << " / " << num_files << "\r\n";
        snprintf(command, 128,
                 "7z a -mx=9 NEST-FULLPOWER.ZIP NEST-%d.TXT >> logfile.txt", filenum);
        system(command);
        snprintf(command, 128, "rm NEST-%d.TXT", filenum);
        system(command);
      }

      current_filenum = filenum;
    }
  }

  cout << "Intention Repeater Nesting Files Written." << endl << endl;

  cout << "Be sure to have your intentions in the INTENTIONS.TXT file." << endl
       << endl;

  if (archive_7zip == "N") {

  cout << "And use " << create_filename
       << " in the Intention Repeater to utilize the full NEST stack." << endl
       << endl;
       
  } else {
  cout << "And use NEST-FULLPOWER.ZIP"
       << " in the Intention Repeater to utilize the full NEST stack." << endl
       << endl;
  }

  return 0;
}
