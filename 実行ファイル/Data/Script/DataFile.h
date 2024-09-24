#pragma once
#include "CharacterBase.h"
#include <map>
#include <string>

class DataFile
{
public:
	DataFile();
	~DataFile();

	Status GetStatus(std::string name) { return m_charaData[name]; }
	int GetPoint() const { return m_point; }
	bool GetisTrophy(int trophyNum) const { return m_isTrophy[trophyNum]; }

	void SetData(SendData data);
	void SetIsTorophy(int trophyNum, bool isHave) { m_isTrophy[trophyNum] = isHave; }

private:

	void CreateData();
	void Save();
	void Load();

	void InitStatus(std::string name);

private:
	// ファイルデータ
	std::map<std::string, Status> m_charaData;

	int m_point;
	bool m_isTrophy[6];


};

