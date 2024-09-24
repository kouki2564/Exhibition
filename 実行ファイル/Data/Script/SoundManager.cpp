#include "SoundManager.h"
#include "DxLib.h"

SoundManager::SoundManager()
{
	Sound temp;
	//// BGM
	temp.handle = LoadSoundMem("Data/Sound/BGM_Title.mp3");
	temp.vol = 255;
	m_sound.insert(std::make_pair("BGM_Title", temp));
	temp.handle = LoadSoundMem("Data/Sound/BGM_Hub.mp3");
	temp.vol = 150;
	m_sound.insert(std::make_pair("BGM_Hub", temp));
	temp.handle = LoadSoundMem("Data/Sound/BGM_Game.mp3");
	temp.vol = 140;
	m_sound.insert(std::make_pair("BGM_Game", temp));


	//// SE
	/// TITLE
	temp.handle = LoadSoundMem("Data/Sound/SE_Title.mp3");
	temp.vol = 150;
	m_sound.insert(std::make_pair("SE_Title", temp));

	/// HUB
	temp.handle = LoadSoundMem("Data/Sound/SE_Hub_choose.mp3");
	temp.vol = 255;
	m_sound.insert(std::make_pair("SE_Hub_choose", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Hub_Dec.mp3");
	temp.vol = 150;
	m_sound.insert(std::make_pair("SE_Hub_Dec", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Hub_LvDown.mp3");
	temp.vol = 255;
	m_sound.insert(std::make_pair("SE_Hub_LvDown", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Hub_LvUp.mp3");
	m_sound.insert(std::make_pair("SE_Hub_LvUp", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Hub_Reward.mp3");
	m_sound.insert(std::make_pair("SE_Hub_Reward", temp));

	/// GAME
	// PLAYER
	temp.handle = LoadSoundMem("Data/Sound/SE_Player_Attack.mp3");
	temp.vol = 150;
	m_sound.insert(std::make_pair("SE_Player_Attack", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Player_Dodge.mp3");
	temp.vol = 170;
	m_sound.insert(std::make_pair("SE_Player_Dodge", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Player_Hit.mp3");
	temp.vol = 255;
	m_sound.insert(std::make_pair("SE_Player_Hit", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Player_Run.mp3");
	m_sound.insert(std::make_pair("SE_Player_Run", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Player_Skill.mp3");
	m_sound.insert(std::make_pair("SE_Player_Skill", temp));
	// BOSS
	temp.handle = LoadSoundMem("Data/Sound/SE_Boss_AttackBig.mp3");
	m_sound.insert(std::make_pair("SE_Boss_AttackBig", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Boss_AttackRight.mp3");
	m_sound.insert(std::make_pair("SE_Boss_AttackRight", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Boss_Hit.mp3");
	m_sound.insert(std::make_pair("SE_Boss_Hit", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Boss_taccle.mp3");
	m_sound.insert(std::make_pair("SE_Boss_taccle", temp));
	// OTHER
	temp.handle = LoadSoundMem("Data/Sound/SE_Game_Lose.mp3");
	temp.vol = 180;
	m_sound.insert(std::make_pair("SE_Game_Lose", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_Game_Win.mp3");
	temp.vol = 130;
	m_sound.insert(std::make_pair("SE_Game_Win", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_GameReady.mp3");
	temp.vol = 200;
	m_sound.insert(std::make_pair("SE_GameReady", temp));
	temp.handle = LoadSoundMem("Data/Sound/SE_GameStart.mp3");
	m_sound.insert(std::make_pair("SE_GameStart", temp));
}

SoundManager::~SoundManager()
{
}

void SoundManager::UpdateVol(std::string name, int vol)
{
	// ‰¹—ÊÝ’è
	//if (m_num == 0 && m_volPow * 51 != m_volume)
	//{
	//	m_volume = m_volPow * 51;
	//	if (this->ChackPlaying(name))
	//	{
	//		if (m_volPow == 0)
	//		{
	//			this->Stop();
	//		}
	//	}
	//	else
	//	{
	//		if (m_volPow != 0)
	//			this->LoopPlay();
	//	}
	//	
	//}
	ChangeVolumeSoundMem(m_sound[name].vol, m_sound[name].handle);
}

void SoundManager::OnePlay(std::string name)
{
	if (m_sound[name].vol != 0)
		ChangeVolumeSoundMem(m_sound[name].vol, m_sound[name].handle);
		PlaySoundMem(m_sound[name].handle, DX_PLAYTYPE_BACK, true);
}

void SoundManager::LoopPlay(std::string name)
{
	if (m_sound[name].vol != 0)
		ChangeVolumeSoundMem(m_sound[name].vol, m_sound[name].handle);
		PlaySoundMem(m_sound[name].handle, DX_PLAYTYPE_LOOP, true);
}

void SoundManager::Stop(std::string name)
{
	StopSoundMem(m_sound[name].handle);
}

bool SoundManager::ChackPlaying(std::string name)
{
	return CheckSoundMem(m_sound[name].handle);
}
