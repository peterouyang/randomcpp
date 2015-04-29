//
//  scoreparser.cpp
//  
//
//  Created by Peter Ouyang on 1/22/15.
//
// call with syntax: ./a.out filename out_file n_measures

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "xmlParser/xmlParser.h"

int note_to_int(char ch);
void shift_array(double *arr1, double *arr2, int n_elts, int shift);


int main(int argc, char * argv[])
{
	XMLNode xMainNode = XMLNode::openFileHelper(argv[1], "score-partwise");
	std::string filename = (std::string) argv[2];
	std::ofstream fout(filename);
	int n_measures = std::stoi(argv[3]);
	// note_count records relative frequency of each pitch bar-by-bar
	// notes related by octaves considered equivalent
	// C natural is 0
	// each half step increments by 1
	// quarter note has weight 1
	double note_count[n_measures][12];
	for(int i=0; i< 12; i++){
		for(int j = 0; j < n_measures;j++)
		{
			note_count[j][i]=0;
		}
	}
	
	//count bars starting from 0
	int num_notes;
	
	int num_parts = xMainNode.nChildNode("part");
	int k =0;
	// if score is written with multiple parts, loop over parts
	while(k < num_parts){
		XMLNode partNode = xMainNode.getChildNode("part", k);

		for(int current_meas = 0; current_meas < n_measures; current_meas++)
		{
			XMLNode mNode = partNode.getChildNode("measure",current_meas);
			XMLNode attNode = mNode.getChildNode("attributes");
			XMLNode dNode;
			if(!attNode.isEmpty())
				dNode = attNode.getChildNode("divisions");
			// divisions counts number of sub-beats in each beat
			double divisions;
			if(!dNode.isEmpty())
			{
				// is there a better way to read from xmlParser
				// than reading strings first and converting to numbers?
				std::string st = (std::string) dNode.getText();
				// note we use double because we want to do division later
				divisions = std::stof(st);
			}
			num_notes = mNode.nChildNode("note");
			
			
			//loop over notes
			for(int i = 0; i < num_notes; i++)
			{
				XMLNode nNode = mNode.getChildNode("note",i);
				XMLNode pitchNode = nNode.getChildNode("pitch");
				// ignores grace notes (for now)
				if(!pitchNode.isEmpty() && nNode.getChildNode("grace").isEmpty())
				{
					// XML records notes by letter
					char ch = *(pitchNode.getChildNode("step").getText());
					// convert letter to number
					int note_val = note_to_int(ch);
					// take accidentals into account
					XMLNode aNode = pitchNode.getChildNode("alter");
					if(!aNode.isEmpty()){
						std::string alt_str = (std::string) aNode.getText();
						note_val += std::stoi(alt_str);
						note_val %= 12;
					}
					XMLNode durNode = nNode.getChildNode("duration");
					std::string dur_str = (std::string) durNode.getText();
					double duration = std::stof(dur_str)/divisions;
					note_count[current_meas][note_val] += duration;
				}
			}
		}
		k++;
	}
	
	for(int j = 0;j < n_measures; j++)
	{
		for(int i = 0; i < 12; i++){
			if(note_count[j][i] < 0.0001)
				note_count[j][i] = 0;
			fout <<note_count[j][i] << " \t";
		}
		fout << std::endl;
	}
	fout.close();

	return 0;
}

//
// function definitions follow
//

int note_to_int(char ch)
{
	int ret=0;
	ch = toupper(ch);
	switch(ch){
		case 'C':
			ret = 0;
			break;
		case 'D':
			ret = 2;
			break;
		case 'E':
			ret = 4;
			break;
		case 'F':
			ret = 5;
			break;
		case 'G':
			ret = 7;
			break;
		case 'A':
			ret = 9;
			break;
		case 'B':
			ret = 11;
			break;
		default:
			std::cout << "Error -- non-note value recorded as note\n";
	}
	return ret;
}

void shift_array(double *arr1, double *arr2, int n, int shift)
{
	for(int i = 0; i < n; i++)
	{
		arr2[(i+shift) % n] = arr1[i];
	}
}