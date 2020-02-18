//============================================================================
// Name        : Project2.cpp
// Author      : KP
// Version     :
// Copyright   : Your copyright notice
// Description : tester, for arrayfunctions.cpp
//				 to autorun on linux, from the Project2_Solution/Debug folder
//				 ./make clean
//				 ./make
//				 then from the Project2_Solution folder
//				 ./Debug/Project2_Solution.ex studentid
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "constants.h"
#include "utilities.h"
#include "array_functions.h"

using namespace std;
using namespace constants;

//tracks how many points you will get out of 100
//yeah, yeah its a global
int total_points =0;

//this is a template class, its mostly here as a helper for me
//the T and U are generic params, I can substitute any type for them
//they must be comparable with ==, templates are extremely hard to get right BTW
template<typename T, typename U>
bool EXPECT_EQ(T expectedVal, U actualVal,string testnumb = "", int pts=ONE_POINTS){
	bool bout = (expectedVal == actualVal);
	if (bout){
		total_points+=pts;
		cout<<"SUCCESS "+testnumb<<" points:"<<total_points;
	}
	else
		cout<<"FAIL "+ testnumb;
	cout<<endl;
	return bout;
}

bool does_file_exist(const std::string &file_name){
	std::ifstream fle(file_name);
	return fle.good();
}

//runs a diff command, I'm using (GNU diffutils) 3.3
//this code cadged from stack overflow
bool diff_files(string testoutput,string correctfile, string testnumb = "" ){

	const int SMALL_BUFFER = 512;
	if (testoutput.length()== 0 || correctfile.length()==0)
		return false;

	//build shell command
	string command ="diff "+  testoutput + " " + correctfile;

	FILE *in;
	char buff[SMALL_BUFFER];

	if(!(in = popen(command.c_str(), "r")))
		return false;

	if (fgets(buff, sizeof(buff), in)==NULL)
		return true;
	else
		cout << buff;

	pclose(in);
	return false;
}

//gets rid of the legacy .out files
void remove_file_debris(){
	popen("rm ./output/*.out", "r");
}

//worth 17 points
void test_ProcessToken(){
	clearArray();
	EXPECT_EQ(0, getArraySize(),"1");

	//make sure you strip the rubbish chars
	string mystring = " ";
	processToken(mystring);
	EXPECT_EQ(0, getArraySize(),"2");

	mystring = "\r ";
	processToken(mystring);
	EXPECT_EQ(0, getArraySize(),"3");

	mystring = ".\r";
	processToken(mystring);
	EXPECT_EQ(0, getArraySize(),"4");

	//start adding real chars
	mystring = "z";
	processToken(mystring);
	EXPECT_EQ(1, getArraySize(),"5");

	mystring = "z";
	processToken(mystring);
	EXPECT_EQ(1, getArraySize(),"6");

	//add 1 more so there are 3 z's
	mystring = "z";
	processToken(mystring);

	//seen z twice now
	EXPECT_EQ((string)"z", getArrayWordAt(0),"7");
	EXPECT_EQ(3, getArrayWord_NumbOccur_At(0),"8");

	//add some different words
	mystring = "lizard";
	processToken(mystring);
	mystring = "is";
	processToken(mystring);
	mystring = "green";
	processToken(mystring);
	mystring = "green";
	processToken(mystring);

	EXPECT_EQ(4, getArraySize(),"9");

	sortArray(ASCENDING);
	EXPECT_EQ("green", getArrayWordAt(0),"10_A");
	EXPECT_EQ(2, getArrayWord_NumbOccur_At(0),"11_A");
	EXPECT_EQ((string)"is", getArrayWordAt(1),"12_A");
	EXPECT_EQ(1, getArrayWord_NumbOccur_At(1),"13_A");
	EXPECT_EQ((string)"lizard", getArrayWordAt(2),"14_A");
	EXPECT_EQ(1, getArrayWord_NumbOccur_At(2),"15_A");
	EXPECT_EQ((string)"z", getArrayWordAt(3),"16_A");
	EXPECT_EQ(3, getArrayWord_NumbOccur_At(3),"17_A");

	sortArray(DESCENDING);
	EXPECT_EQ("z", getArrayWordAt(0),"10_D");
	EXPECT_EQ(3, getArrayWord_NumbOccur_At(0),"11_D");
	EXPECT_EQ((string)"lizard", getArrayWordAt(1),"12_D");
	EXPECT_EQ(1, getArrayWord_NumbOccur_At(1),"13_D");
	EXPECT_EQ((string)"is", getArrayWordAt(2),"14_D");
	EXPECT_EQ(1, getArrayWord_NumbOccur_At(2),"15_D");
	EXPECT_EQ((string)"green", getArrayWordAt(3),"16_D");
	EXPECT_EQ(2, getArrayWord_NumbOccur_At(3),"17_D");

	sortArray(NUMBER_OCCURRENCES);
	EXPECT_EQ((string)"z", getArrayWordAt(0),"16_N");
	EXPECT_EQ(3, getArrayWord_NumbOccur_At(0),"17_N");
	EXPECT_EQ("green", getArrayWordAt(1),"10_N");
	EXPECT_EQ(2, getArrayWord_NumbOccur_At(1),"11_N");
}

//worth 12 points
void test_ProcessLine(){
	clearArray();
	EXPECT_EQ(0, getArraySize(),"18",TWO_POINTS);

	string mystring = "I would like a. bit of butter and A slice of toast.\r";
	processLine(mystring);
	EXPECT_EQ(10, getArraySize(),"19",TWO_POINTS);

	//make sure 2 a's
	EXPECT_EQ("a", getArrayWordAt(3),"20",TWO_POINTS);
	EXPECT_EQ(2, getArrayWord_NumbOccur_At(3),"21",TWO_POINTS);

	//make sure toast is correct
	EXPECT_EQ("toast", getArrayWordAt(9),"22",TWO_POINTS);
	EXPECT_EQ(1, getArrayWord_NumbOccur_At(9),"23",TWO_POINTS);
}

//worth 20 points
void test_File_OpenClose(){
	fstream myfstream;

	//test bogus file
	string filename = TEST_DATA_NON_EXISTANT;
	EXPECT_EQ (false, openFile(myfstream, filename),"24",FIVE_POINTS);
	EXPECT_EQ (false, myfstream.is_open(),"25",FIVE_POINTS);

	//test real file
	filename = TEST_DATA_FULL;
	EXPECT_EQ (true, openFile(myfstream, filename),"26",FIVE_POINTS);
	EXPECT_EQ (true, myfstream.is_open(),"27",FIVE_POINTS);
	closeFile(myfstream);
}

//see if we can open, process, sort, and write the file back to disk
bool test_File(string filename, sortOrder mysortorder= NONE){
	clearArray();
	fstream myfstream;
	if (!openFile(myfstream, filename))
		return false;

	if (!processFile(myfstream))
		return false;

	sortArray(mysortorder);
	string name = TEST_DATA_FULL_OUT;
	if ( writeArraytoFile(name)!= SUCCESS)
		return false;

	//NOTE: C++ does not support try-finally
	//it expects you to use destructors instead
	//fine for object oriented programming
	//very restrictive for structured programming
	closeFile(myfstream);
	return true;
}

//worth 25
void test_system(){
	//worth 5
	EXPECT_EQ(false, test_File(TEST_DATA_EMPTY), "28",FIVE_POINTS);

	//worth 10 (no sorting)
	EXPECT_EQ(true,test_File(TEST_DATA_FULL),"29",FIVE_POINTS);

	//worth 10 (correct output)
	if (does_file_exist(TEST_DATA_FULL_OUT))
		EXPECT_EQ(true,diff_files(TEST_DATA_FULL_OUT, TEST_DATA_FULL_PROCESSED),"30",FIVE_POINTS);
	else
		cout<<"ERROR 30 "<<TEST_DATA_FULL_OUT<< " does not exist"<<endl;
		
	//worth 10 (sorting)
	EXPECT_EQ(true,test_File(TEST_DATA_FULL, ASCENDING),"31",TEN_POINTS);
	
	if (does_file_exist(TEST_DATA_FULL_OUT))
		EXPECT_EQ(true,diff_files(TEST_DATA_FULL_OUT,TEST_DATA_FULL_PROCESSED_SORTED),"32",THIRTEEN_POINTS);
	else
		cout<<"ERROR 32 "<<TEST_DATA_FULL_OUT<< " does not exist"<<endl;
}

//============================================================================
//	main
//============================================================================
int main(int argc, char *argv[]) {
	//need this to make it equal 100
	total_points+=1;

	test_ProcessToken();
	test_ProcessLine();
	test_File_OpenClose();
	test_system();
	//remove_file_debris();

	string studentname = "Your";
	if (argc == 2) {
		//a command line param was passed in,
		//my script will pass in the student ID
		studentname = argv[1];
	}
	//what is your score
	cout<<studentname + " grade is "<<total_points<<endl;
	return SUCCESS;
}
