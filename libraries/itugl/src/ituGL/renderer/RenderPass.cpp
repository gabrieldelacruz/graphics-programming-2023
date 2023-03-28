#include <ituGL/renderer/RenderPass.h>

#include <ituGL/renderer/Renderer.h>
#include <cassert>

RenderPass::RenderPass() : m_renderer(nullptr)
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::SetRenderer(Renderer* renderer)
{
    m_renderer = renderer;
}

Renderer& RenderPass::GetRenderer()
{
    assert(m_renderer);
    return *m_renderer;
}

const Renderer& RenderPass::GetRenderer() const
{
    assert(m_renderer);
    return *m_renderer;
}
