#pragma once
#include <vector>
#include <map>
#include <string>

struct Sound
{
	int handle;
	int vol = 150;
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void UpdateVol(std::string name, int vol);

	void OnePlay(std::string name);
	void LoopPlay(std::string name);
	void Stop(std::string name);

	bool ChackPlaying(std::string name);
	int GetVol(std::string name) { return m_sound[name].vol; }

private:
	// saund
	std::map < std::string, Sound> m_sound;
};

