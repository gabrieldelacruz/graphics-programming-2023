#pragma once

class Renderer;

class RenderPass
{
public:
    RenderPass();
    virtual ~RenderPass();

    virtual void Render() = 0;

protected:
    Renderer& GetRenderer();
    const Renderer& GetRenderer() const;

private:
    friend class Renderer;
    void SetRenderer(Renderer* renderer);

private:
    Renderer* m_renderer;
};
