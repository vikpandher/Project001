#pragma once

#include "TestSceneBase001.h"

#include <vector>


namespace Project001
{
    struct KeyEvent;
}

class TestScene004 : public TestSceneBase001
{
public:
    TestScene004();
    ~TestScene004();

    TestScene004(TestScene004& other) = delete;
    void operator=(const TestScene004&) = delete;

    const char* Name() override;

    void Initialize() override;

    void Deinitialize() override;

    void OnEvent(Project001::Event& event) override;

protected:
    void ClearIndiciesAndEntityIds();

    void ProcessKeyEvent(Project001::KeyEvent& keyEvent);

    // Mesh Indicies: ----------------------------------------------------------

    std::vector<unsigned int> meshIndicies_;

    // Texture Indicies: -------------------------------------------------------

    unsigned int _32x32_123abc_TextureIndex_;

    // Entity Ids: -------------------------------------------------------------

    std::vector<unsigned int> entityIds_;

private:
};