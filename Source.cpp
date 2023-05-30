#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <cctype>

bool CheckWordExists(const std::string& word, const std::vector<std::string>& AllWords) {
	for (const std::string& w : AllWords) {
		if (w == word) {
			return true;
		}
	}
	return false;
}


std::vector<std::string> GetIntersection(const std::vector<std::string>& vec1, const std::vector<std::string>& vec2) {
	std::vector<std::string> intersection;

	for (const std::string& s : vec1) {
		if (CheckWordExists(s, vec2)) {
			intersection.push_back(s);
		}
	}
	return intersection;
}


std::vector<int> fill_buckets(const std::string& word) {
	std::vector<int> bucket(26, 0);

	for (const char& c : word) {
		bucket[c - 'a']++;
	}
	return bucket;
}

int ScoreWords(const std::string& word1, const std::string& word2) {
	int score = 0;

	const auto bucket1 = fill_buckets(word1);
	const auto bucket2 = fill_buckets(word2);

	for (int i = 0; i < 26; i++) {
		score += std::min(bucket1[i], bucket2[i]);
	}

	for (int i = 0; i < word1.size(); i++) {
		if (word1[i] == word2[i]) {
			score++;
		}	
	}
	return score;
}

int main() {

	std::vector<std::string> five_words;

	{
		std::ifstream five_word_file("sgb-words.txt");

		for (std::string line; std::getline(five_word_file, line);) {
			if (line.empty()) {
				continue;
			}
			five_words.push_back(line);
		}
	}

	std::vector<std::string> freq_words;

	{
		std::ifstream freq_file("commonwords.txt");

		for (std::string line; std::getline(freq_file, line);) {
			if (line.empty()) {
				continue;
			}
			freq_words.push_back(line);
		}
	}

	
	freq_words.resize(2000);


	auto filtered_words = GetIntersection(five_words, freq_words);





	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, filtered_words.size() - 1);
	const std::string target = filtered_words[dist(rng)];

	while (true) {
		std::cout << "Guess the five letter word : ";
		std::string guess;
		std::getline(std::cin, guess);
	
		for (char& c : guess) {
			c = tolower(c);
		}


		if (guess.size() != 5) {
			std::cout << "Enter a five letter word only." << std::endl;
			continue;
		}
		if (!CheckWordExists(guess, five_words)) {
			std::cout << guess << "?, The word you entered dosen't exist." << std::endl;
			continue;
		}

		const int score = ScoreWords(guess, target);
		if (score == 10) {
			std::cout << "Correct." << std::endl;
			break;
		}
		else {
			std::cout << "Wrong. Here is your score : " << score << std::endl;
			continue;
		}
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cin.get();

	return 0;
}