#include "DebugLayer.h"
#include "Inspector/ImGuiInspector.h"

DebugLayer::DebugLayer()
    : FileOperator("DebugLayer")
    , mInspector(std::make_unique<ImGuiInspector>())
{
}

DebugLayer::~DebugLayer() = default;

void DebugLayer::draw() const {
    mInspector->drawInspect();
}

IInspector* DebugLayer::inspector() const {
    return mInspector.get();
}

void DebugLayer::childSaveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mInspector->writeAndRead(inObj, alloc, mode);
}
