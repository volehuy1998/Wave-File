#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;
using byte = char;

const char *pathIn =  "audio.wav";
const char *pathOut = "audio2.wav";

struct Wav_RIFF_header
{
	char ChunkID[4];
	char ChunkSize[4];
	char Format[4];
	Wav_RIFF_header()
	{
		memset(ChunkID, 0, sizeof ChunkID);
		memset(ChunkSize, 0, sizeof ChunkSize);
		memset(Format, 0, sizeof Format);
	}
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
	Wav_FMT()
	{
		memset(Subchunk1ID, 0, sizeof Subchunk1ID);
		memset(Subchunk1Size, 0, sizeof Subchunk1Size);
		memset(AudioFormat, 0, sizeof AudioFormat);
		memset(NumChannels, 0, sizeof NumChannels);
		memset(SampleRate, 0, sizeof SampleRate);
		memset(ByteRate, 0, sizeof ByteRate);
		memset(BlockAlign, 0, sizeof BlockAlign);
		memset(BitsPerSample, 0, sizeof BitsPerSample);
		memset(ExtraParamSize, 0, sizeof ExtraParamSize);
	}
} fmt;

struct Wav_DATA_header
{
	char Subchunk2ID[4];
	char Subchunk2Size[4];
	Wav_DATA_header()
	{
		memset(Subchunk2ID, 0, sizeof Subchunk2ID);
		memset(Subchunk2Size, 0, sizeof Subchunk2Size);
	}
} data_header;

struct Wav_audio
{
	char* data;
} data;

int main()
{
	ifstream is (pathIn,  ios::binary);
	ofstream os (pathOut, ios::binary); 
	// Read header
	is.read((char*)&riff, sizeof riff);
	is.read((char*)&fmt, sizeof fmt);
	is.read((char*)&data_header, sizeof data_header);

	// Get length
	int len = *(int*)data_header.Subchunk2Size;

	// Alloc
	data.data = new char[len];

	// Read data
	is.read(data.data, len);
	
	// Write data
	os.write((char*)&riff, sizeof riff);
	os.write((char*)&fmt, sizeof fmt);
	os.write((char*)&data_header, sizeof data_header);
	os.write(data.data, len);

	// Fin
	delete[] data.data;
	is.close();
	os.close();
	return 0;
}































