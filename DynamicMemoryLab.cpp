#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

struct SALESREC
{
	char     date[10];
	char     region[10];
	char     rep[10];
	char     item[10];
	int      units;
	float    unitCost;
	float    Total;
};


void simpleSortTotal(SALESREC salesArr[40], int c);

int main()
{
	ifstream infile;
	int i=0;
	char cNum[10] ;
	SALESREC *salesArr = new SALESREC[40];
	int   salesArrayCount = 0;
	//SALESREC* s[40];



		infile.open ("SalesData.txt", ifstream::in);
		if (infile.is_open())
		{
		int   c=0;
		while (infile.good())
		{
			infile.getline(salesArr[c].date, 256, ',');
			infile.getline(salesArr[c].region, 256, ',');
			infile.getline(salesArr[c].rep, 256, ',');
			infile.getline(salesArr[c].item, 256, ',');
			infile.getline(cNum, 256, ',');
			salesArr[c].units = atoi(cNum);
			infile.getline(cNum, 256, ',');
			salesArr[c].unitCost = atof(cNum);
			infile.getline(cNum, 256, '\n');
			salesArr[c].Total = atof(cNum);

			i++ ;
			c++;
		}
		salesArrayCount = c-1;
		infile.close();
	}
	else
	{
		cout << "Error opening file";
	}
	// Initialize the sort array pointers
	// for (int i=0; i < salesArrayCount; i++)
	// 	s[i] = _______________;
	
	simpleSortTotal (salesArr, salesArrayCount);
	  
	for (int i=0; i < salesArrayCount; i++)
		cout << "Record: " << salesArr[i].date <<", " << salesArr[i].region << ", " << salesArr[i].rep << ", " << salesArr[i].item << ", " << salesArr[i].units << ", " << salesArr[i].unitCost << ", " << salesArr[i].Total << endl;
	
	delete[] salesArr;
}
 
// arguments will be an array of pointers and a count
void simpleSortTotal( SALESREC salesArr[40] , int c)
{
	SALESREC placeHolder;
	for (int i=0; i < c; i++)
		for (int j=i; j < c; j++)
		if (salesArr[i].Total < salesArr[j].Total)
		{
			placeHolder = salesArr[i];
		  	salesArr[i] = salesArr[j];
		  	salesArr[j] = placeHolder;
		}
}