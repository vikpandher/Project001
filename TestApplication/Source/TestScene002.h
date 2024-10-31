// =============================================================================
// @AUTHOR Vik Pandher
// @DATE 2024-10-30

#pragma once

#include "TestInstructionScene001.h"
#include "TestSceneBase001.h"



class TestScene002 : public TestSceneBase001
{
public:
    TestScene002(Project001::Application* applicationPtr);
    ~TestScene002();

    TestScene002(TestScene002& other) = delete;
    void operator=(const TestScene002&) = delete;

    void HandleEvent(Project001::Event& event) override;

protected:
    void ProcessInitializeEvent(Project001::InitializeEvent& initializeEvent);
    void ProcessDeinitializeEvent(Project001::DeinitializeEvent& deinitializeEvent);

    // -------------------------------------------------------------------------

    TestInstructionScene001 instructionScene_;

private:
    void AutoIdMapTest() const;
    void UniqueBiMapTest() const;
    void ComponentContainerTest() const;
    void ComponentStoresTest() const;
    void ResourceStoresTest() const;
    void ResourceStoresTest2() const;
    void MeshLoaderTest() const;
    void TextureLoaderTest() const;
};