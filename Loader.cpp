#include "Loader.h"

void Loader::LoadHeightMap(char* filename, vector<float>& hmArray, int width, int depth, float scale)
{
	vector<char> in(width * depth);
	ifstream inFile;
	inFile.open(filename, ios_base::binary);

	if (inFile.good())
	{
		inFile.read((char*)&in[0], (streamsize)in.size());
		inFile.close();
	}

	hmArray.resize(depth * width, 0);

	for (unsigned int i = 0; i < (width * depth); i++)
		hmArray[i] = (in[i] / 255.0f) * scale;
}