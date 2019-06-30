#include <fstream>
#include <iostream>

using namespace std;
using byte = char;

const char *path = "audio.wav";

struct Wav_RIFF_header
{
	char ChunkID[4];
	char ChunkSize[4];
	char Format[4];
} riff;

struct Wav_FMT
{
	char Subchunk1ID[4];
	char Subchunk1Size[4];
	char AudioFormat[2];
	char NumChannels[2];
	char SampleRate[4];
	char ByteRate[4];
	char BlockAlign[2];
	char BitsPerSample[2];
	char ExtraParamSize[2];
} fmt;

struct Wav_DATA_header
{
	char Subchunk2ID[4];
	char Subchunk2Size[4];
} data_header;

struct Wav_Audio
{
	char* data;
} audio;

int main()
{
	ifstream is (path, ios::binary);
	
	is.read((char*)&riff, sizeof riff);
	is.read((char*)&fmt, sizeof fmt);
	is.read((char*)&data_header, sizeof data_header);

	is.close();
	return 0;
}































