#include "my_aod_to_particle.hpp"

void Utility::RemoveCharFromTheString(string& str, char symbol)
{
    for (size_t i = 0; i < str.length(); )
    {
        if (str[i] == symbol)
        {
            str.erase(i, 1);
        }
        else
        {
            ++i;
        }
    }
}


static size_t Utility::CountOccurrences(const uint8_t* buffer, size_t& size, const string& word)
{
	std::string data(reinterpret_cast<const char*>(buffer), size);

	size_t count = 0;
	size_t pos = data.find(word, 0);

	while (pos != string::npos)
	{
		++count;
		pos = data.find(word, pos + word.length()); // Przesuwamy indeks za znalezione s³owo
	}

	return count;
}

queue<string> Utility::FindLineStrings(const char* buffer, size_t bufferSize, const string pattern)
{
	queue<string> results;
	const size_t patternLength = pattern.length();

	const char* currentPos = buffer;
	const char* endPos = buffer + bufferSize;

	while (currentPos < endPos)
	{
		// ZnajdŸ wyst¹pienie "texture "
		const char* found = search(currentPos, endPos, pattern.begin(), pattern.end());

		if (found == endPos)
		{
			break; // Nie znaleziono wiêcej wyst¹pieñ
		}

		// Przesuñ wskaŸnik na pocz¹tek tekstu po "texture "
		const char* textStart = found + patternLength;

		// ZnajdŸ koniec linii (znak nowej linii lub koniec bufora)
		const char* lineEnd = find(textStart, endPos, '\n');

		// Zapisz wynik (od tekstu po "texture " do koñca linii)
		results.emplace(textStart, lineEnd);

		// Przesuñ wskaŸnik na koniec bie¿¹cej linii do przeszukania
		currentPos = lineEnd;
	}

	return results;
}

void Utility::InitializeHashMapViaLabelsVector(unordered_map<string, int64_t>& labels_map, vector<string>& labels)
{
	int64_t i = 0;
	for (const auto& str : labels)
	{
		labels_map[str] = i;
		++i;
	}
}

void Utility::ReplaceStringInFileBuffer(string& file_buffer, const string search_str, const string replace_str)
{
	size_t pos = file_buffer.find(search_str);

	while (pos != string::npos)
	{
		file_buffer.replace(pos, search_str.length(), replace_str);
		pos = file_buffer.find(search_str, pos + replace_str.length());
	}
}