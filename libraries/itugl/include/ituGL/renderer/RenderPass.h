#pragma once

#include <memory>

class Renderer;
class FramebufferObject;

class RenderPass
{
public:
    RenderPass(std::shared_ptr<const FramebufferObject> targetFramebuffer = nullptr);
    virtual ~RenderPass();

    std::shared_ptr<const FramebufferObject> GetTargetFramebuffer() const;

    virtual void Render() = 0;

protected:
    Renderer& GetRenderer();
    const Renderer& GetRenderer() const;

protected:
    std::shared_ptr<const FramebufferObject> m_targetFramebuffer;

private:
    friend class Renderer;
    void SetRenderer(Renderer* renderer);

private:
    Renderer* m_renderer;
};
