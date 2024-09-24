#pragma once
#include "SceneBase.h"
class DebugScene :
    public SceneBase
{
public:
    DebugScene();
    ~DebugScene();

    void Init(std::shared_ptr<Effect>& effectmanager, std::shared_ptr<SoundManager>& soundManager, std::shared_ptr<DataFile>& dataFile);
    void Update();
    void Draw() const;

private:
};

