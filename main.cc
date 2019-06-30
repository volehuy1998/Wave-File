#include <fstream>
#include <cstring>

using namespace std;
using byte = char;

const byte *pathIn =  "origin.wav";
const byte *pathOut = "noise.wav";

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
	byte ExtraParamSize[2];
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
} data;

int main()
{
	ifstream is (pathIn,  ios::binary);
	ofstream os (pathOut, ios::binary); 
	// Read header
	is.read((byte*)&riff, sizeof riff);
	is.read((byte*)&fmt, sizeof fmt);
	is.read((byte*)&data_header, sizeof data_header);

	// Get length
	int len = *(int*)data_header.Subchunk2Size;

	// Alloc
	data.data = new byte[len];

	// Read data
	is.read(data.data, len);

	// Processing: Basic noise 
	for (int i = 0; i < len; ++i)
	{
		byte& c = data.data[i];
		if (c < 0) c = 0;
		else if (0 < c && c < 50) c = 50;
		else c = 200;
	}
	
	// Write data
	os.write((byte*)&riff, sizeof riff);
	os.write((byte*)&fmt, sizeof fmt);
	os.write((byte*)&data_header, sizeof data_header);
	os.write(data.data, len);

	// Fin
	delete[] data.data;
	is.close();
	os.close();
	return 0;
}
