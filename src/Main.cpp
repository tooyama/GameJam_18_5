# include <Siv3D.hpp>
# include <HamFramework.hpp>

void Main()
{
    Graphics::SetBackground(ColorF(0.2));
    Window::Resize(480, 720);
    
    const float jumpForce = -0.5;
    const float fallForce = 0.1;
    
    bool isStart = false;
    
    Vec2 player(Window::Center().x,0);
    float size = 20;
    float flameSize = 5;
    float speed = 10;
    float force = fallForce;
    float velocityY = 0;
    
    while (System::Update())
    {
        if(isStart)
        {
            if(Window::Height() < player.y)
            {
                force = jumpForce;
            }
            float velocityTemp = player.y;
            player.y += (player.y - velocityY) + force;
            velocityY = velocityTemp;
            force = fallForce;
        
            Vec2 dir(KeyRight.pressed() - KeyLeft.pressed(), 0);
            if (!dir.isZero()) player.moveBy(dir.setLength(speed));
            player.clamp(Rect(0,-500,Window::Width(),Window::Height()+1000));
        }
        else
        {
            if(KeyZ.down())
            {
                isStart = true;
            }
        }
        
        Circle(player, size).draw(ColorF(1.0, 0.4, 0.3)).drawFrame(flameSize,0);
    }
}
