
#include "pch.h"
#pragma once
#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <list> 
#include <utility>
#include <algorithm>

#define MAX 10

using namespace std;
class Automata {
private:
	string line;
	string states[MAX];
	string vocabulary[MAX];
	map<string, string> * table = new map<string, string>();
	string final_states[MAX];
	string start_state;

	map <string, string> deleteFromMap(string first, string second, map <string, string> m);
	void minimize_write_file(list <pair <string, string>> same, string file_name);
	void deleteFromList(string first, string second, list <pair<string, string>> m);
public:
	Automata(string file_name, int number);
	string *getVocab();
	string *getStates();
	string *getFinals();
	void setStart(string S) { start_state = S; }
	string getStart() { return start_state; }
	Automata(string file_name);
	void setStates(string *States, string line);
	void setVocab(string *V, string line);
	void setTable(map<string, string> &m, string line);
	void setFinalStates(string *final_states, string line);
	map <string, string> getTable();
	bool isEmpty(string start, map <string, string> &visited);
	bool hasElement(string start, string w);
	bool Minimize(string file_name);
	bool isFinite(string start, int control);
	bool isDistinguishable(string s1, string s2);
	bool isFinal(string s);
	bool isState(string start);
	list <string> getSinkStates(map<string, string> m1);
	bool checkLoop(string start, string Finish, list<string> visited, list <string> sink);
	bool operator ==(Automata B);
	bool compareMaps(Automata C, map<string, string>, string, string);
};





Automata::Automata(string file_name,int number) {
	ifstream f(file_name, ios::in);
	int i = 0;
	if (f)
	{
		if (number == 2)
		{
			while (i < 4)
			{
				getline(f, line);
				i++;
			}
			i = 0;
		}
			while (getline(f, line)&& i<4) {
			if (i == 0) setStates(states, line);
			if (i == 1) setVocab(vocabulary, line);
			if (i == 2) setTable(*table, line);
			if (i == 3)  setFinalStates(final_states, line);
			i++;
		}

		
	}
	else
	{
		//cout << "No file found ! ";
		exit(0);
	}
}

Automata::Automata(string file_name) {
	ifstream f(file_name, ios::in);
	int i = 0;
	if (f)
	{
		while (getline(f, line)) {
			if (i == 0) setStates(states, line);
			if (i == 1) setVocab(vocabulary, line);
			if (i == 2) setTable(*table, line);
			if (i == 3)  setFinalStates(final_states, line);
			i++;
		}

		if (i != 4)
		{
			//cout << "Your text file need to has at least 4 rows";
			exit(0);
		}
	}
	else
	{
		//cout << "No file found ! ";
		exit(0);
	}
}

void Automata::setStates(string *States, string line) {
	//cout << line << endl;
	int k = 0;
	string s1;

	//taking states
	for (int i = 0; i < line.length(); i = i + 1)
	{
		s1.assign(line, i, 1);
		if (s1 == ",") k++;
		if (s1 == "{" || s1 == "}" || s1 == ",") continue;
		States[k].append(line, i, 1);

	}

}
void Automata::setVocab(string *V, string line) {
	int k;
	//cout << line << endl;
	k = 0;
	for (int i = 1; i < line.length(); i = i + 2)
	{
		V[k].assign(line, i, 1);
		k++;
	}

}
void Automata::setTable(map<string, string> &m, string line) {
	//cout << line << endl;
	string s1, s2, s3, s4;
	for (int i = 0; i < line.length(); i++)
	{
		s1.assign(line, i, 1);
		if (s1 == "{" || s1 == "}") continue;

		if (s1 == "<")
		{
			s1.assign(line, ++i, 1);
			while (s1 != ",")
			{
				s2.append(s1);
				s1.assign(line, ++i, 1);
			}
			s1.assign(line, ++i, 1);
			s2.append(",");
			s2.append(s1);

			s1.assign(line, ++i, 1); //>
			s1.assign(line, ++i, 1);//,
			s1.assign(line, ++i, 1);
			while (s1 != ")")
			{
				s3.append(s1);
				s1.assign(line, ++i, 1);
			}
			m.insert(pair<string, string>(s2, s3));
			s2 = s3 = "";
		}
	}


}
void Automata::setFinalStates(string *final_states, string line) {
	//cout << line << endl;
	int k = 0;
	string s1;
	for (int i = 0; i < line.length(); i = i + 1)
	{
		s1.assign(line, i, 1);
		if (s1 == ",") k++;
		if (s1 == "{" || s1 == "}" || s1 == ",") continue;
		final_states[k].append(line, i, 1);

	}

}
bool Automata::isEmpty(string start, map <string, string> &visited) {
	//	map <string, int> visited;
	string temp, temp2;
	visited.insert(pair<string, string>(start, "1"));
	//start = states[0];
	for (int i = 0; i < MAX; i++)
	{
		if (start == final_states[i])
			return false;
	}
	temp = temp2 = start;
	temp = temp.append(",0");
	temp2 = temp2.append(",1");
	map<string, string> m;
	m = getTable();
	temp = m[temp];
	temp2 = m[temp2];
	bool a = 1, b = 1;

	if (visited.find(temp) == visited.end())
		a = isEmpty(temp, visited);

	if (visited.find(temp2) == visited.end())
		b = isEmpty(temp2, visited);

	if ((!a) || (!b))
		return false;
}
map <string, string> Automata::getTable() {
	return *table;
}

bool Automata::hasElement(string start, string w) {
	int length = w.length();
	string ch;
	map<string, string> m;
	m = getTable();
	string Input, Output;
	Input = start;
	for (int i = 0; i < length; i++)
	{
		ch = w.at(i);
		Input = Input.append(",");
		Input = Input.append(ch);
		Output = m[Input];
		Input = Output;
	}

	return isFinal(Output);

}

bool Automata::Minimize(string file_name) {
	list <pair <string, string>> same; //since max =10, max combination is (2 of 10) which equals COMB=45 
	string first_part, second_part;
	bool b;


	for (int j = 0; j < MAX; j++) {

		if (states[j] == "") break;
		first_part = states[j];
		for (int i = 0; i < MAX; i++)
		{
			if (states[i] == "") break;
			if (i == j) continue;
			second_part = states[i];
			pair <string, string > q = make_pair(second_part, first_part);
			if (!same.empty()) {
				list<pair <string, string>>::iterator findIter = find(same.begin(), same.end(), q);
				/*	list<pair <string, string>>::iterator findIter;
					for(findIter=same.begin(); findIter != same.end(); findIter++)
					{
						if ((*findIter).first == q.first && (*findIter).second == q.second)
							break;
					}*/


				if (findIter == same.end())
				{
					b = isDistinguishable(first_part, second_part);
					if (!b) //if not distinguishable, then they can be represted as same
					{
						pair <string, string > p = make_pair(first_part, second_part);
						same.push_front(p);
					}
				}
			}
			else
			{
				b = isDistinguishable(first_part, second_part);
				if (!b) //if not distinguishable, then they can be represted as same
				{
					pair <string, string > p = make_pair(first_part, second_part);
					same.push_front(p);
				}
			}

		}

	}
	if (same.empty()) {
		cout << "FALSE" << endl;//no minimization can be done
		return false;
	}
	minimize_write_file(same, file_name);
	return true;
}
//recursive kullan
bool Automata::isDistinguishable(string s1, string s2)
{
	string temp, temp2;
	string out_s1_1, out_s1_2;
	string out_s2_1, out_s2_2;
	map<string, string> m;
	bool final_1, final_2;
	m = getTable();



	final_1 = isFinal(s1);
	final_2 = isFinal(s2);
	if (final_1 != final_2)
		return true; //if one is final and the other is not, return true(distinguisable) without checking anything

	temp.assign(s1);
	temp2.assign(s1);
	temp = temp.append(",0");
	temp2 = temp2.append(",1");
	out_s1_1 = m[temp];
	out_s1_2 = m[temp2];


	temp.assign(s2);
	temp2.assign(s2);
	temp = temp.append(",0");
	temp2 = temp2.append(",1");
	out_s2_1 = m[temp];
	out_s2_2 = m[temp2];

	if ((out_s1_1 == out_s2_1) || (isFinal(out_s1_1) && isFinal(out_s2_1)))
	{
		if (out_s2_2 == out_s1_2 || (isFinal(out_s2_2) && isFinal(out_s1_2)))
			return false;//Eğer bütün değerler aynıysa birbirinden ayırt edemeyiz. 
	}
	else if ((isFinal(out_s1_1) != isFinal(out_s2_1)) || (isFinal(out_s2_2) != isFinal(out_s1_2)))
		return true;
	else if ((s1 == out_s1_1 && s2 == out_s2_1) || (s1 == out_s2_1 && s2 == out_s1_1)) {
		if ((s1 == out_s1_2 && s1 == out_s1_1) && (s2 == out_s2_2 && s2 == out_s2_1))
			return false; // both sink states
		//else if ((s1 == out_s1_2 && s1 == out_s1_1) || (s2 == out_s2_2 && s2 == out_s2_1))
		//	return true; // one sink, the other is not 
		else return isDistinguishable(out_s1_2, out_s2_2); //or both are not
	
	}
	else if ((s1 == out_s1_2 && s2 == out_s2_2) || (s1 == out_s2_2 && s2 == out_s1_2))
		return isDistinguishable(out_s1_1, out_s2_1);
	else
		return isDistinguishable(out_s1_2, out_s2_2) || isDistinguishable(out_s1_1, out_s2_1);
}

bool Automata::isFinal(string Output) {
	for (int i = 0; i < MAX; i++)
	{
		if (final_states[i] == "") break;
		if (Output == final_states[i])
			return true;
	}
	return false;

}

map <string, string> Automata::deleteFromMap(string first, string second, map <string, string> m) {
	//Assuming first = second for minimization. So we need to change second to first in the map
		//map <string, string> m = getTable();
	map <string, string> new_map;
	map<string, string>::iterator itr;
	string new_str;
	for (itr = m.begin(); itr != m.end(); ++itr) {
		new_str = (itr->first);
		size_t found = (new_str).find(second);
		if (found != string::npos)
			continue;//eğer ilk kısımda silinicek öğe varsa hiç bir şey yapma
		//Eğer bulunmamışsa 2.kısımı control et 
		if ((itr->second) == second)
			new_map[itr->first] = first;
		else
			new_map[itr->first] = itr->second; //eğer ilk ve 2. kısım seconddan bağımsızsa, entry i değiştirmeden yeni map'a ekle.

	}

	return new_map;










}
void Automata::deleteFromList(string first, string second, list <pair<string, string>> l) {
	// Assuming first = second for minimization.So we need to change any other to first in the list
		//map <string, string> m = getTable();
	list <pair<string, string>> new_list;
	list<pair<string, string>>::iterator itr;

	itr = l.begin();
	//delete second part of it from list.
	for (; itr != l.end(); ++itr) {
		if (second == (*itr).first)
			(*itr).first = first;
		if (second == (*itr).second)
		{
			(*itr).second = first;
		}

	}
}
void Automata::minimize_write_file(list <pair <string, string>> same, string file_name) {
	//new states is specified for the file 
	list<string>::iterator findIter;
	list<pair <string, string>>::iterator it,it2;
	list<string> new_states;
	list<string> new_finals;
	list <string> deleted;
	string temp;
	map <string, string> new_map;
	map <string, string> m = getTable();
	string second_part, first_part;
	//coppying the states and final states
	new_map = m;
	for (int i = 0; i < MAX; i++)
	{
		if (states[i] != "")
			new_states.push_front(states[i]);
		if (final_states[i] != "")
			new_finals.push_front(final_states[i]);

	}

	for (it = same.begin(); it != same.end(); ++it) {
		findIter = find(new_states.begin(), new_states.end(), (*it).second);
		second_part = (*it).second;
		first_part = (*it).first;
		//deleteFromList(first_part,second_part,same);
		//deleted.push_back(second_part);
		if (find(deleted.begin(), deleted.end(), first_part) != deleted.end())
		{
			for (it2 = same.begin(); it2 != it; ++it2) {
				//eğer silinen element başka bir elemente eşitse, eski değerini bul
				if ((*it2).second == first_part)
					first_part = (*it2).first;
			}

		}
		if (find(deleted.begin(), deleted.end(), second_part) == deleted.end())
		{
			deleted.push_back(second_part);
			new_states.erase(findIter);//Deleting same states
			if (isFinal(second_part)) //if its in final states,delete from there also
			{
				findIter = find(new_finals.begin(), new_finals.end(), second_part);
				new_finals.erase(findIter);
			}

			new_map = deleteFromMap(first_part, second_part, new_map); //delete it from the table also
		}
	}
		
	//open a file and write them
	ofstream myfile;
	list<string >::iterator itr;
	//Minimizmization done.Saving it to the file : minimaldfa.txt
	//cout << "TRUE" << endl; 
	myfile.open(file_name);
	myfile << "{";
	itr = new_states.begin();
	while (itr != new_states.end()) {
		myfile << *(itr);
		itr++;
		if (itr != new_states.end())
			myfile << ",";
	}
	myfile << "}";
	myfile << endl;

	//vocabulary didnt change sooo
	myfile << "{";
	for (int i = 0; i < MAX; i++)
	{

		if (vocabulary[i] != "")
		{
			if (i != 0) myfile << ",";
			myfile << vocabulary[i];
		}
	}

	myfile << "}";
	myfile << endl;







	map<string, string>::iterator map_itr, map_end;
	map_itr = new_map.begin();
	myfile << "{";
	while (map_itr != new_map.end()) {
		myfile << "(" << "<" << (*map_itr).first << ">," << (*map_itr).second << ")";
		map_itr++;
		if (map_itr != new_map.end())
			myfile << ",";
	}
	myfile << "}";
	myfile << endl;

	//4th row

	myfile << "{";
	itr = new_finals.begin();
	while (itr != new_finals.end()) {
		myfile << *(itr);
		itr++;
		if (itr != new_finals.end())
			myfile << ",";
	}
	myfile << "}";
	myfile << endl;


	myfile.close();

	

}
bool Automata::isState(string start) {

	for (int i = 0; i < MAX; i++)
	{
		if (states[i] == "")
			break;
		if (start == states[i])
			return true;

	}
	return false;

}

bool Automata::isFinite(string start, int control) {
	ifstream myFile("minimaldfa.txt");
	if (!myFile.fail()) {
		if (control == 0)
		{
			Automata B("minimaldfa.txt");
			return B.isFinite(start, 1);
		}
	}

	int k = 0;
	list<string> visited;
	list <string> sink;
	sink = getSinkStates(getTable());
	//check if there is a loop 
	for (int i = 0; i < MAX; i++)
	{ //sink olmayan bütün stateler için loop controlü 
		if (states[i] == "") break;
		visited.clear();
		if (checkLoop(states[i], states[i], visited, sink))
		{
			//cout << "There is a loop in the automata!" << endl;
			return false; //loop infine
		}

	}
	return true; //no loop = finite
}
list<string> Automata::getSinkStates(map<string, string> m1) {

	list <string> Sink;
	string goesZero, goesOne, One, Zero;
	for (int i = 0; i < MAX; i++)
	{
		if (states[i] == "")
			break;
		if ((isFinal(states[i])))
			continue;

		goesOne = states[i];
		goesZero = states[i];
		goesOne.append(",");
		goesOne.append("1");
		goesZero.append(",");
		goesZero.append("0");

		One = m1[goesOne];
		Zero = m1[goesZero];

		if (One == states[i] && Zero == states[i])
		{
			Sink.push_front(states[i]);

		}
	}
	return Sink;
}
bool Automata::checkLoop(string start, string Finish, list<string> visited, list <string> sink) {

	string temp, temp2;
	visited.push_front(start);

	if ((std::find(sink.begin(), sink.end(), start) != sink.end()))
		return false;//dont count sink states as loop
	map<string, string> m;
	//visited.insert(pair<string, string>(start, "1"));
	//start = states[0];
	temp = temp2 = start;
	temp = temp.append(",0");
	temp2 = temp2.append(",1");
	m = getTable();
	temp = m[temp];
	temp2 = m[temp2];
	bool a = 1, b = 1;

	if (temp == Finish || temp2 == Finish)
		return true; //so there is a loop again


	if ((std::find(visited.begin(), visited.end(), temp) != visited.end()))
		return true;//if it visites a node, it came before then there is a loop
	else
		b = checkLoop(temp, Finish, visited, sink);
	if ((std::find(visited.begin(), visited.end(), temp2) != visited.end()))
		return true;//if it visites a node, it came before then there is a loop
	else
		a = checkLoop(temp2, Finish, visited, sink);
	if ((a) || (b))
		return true;
	else
		return false;

}
string *Automata::getStates() {
	return states;
}
string *Automata::getVocab()
{
	return vocabulary;
}
string *Automata::getFinals()
{
	return final_states;
}
bool Automata::operator ==(Automata C) {

	//C.setStart(B.start_state);
	string *A_states = getStates();
	int counter_A = 0; //counts states of minimized version of A
	string *C_states = C.getStates();
	int counter_C = 0;//counts states of minimized version of B
	for (int i = 0; i < MAX; i++)
	{
		if (A_states[i] != "")  counter_A++;
		if (C_states[i] != "") counter_C++;
	}

	if (counter_A != counter_C)
		return false; //if counts are not equal then they are not equal



	//compare vocabularies
	string *A_vocab = getVocab();
	string *C_vocab = C.getVocab();
	int j;
	for (int i = 0; i < MAX; i++)
	{
		if (A_vocab[i] == "") break;

		for (j = 0; j < MAX; j++)
		{
			if (A_vocab[i] == C_vocab[j]) break;

		}

		if (j == MAX) return false; //if an element of A's vocab doesnt exist in B's vocab return false
	}


	//compare final state counts.
	string *A_finals = getFinals();
	string *B_finals = C.getFinals();
	counter_A = 0;
	counter_C = 0;
	for (int i = 0; i < MAX; i++)
	{
		if (A_finals[i] != "") counter_A++;
		if (B_finals[i] != "") counter_C++;
	}

	if (counter_A != counter_C) return false;





	//At this point we know that we got same #of nodes and final states and has same vocabulary.
	//so that we can compare their connections.
	//we know that 2 of the dfa's start point has to be same so we go with this assumption.
	map<string, string> MatchUp;
	string Start_A = getStart();
	string Start_C = C.getStart();
	MatchUp.insert(pair<string, string>(Start_A, Start_C));

	return compareMaps(C, MatchUp, Start_A, ",0") && compareMaps(C, MatchUp, Start_A, ",1");

}

bool Automata::compareMaps(Automata C, map<string, string> MatchUp, string stringA, string number) {
	std::map<string, string>::iterator it;
	string temp, temp2;
	map<string, string> map_A;
	map<string, string> map_C;
	map_A = getTable();
	map_C = C.getTable();
	string stringC = MatchUp[stringA];
	temp = stringA;
	temp2 = stringC;

	temp = temp.append(number); //,0 yada ,1
	temp2 = temp2.append(number);
	temp = map_A[temp];
	temp2 = map_C[temp2];
	if (isFinal(temp) == C.isFinal(temp2)) {//eğer temp ve temp2, 2 si birden final state ise yada 2 si birden final değil ise
		if (MatchUp.find(temp) != MatchUp.end())
		{//Eğer temp match up ın içindeyse

			if (MatchUp[temp] != temp2) //temp ile temp2 eşit değilse 
				return false;//aynı automata değillerdir.

		}
		else
		{
			MatchUp.insert(pair <string, string>(temp, temp2)); //Eğer match upta yoksalar matchup a ekle.
			return compareMaps(C, MatchUp, temp, ",0") && compareMaps(C, MatchUp, temp, ",1");
		}
	}
	else//eğer yanlızca biri final ise  aynı automata değillerdir.
		return false;

	return true;


}

bool fexists(const char *filename)
{
	ifstream ifile(filename);
	return ifile.good();
}

int main()
{

	//cout << "Welcome to my program! " << endl;
	//cout << "This program consist of part a, b,c,d and e of the takehome " << endl;
	//cout << "At first program designed that the user specify every detail!" << endl;
	//cout << "If you wanna specify your own start node , please use the code in comment.If you dont, program spcify  " << endl;
	//cout << "start node as  'q' as default.I give this node as default to make program more usable. " << endl;
	//cout << "If you wanna try your own string you can use comments codes again.W is specified in the program to make it more usable" << endl;
	//cout << "program compares automatically same minimilized dfa as default" << endl;
	//cout << "but its working if you create other file which equals to the dfa or not" << endl;
	//cout << "you can find codes to try in comments." << endl;
	//cout << endl;


	Automata A("finiteautomata.txt");
	cout << endl;
	string start;
	start = "q";
	int pick = 0;
	string w;
	//OPEN THİS COMMENTS IF YOU WANNA SPECİFY YOUR OWN START NODE
	//cout << "Please enter the start node : " << endl;
	//cin >> start;
	//cin.ignore();
	//if (!(A.isState(start)))
	//{
	//	cout << "Wrong entry pls try again " << endl;
	//	exit(0);
	//}
	cin >> pick;
	A.setStart(start);

	map <string, string> *m = new map<string, string>();
	//this map shows path with its key, values are all 1 means its visited.Its used in the function
	//isEmpty.Its not for users, but its needed(an empty one )from isEmpty to work 
	//if i ll have time after finishing this midterm, i ll change it to another data structure(since its useless)
if(pick==1)
	//cout << "Part A : Is the language empty?" << endl;
	if (A.isEmpty(start, *m))
		cout << "TRUE" << endl; //language is empty
	else
	{
		
		cout << "FALSE" << endl; //language is not empty


	}
else if (pick == 2)
{
	w = "001";

	//OPEN THESE COMMENTS IF YOU WANNA TAKE w FROM USER
	//cout << "Please enter an string: " << endl;
	//getline(cin, w);

	//cout << "Part B: Is " << w << " in language ?" << endl;
	if (A.hasElement(start, w))
		cout << "TRUE"  << endl; //Automata recognizes this string
	else
		cout << "FALSE"  << endl; //Automata doesnt recognize this string

}
else if (pick == 3) {
	
	//cout << "Part C : Minimimization starting.." << endl;
	if (A.Minimize("minimaldfa.txt"))
		cout << "TRUE";
	else
		cout << "FALSE";
	cout << endl;
}
else if (pick == 4) {
	//cout << "Part D : Is L infinite ? " << endl;
	//cout << "Automata (minimized version if its available)" << endl;
	if (A.isFinite(start, 0))
		cout << "TRUE" << endl; //Automata is finite !
	else
		cout << "FALSE " << endl; //Automata is infinite ! 
}
else if (pick == 5) {
	cout << endl;
	cout << "Part E :  Does to minimal dfa equal to eachother ? " << endl;
	//OPEN THIS COMMENT İF YOU WANNA TRY ON WİTH DİFFERENT AUTOMATAS. 
	if (fexists("finiteautomata2.txt"))
	{
		Automata B("finiteautomata2.txt",1);
	    B.setStart("q");
		B.Minimize("minimaldfa.txt");
		Automata MinimalB("minimaldfa.txt");
		cout << endl;
		Automata C("finiteautomata2.txt",2); //Change this line for different automatas
		C.setStart("q");
		C.Minimize("minimaldfa.txt");
		Automata MinimalC("minimaldfa.txt");
	//	//both dfa has to be minimal before sending == operation.
	//	//here the same dfa is used to show equalivance to show it.
		if ( MinimalC== MinimalB)
			cout << "TRUE" << endl; //EQUAL
			else
				cout << "FALSE " << endl; //NOT EQUAL
	}


	//if (fexists("minimaldfa.txt"))
	//{
	//	Automata C("minimaldfa.txt");
	//	C.setStart(start);
	//	//both dfa has to be minimal before sending == operation.
	//	//here the same dfa is used to show equalivance to show it.
	//	if (C == C)
	//		cout << "Both automata's are equal" << endl;
	//	else
	//		cout << "They are not equal " << endl;
	//}



}

	return (0);
}
