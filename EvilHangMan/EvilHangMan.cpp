// EvilHangMan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <fstream>
#include <random>
#include <cctype>

using namespace std;

unordered_map<int, string> readDictionary(int wordSize) {
	ifstream dict("dictionary.txt");
	unordered_map<int, string> indexToWord{};

	if (!(dict.is_open())) {
		cerr << "File cannot be opened!" << endl;
		return {};
	}

	string s;
	int j = 0;
	while (getline(dict, s)) {
		if (s.size() == wordSize) {
			indexToWord[j++] = s;
		}
	}

	return indexToWord;
}

string getRandomWord(unordered_map<int, string> &dict) {
	srand(time(NULL));
	int randNums = (rand() % dict.size());
	return dict.at(randNums);
}

void coutUnorderedMap(unordered_map<int, string> &map) {
	for (const auto& i : map) {
		cout << i.first << " " << i.second << endl;
	}
}

void showUnderline(string &chosenWord, unordered_set<char> &usedCharCorrect) {
	if (chosenWord == "") cout << "There's no chosen word";
	for (int i = 0; i < chosenWord.size(); i++){
		if (usedCharCorrect.find(chosenWord[i]) != usedCharCorrect.end()) {
			cout << chosenWord[i] << "  ";
			continue;
		}
		cout << "_ " << " ";
	}
	cout << endl;
}

char checkInput(set<char> usedChar) {
	string userInput{};

	cout << "User Letter: ";
	cin >> userInput;
	while (true) {
		if (userInput.size() > 1) {
			cout << "Please input a letter.\nUser Letter: ";
			cin.clear();
			cin.ignore(1000, '\n');
			cin >> userInput;
			continue;
		}

		if (!isalpha(userInput[0])) {
			cout << "Please input a letter.\nUser Letter: ";
			cin.ignore(1000, '\n');
			cin >> userInput;
			continue;
		}

		if (usedChar.find(userInput[0]) != usedChar.end()) {
			cout << "The letter " << userInput << " has already been chosen, please enter another\nUser Letter : ";
			cin.ignore(1000, '\n');
			cin >> userInput;
			continue;
		}
		break;
	}

	return userInput[0];
}

pair<bool, char> logic(unordered_map<int,string>& dict, char userInput) {
	unordered_map<int, string> newDict{};
	int j = 0;
	for (const auto& i : dict) {
		if (i.second.find(userInput) == string::npos) {
			newDict.insert({j++, i.second});
		}
	}

	if (newDict.empty()) {
		return { true, userInput };
	} 

	dict = newDict;
	return { false, userInput };
}

bool checkWinner(string& chosenWord, unordered_set<char>& usedCharCorrect) {
	int temp = 0;
	for (int i = 0; i < chosenWord.size(); i++) {
		if (usedCharCorrect.find(chosenWord[i]) != usedCharCorrect.end()) {
			temp++;
		}
	}
	if (temp == chosenWord.size()) {
		return true;
	} return false;
}

void mainMenu() {
	// word length adjust here
	int wordLength = 5; 

	unordered_map<int, string> dict = readDictionary(wordLength);
	set<char> usedChar{};
	unordered_set<char> usedCharCorrect{};
	string chosenWord{}; 
	char userInput{};
	int numOfGuess = 0;
	bool stillPlaying = true;

	while (stillPlaying) {
		chosenWord = getRandomWord(dict);
		
		cout << "Word left in dictionary: " << dict.size() << endl;
		showUnderline(chosenWord, usedCharCorrect);
		cout << "Character used: ";
		for (const auto& i : usedChar) { cout << i << " "; };
		cout << endl;

		cout << "Number of guesses: " << numOfGuess++ << endl;

		userInput = checkInput(usedChar);
		pair<bool, char> choiceInfo = logic(dict, userInput);

		if (choiceInfo.first) {
			usedChar.insert(choiceInfo.second);
			usedCharCorrect.insert(choiceInfo.second);
		}
		else {
			usedChar.insert(choiceInfo.second);
		}

		if (checkWinner(chosenWord, usedCharCorrect)) {
			system("cls");
			cout << "And the final word is..." << endl << chosenWord << endl;
			cout << "Congratulations! You win with " << numOfGuess << " guesses" << endl;
			break;
		}
		system("cls");
	}
	
}

int main()
{
	mainMenu();
}
