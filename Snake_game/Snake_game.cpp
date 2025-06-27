#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool isVowel(char c)
{
	char lowerC = tolower(c);
	return lowerC == 'a' || lowerC == 'e' || lowerC == 'i' || lowerC == 'o' || lowerC == 'u';
}

string reverseVowels(const string& s)
{
	vector<char> vowels;
	for (char c : s)
	{
		if (isVowel(c))
		{
			vowels.push_back(c);
		}
	}
	reverse(vowels.begin(), voewls.end());
	string result = s;
	int vowelIndex = 0;
	for (size_t i = 0; i < result.size(); i++)
	{
		if (isVowel(result[i]))
		{
			result[i] = vowels[vowelIndex++];
		}
	}
	return result;
}

int main()
{
	string s = "beautiful";
	string result = reverseVowels(s);
	cout << result << endl;
	return 0;
}