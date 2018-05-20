# include <Siv3D.hpp>
# include <HamFramework.hpp>

Vec2 GetStickInput()
{
    if(JoyCon::IsJoyConL(Gamepad(0)))
    {
        const auto joy = JoyCon(Gamepad(0));
        
        if (auto d = joy.povD8())
        {
            return Circular(1, *d * 45_deg);
        }
        else
        {
            return Vec2(0,0);
        }
    }
    else
    {
        return Vec2(KeyRight.pressed() - KeyLeft.pressed(), KeyDown.pressed() - KeyUp.pressed());
    }
}

void Main()
{
    Window::Resize(1280, 720);
    Effect effect;
    
    const double joystickInterval = 360_deg/8;
    
    Vec2 pos(640 - 100, 100);
    float speed = 10.0;
    
    while (System::Update())
    {
        Print(GetStickInput());
        /*
        if (JoyCon::IsJoyConL(Gamepad(0)))
        {
            const auto joy = JoyCon(Gamepad(0));

            if (auto d = joy.povD8())
            {
                Print(*d);
                pos += Circular(speed, *d * joystickInterval);
            }
            
            if (joy.button2.down())
            {
                effect.add([center = pos](double t)
                {
                    Circle(center, 20 + t * 200).drawFrame(10, 0, AlphaF(1.0 - t));
                    return t < 1.0;
                });
            }
        }
        */
        
        Circle(pos, 30).draw(ColorF(0.0, 0.75, 0.9));
        effect.update();
    }
}
