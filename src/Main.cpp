# include <Siv3D.hpp>
# include <HamFramework.hpp>

void drawFigure(int id, int mainId, Vec2 pos, float size)
{
    switch(id)
    {
        case 0:
            Circle(pos,size).draw(id==mainId ? Palette::Red : Palette::White);
            break;
        case 1:
            Shape2D::Plus(size, size/2, pos, 45_deg).draw(id==mainId ? Palette::Red : Palette::White);
            break;
        case 2:
            Shape2D::Hexagon(size, pos).draw(id==mainId ? Palette::Red : Palette::White);
            break;
        case 3:
            Shape2D::Star(size, pos).draw(id==mainId ? Palette::Red : Palette::White);
            break;
        default:
            break;
    }
}

void Main()
{
    Graphics::SetBackground(ColorF(0.2));
    Window::Resize(480, 720);
    
    const Font font(50,Typeface::Bold);
    
    const float jumpForce = -0.1;
    const float fallForce = 0.1;
    
    const int maxRotateCount = 4;
    
    bool isStart = false;
    
    Vec2 player(Window::Center().x,0);
    float size = 10;
    float flameSize = 5;
    float speed = 10;
    float force = fallForce;
    float velocityY = 0;
    
    int rotateCount = 0;
    
    while (System::Update())
    {
        //挙動
        if(isStart)
        {
            //プレイヤーの移動処理
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
            
            //図形選択処理
            if(KeyZ.down())
            {
                --rotateCount;
                if(rotateCount<0) rotateCount = maxRotateCount-1;
            }
            
            if(KeyX.down())
            {
                ++rotateCount;
                if(maxRotateCount<=rotateCount) rotateCount = 0;
            }
            
            //床の移動処理
            
        }
        else
        {
            if(KeyZ.down())
            {
                isStart = true;
            }
        }
        
        //描画
        if(!isStart)
        {
            font(U"Press Z Key").drawAt(Window::Center());
        }
        
        RectF(player-Vec2(size/2,size/2),size).rotated(45_deg).draw().drawFrame(0,flameSize,ColorF(1.0, 0.4, 0.3));
        
        for (auto i : Range(0, maxRotateCount-1))
        {
            Print(rotateCount);
            const Vec2 pos = OffsetCircular(player, size*3, 180_deg + 360_deg/maxRotateCount*(i-rotateCount));
            
            drawFigure(i,rotateCount,pos,size*2);
        }
    }
}
