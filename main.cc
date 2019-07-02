#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;
using byte = char;

const string pathIn =  "origin.wav";
const string pathOut = "noise.wav";

struct Wav_RIFF_header
{
	byte ChunkID[4];
	byte ChunkSize[4];
	byte Format[4];
	Wav_RIFF_header()
	{
		memset(ChunkID, 0, sizeof ChunkID);
		memset(ChunkSize, 0, sizeof ChunkSize);
		memset(Format, 0, sizeof Format);
	}
} riff;

struct Wav_FMT
{
	byte Subchunk1ID[4];
	byte Subchunk1Size[4];
	byte AudioFormat[2];
	byte NumChannels[2];
	byte SampleRate[4];
	byte ByteRate[4];
	byte BlockAlign[2];
	byte BitsPerSample[2];
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
	}
} fmt;

struct Wav_DATA_header
{
	byte Subchunk2ID[4];
	byte Subchunk2Size[4];
	Wav_DATA_header()
	{
		memset(Subchunk2ID, 0, sizeof Subchunk2ID);
		memset(Subchunk2Size, 0, sizeof Subchunk2Size);
	}
} data_header;

struct Wav_audio
{
	byte* data;
	int len;
	Wav_audio() : data(nullptr), len(0) {}
	~Wav_audio() 
	{
		if (data) delete[] data;
		len = 0;
	}
} data;

struct FILE_Audio
{
	ifstream is;
	FILE_Audio(const string& pathIn) : is(pathIn.c_str(), ios::binary) {}
	void read()
	{
		// Read header
		is.read((byte*)&riff, sizeof riff);
		is.read((byte*)&fmt, sizeof fmt);

		int PCM = *(short*)fmt.AudioFormat;

		if (PCM == 1)
		{
#define PCM
			is.seekg(2, is.cur);
		}

		is.read((byte*)&data_header, sizeof data_header);
		for (int i = 0; i < 4; i++)
			cout << data_header.Subchunk2ID[i];

		// Get length
		data.len = *(int*)data_header.Subchunk2Size;

		// Alloc
		data.data = new byte[data.len];

		// Read data
		is.read(data.data, data.len);
		is.close();
	}
	void write(const string& pathOut)
	{
		ofstream os(pathOut.c_str(), ios::binary);
		// Write data
		os.write((byte*)&riff, sizeof riff);
		os.write((byte*)&fmt, sizeof fmt);
#ifdef PCM
		byte ExtraParamSize[2];
		os.write(ExtraParamSize, sizeof ExtraParamSize);
#endif
		os.write((byte*)&data_header, sizeof data_header);
		os.write(data.data, data.len);
		os.close();
	}
	void make_noise()
	{
		if (data.data == nullptr || data.len == 0)
			return;
		// Processing: Basic noise 
		for (int i = 0; i < data.len; ++i)
		{
			byte& c = data.data[i];
			if (c < 0) c = 0;
			else if (0 < c && c < 50) c = 50;
			else c = 200;
		}
	}
} wav(pathIn);

int main()
{
	wav.read();
	wav.make_noise();
	wav.write(pathOut);
	return 0;
}
