#pragma once

#include <ituGL/renderer/RenderPass.h>

class ForwardRenderPass : public RenderPass
{
public:
    ForwardRenderPass();
    ForwardRenderPass(int drawcallCollectionIndex);

    void Render() override;

private:
    int m_drawcallCollectionIndex;
};
