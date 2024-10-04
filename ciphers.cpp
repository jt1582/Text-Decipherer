#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

/**
 * Print instructions for using the program.
 */
void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

void decryptSubstCipherFromFile(const QuadgramScorer& scorer);

int main() {
  Random::seed(time(NULL));
  string command;

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    // Use getline for all user input to avoid needing to handle
    // input buffer issues relating to using both >> and getline
    getline(cin, command);
    cout << endl;

    vector<string> dictionary;
    ifstream file;
    file.open("dictionary.txt");
    string line;
    while (getline(file, line)) {
      dictionary.push_back(line);
    }

    string input;
    ifstream file2;
    vector<string> quadgrams;
    vector<int> quadgramCounts;
    file2.open("english_quadgrams.txt");
    while (getline(file2, input)) {
      stringstream ss(input);
      string quadgram;
      string quadgramCount;
      getline(ss, quadgram, ',');
      getline(ss, quadgramCount);
      int qCount = stoi(quadgramCount);
      quadgrams.push_back(quadgram);
      quadgramCounts.push_back(qCount);
    }
    const QuadgramScorer scorer(quadgrams, quadgramCounts);

    if (command == "C" || command == "c") {
      runCaesarEncrypt();
    } else if (command == "D" || command == "d") {
      runCaesarDecrypt(dictionary);
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFromFile(scorer);
    } else if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    }

    cout << endl;
  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  // TODO: student fill this in
  vector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  int cIndex = 0;
  int newCIndex = 0;
  char newC = 'B';

  for (int i = 0; i < alphabet.size(); i++) {
    if (alphabet[i] == c) {
      cIndex = i;
    }
  }
  newCIndex = cIndex + amount;

  while (newCIndex > 25 || newCIndex < 0) {
    if (newCIndex > 25) {
      newCIndex -= 26;
    } else if (newCIndex < 0) {
      newCIndex += 26;
    }
  }

  newC = alphabet[newCIndex];
  return newC;
}

string rot(const string& line, int amount) {
  // TODO: student fill this in
  vector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  string newString;

  for (int i = 0; i < line.length(); i++) {
    if (line[i] == ' ') {
      newString += " ";
      continue;
    }
    char c = toupper(line[i]);
    for (int j = 0; j < alphabet.size(); j++) {
      if (c == alphabet[j]) {
        int newCIndex = j + amount;
        while (newCIndex > 25) {
          newCIndex -= 26;
        }
        while (newCIndex < 0) {
          newCIndex += 26;
        }
        newString += alphabet[newCIndex];
        break;
      }
    }
  }
  return newString;
}

void runCaesarEncrypt() {
  // TODO: student fill this in
  string text;
  string input;
  int numRot;

  cout << "Enter the text to Caesar encrypt: " << endl;
  getline(cin, text);
  cout << "Enter the number of characters to rotate by: " << endl;
  getline(cin, input);
  numRot = stoi(input);
  cout << rot(text, numRot) << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student fill this in
  for (int i = 0; i < strings.size(); i++) {
    strings[i] = rot(strings[i], amount);
  }
}

string clean(const string& s) {
  // TODO: student fill this in
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      result += toupper(c);
    }
  }
  return result;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student fill this in
  vector<string> result;
  stringstream ss(s);
  string word;
  while (ss >> word) {
    clean(word);
    result.push_back(word);
  }

  return result;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student fill this in
  string result;
  for (int i = 0; i < words.size(); i++) {
    result += words[i];
    if (i < words.size() - 1) {
      result += " ";
    }
  }
  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  // TODO: student fill this in
  int count = 0;
  for (int i = 0; i < dict.size(); i++) {
    for (int j = 0; j < words.size(); j++) {
      if (words[j] == dict[i]) {
        count++;
      }
    }
  }
  return count;
}

void runCaesarDecrypt(const vector<string>& dict) {
  // TODO: student fill this in
  string text;
  cout << "Enter the text to Caesar decrypt: " << endl;
  getline(cin, text);
  vector<string> noSpacesText = splitBySpaces(text);
  vector<string> validResults;

  for (int i = 0; i < 26; i++) {
    // Rotate the text
    vector<string> rotatedText = noSpacesText;
    rot(rotatedText, i);
    if (numWordsIn(rotatedText, dict) > (rotatedText.size() / 2)) {
      string result = joinWithSpaces(rotatedText);
      validResults.push_back(result);
    }
  }
  if (!validResults.empty()) {
    for (const string& res : validResults) {
      cout << res << endl;
    }
  } else {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student fill this in
  vector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                           'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                           'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
  vector<int> cipherIndex;
  vector<char> nonAlpha;
  string result;

  for (char c : s) {
    if (isalpha(c)) {
      char upperChar = toupper(c);
      for (int i = 0; i < alphabet.size(); i++) {
        if (upperChar == alphabet[i]) {
          cipherIndex.push_back(i);
          break;
        }
      }
      nonAlpha.push_back('n');
    } else {
      cipherIndex.push_back(-1);
      nonAlpha.push_back(c);
    }
  }
  for (int i = 0; i < s.length(); i++) {
    if (cipherIndex[i] != -1) {
      result += cipher[cipherIndex[i]];
    } else {
      result += nonAlpha[i];
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  // TODO: student fill this in
  vector<char> randAlphabet = genRandomSubstCipher();
  string line;
  cout << "Enter the text to substitution-cipher encrypt: ";
  getline(cin, line);
  string result = applySubstCipher(randAlphabet, line);
  cout << result << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student fill this in
  string cleanString = clean(s);
  if (cleanString.length() < 4) {
    return 0;
  }
  vector<string> quadgrams;
  for (int i = 0; i <= cleanString.length() - 4; i++) {
    quadgrams.push_back(cleanString.substr(i, 4));
  }
  double result = 0;
  for (const string& quadgram : quadgrams) {
    result += scorer.getScore(quadgram);
  }

  return result;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student fill this in
  vector<string> quadgrams;
  string text;
  cout << "Enter a string to score: ";
  getline(cin, text);
  double result = scoreString(scorer, text);
  cout << "Englishness: " << result << endl;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student fill this in
  vector<char> bestKey;
  double bestScore = -99999.9;
  string text = clean(ciphertext);
  for (int i = 0; i < 20; i++) {
    vector<char> currentKey = genRandomSubstCipher();
    string currentText = applySubstCipher(currentKey, text);
    double currentScore = scoreString(scorer, currentText);
    int count = 0;
    while (count < 1500) {
      vector<char> newKey = currentKey;
      int letterIndex1 = 0;
      int letterIndex2 = 0;
      while (letterIndex1 == letterIndex2) {
        letterIndex1 = Random::randInt(25);
        letterIndex2 = Random::randInt(25);
      }
      swap(newKey[letterIndex1], newKey[letterIndex2]);
      string newText = applySubstCipher(newKey, text);
      double newScore = scoreString(scorer, newText);
      if (newScore > currentScore) {
        currentKey = newKey;
        currentScore = newScore;
        count = 0;
      } else {
        count++;
        swap(newKey[letterIndex1], newKey[letterIndex2]);
      }
    }
    if (currentScore > bestScore) {
      bestScore = currentScore;
      bestKey = currentKey;
    }
  }

  return bestKey;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  // TODO: student fill this in
  string text;
  cout << "Enter text to substitution-cipher decrypt: ";
  getline(cin, text);
  vector<char> key = decryptSubstCipher(scorer, text);

  cout << applySubstCipher(key, text) << endl;
}

void decryptSubstCipherFromFile(const QuadgramScorer& scorer) {
  cout << "Enter filename with text to substitution-cipher decrypt: ";
  string fileInput1;
  string fileInput2;
  getline(cin, fileInput1);
  cout << "Enter filename to write results to: ";
  getline(cin, fileInput2);
  stringstream ss;
  ifstream file1;
  ofstream file2;
  string line;
  file1.open(fileInput1);
  file2.open(fileInput2);

  while (getline(file1, line)) {
    ss << line << endl;
  }
  vector<char> key = decryptSubstCipher(scorer, ss.str());
  file2 << applySubstCipher(key, ss.str());
}

#pragma endregion SubstDec
