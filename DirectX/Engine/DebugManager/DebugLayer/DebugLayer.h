#pragma once

#include "Inspector/IInspector.h"
#include "../../../Device/FileOperator.h"
#include "../../../Math/Math.h"
#include <memory>

class ImGuiInspector;

class DebugLayer
    : public FileOperator
{
public:
    DebugLayer();
    ~DebugLayer();
    void draw() const;
    IInspector* inspector() const;

private:
    DebugLayer(const DebugLayer&) = delete;
    DebugLayer& operator=(const DebugLayer&) = delete;

    virtual void childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

private:
    std::unique_ptr<ImGuiInspector> mInspector;
};
