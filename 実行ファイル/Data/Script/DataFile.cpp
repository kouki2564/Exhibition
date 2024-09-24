#include "DataFile.h"
#include "DxLib.h"
#include "Game.h"
#include <cassert>
#include <cerrno>

DataFile::DataFile()
{
	Status temp;
	m_charaData.insert(std::make_pair("Player", temp));
	m_charaData.insert(std::make_pair("Boss", temp));
	Load();
}

DataFile::~DataFile()
{
	Save();
}

void DataFile::SetData(SendData data)
{
	m_charaData["Player"].Level = data.playerLevel;
	m_charaData["Player"].MaxLevel = data.playerMaxLevel;
	InitStatus("Player");
	m_charaData["Boss"].Level = data.bossLevel;
	m_charaData["Boss"].MaxLevel = data.bossMaxLevel;
	InitStatus("Boss");
	m_point = data.point;
	if (m_point > 99999)
	{
		m_point = 99999;
	}
}

void DataFile::CreateData()
{
	Status temp;
	m_charaData["Player"].Level = 1;
	InitStatus("Player");
	m_charaData["Boss"].Level = 1;
	InitStatus("Player");
	m_point = 0;

	for (int i = 0; i < 6; i++)
	{
		m_isTrophy[i] = false;
	}
}

void DataFile::Save()
{
	// ファイルの枠作成
	FILE* Writefp = nullptr;

	// == 0　でファイル存在
	if (fopen_s(&Writefp, "Data/GameData/statusData", "wb") == 0)
	{

		fwrite(&m_charaData["Player"].Level, sizeof(int), 1, Writefp);
		fwrite(&m_charaData["Player"].MaxLevel, sizeof(int), 1, Writefp);
		fwrite(&m_charaData["Boss"].Level, sizeof(int), 1, Writefp);
		fwrite(&m_charaData["Boss"].MaxLevel, sizeof(int), 1, Writefp);
		fwrite(&m_point, sizeof(int), 1, Writefp);

		for (int i = 0; i < 6; i++)
		{
			fwrite(&m_isTrophy[i], sizeof(bool), 1, Writefp);
		}

		fclose(Writefp);
	}
	else
	{
		assert(false);
		return;
	}
}

void DataFile::Load()
{
	FILE* Readfp = nullptr;
	// == 0　でファイル存在
	if (fopen_s(&Readfp, "Data/GameData/statusData", "rb") == 0)
	{
		// 配列個数分のintを入れる
		for (int i = 0; i < 5; i++)
		{
			fread(&m_charaData["Player"].Level, sizeof(int), 1, Readfp);
			fread(&m_charaData["Player"].MaxLevel, sizeof(int), 1, Readfp);
			InitStatus("Player");
			fread(&m_charaData["Boss"].Level, sizeof(int), 1, Readfp);
			fread(&m_charaData["Boss"].MaxLevel, sizeof(int), 1, Readfp);
			InitStatus("Player");
			fread(&m_point, sizeof(int), 1, Readfp);

			for (int i = 0; i < 6; i++)
			{
				fread(&m_isTrophy[i], sizeof(bool), 1, Readfp);
			}
		}
		fclose(Readfp);
	}
	// ファイルが無い(初プレイ)時の初期データ作成
	else
	{
		CreateData();
		return;
	}
}

void DataFile::InitStatus(std::string name)
{
	if (name == "Player")
	{
		m_charaData["Player"].HP = 100 + m_charaData["Player"].Level * 100;
		m_charaData["Player"].POW = 15 + m_charaData["Player"].Level * 5;
		m_charaData["Player"].AGI = 5 + m_charaData["Player"].Level * 5;
	}
	else if (name == "Boss")
	{
		m_charaData["Boss"].HP = 200 + m_charaData["Boss"].Level * 300;
		m_charaData["Boss"].POW = 15 + m_charaData["Boss"].Level * 8;
		m_charaData["Boss"].AGI =  5 + m_charaData["Boss"].Level * 2;
	}
}
