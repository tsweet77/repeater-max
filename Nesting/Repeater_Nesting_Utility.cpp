//Intention Repeater Nesting Utility created by Anthro Teacher aka Thomas Sweet.
//May 20, 2021.

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	std::string num_files, num_repetitions, filename, create_filename;
	int repnum, filenum, current_filenum;
	
	cout << "Intention Repeater Nesting File Creation Utility v1.0." << endl;
	cout << "Created by Anthro Teacher (5/21/2021)." << endl;
	cout << "Note: 10 reps per file X 100 files = 1 Googol Repetitions per iteration!" << endl;
	cout << "Number of total Reps = (# reps per file) ^ (# files)." << endl << endl;
	
	cout << "Filename you would like to use in the Repeater for Nesting (Not INTENTIONS.TXT): ";
	std::getline(std::cin, create_filename);
	
	cout << endl;
	
    cout << "How many nesting files to create: ";
    std::getline(std::cin, num_files);
    
    cout << endl;
    
    cout << "How many repetitions in each file: ";
    std::getline(std::cin, num_repetitions);
	
	cout << endl;
	
	std::fstream myfile;
	
	filename = "NEST-0.TXT";
	
	myfile.open(filename,ios::out);

	
	if(myfile.is_open())
	{
		for (repnum=1; repnum<=stoi(num_repetitions); repnum++)
		{
			myfile << "INTENTIONS.TXT\r\n";
		}
	}

	myfile.close();	
	
	if (stoi(num_files) > 1)
	{
		for (filenum=1; filenum<=stoi(num_files); filenum++)
		{
			filename = "NEST-" + std::to_string(filenum) + ".TXT";
			myfile.open(filename,ios::out);
			
			for (repnum=1; repnum<=stoi(num_repetitions); repnum++)
			{
				myfile << "NEST-" + std::to_string(filenum - 1) + ".TXT\r\n";
			}
			myfile.close();
			current_filenum = filenum;
		}
		filename = create_filename;
		myfile.open(filename,ios::out);
		for (repnum=1; repnum<=stoi(num_repetitions); repnum++)
			{
				myfile << "NEST-" + std::to_string(filenum-1) + ".TXT\r\n";
			}
		myfile.close();
	}
	
	cout << "Intention Repeater Nesting Files Written." << endl << endl;
	
	cout << "Be sure to have your intentions in the INTENTIONS.TXT file." << endl << endl;
	
	cout << "And use " << create_filename << " in the Intention Repeater to utilize the full NEST stack." << endl << endl;
	
	return 0;
}
