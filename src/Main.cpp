# include <Siv3D.hpp>
# include <HamFramework.hpp>

class FloorFigure
{
public:
    FloorFigure(float posY, Array<int> figures)
    :m_posY(posY)
    ,m_isAlive(true)
    ,m_figures(figures)
    {}
    
    ~FloorFigure(){}
    
    const float getPos()
    {
        return m_posY;
    }
    
    const int getFigure(int i)
    {
        return m_figures[i];
    }
    
    const size_t getArraySize()
    {
        return m_figures.size();
    }
    
    const bool alive()
    {
        return m_isAlive;
    }
    
    void dead()
    {
        m_isAlive = false;
    }
    
private:
    float m_posY;
    bool m_isAlive;
    Array<int> m_figures;
};

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
    Effect effect;
    
    constexpr int32 NumKeys = 20;
    std::array<Audio, NumKeys> sounds;
    for (auto i : step(NumKeys))
    {
        sounds[i] = Audio(Wave(0, static_cast<uint8>(PianoKey::A3 + i), 0.5s));
    }
    
    const Font font(50,Typeface::Bold);
    const int maxRotateCount = 4;
    
    bool isStart = false;
    bool isAlive = true;
    
    Vec2 player(Window::Center().x,0);
    float size = 10;
    float flameSize = 5;
    float speed = 10;
    float fallForce = 0.1;
    float force = fallForce;
    float jumpForce = -12.5;
    float jumpRange = 0.01;
    float velocityY = 0;
    
    int rotateCount = 0;
    
    Array<FloorFigure> floorFigures;
    float floorPosY=0;
    float tempFloorPos=0;
    float floorRange=0;
    
    Stopwatch stopwatch;
    Stopwatch resultStopwatch;
    float resultPosY = Window::Height();
    int score = 0;
    
    /*
    const CSVData csv(U"csv/map.csv");
    
    if (!csv)
    {
        return;
    }
    
    for(auto i : step(csv.rows()))
    {
        Array<int> figure;
        
        for(auto j : Range(1, csv.columns(i)-1))
        {
            figure.push_back(csv.get<int>(i, j));
        }
        
        floorFigures.push_back(FloorFigure(csv.get<float>(i, 0),figure));
    }
    */

    floorFigures.push_back(FloorFigure(float(360),Array<int>({0,0,0,0,0,0,0,0})));
    floorFigures.push_back(FloorFigure(float(720),Array<int>({1,1,1,1,0,0,0,0})));
    floorFigures.push_back(FloorFigure(float(820),Array<int>({2,2,1,1,0,0,0,0})));
    floorFigures.push_back(FloorFigure(float(920),Array<int>({0,0,1,1,2,2,3,3})));
    floorFigures.push_back(FloorFigure(float(1400),Array<int>({0,0,1,1,2,2,3,3})));
    floorFigures.push_back(FloorFigure(float(1720),Array<int>({1,0,0,0,2,2,3,3})));
    floorFigures.push_back(FloorFigure(float(2000),Array<int>({0,0,2,0,1,3,2,1})));
    floorFigures.push_back(FloorFigure(float(2200),Array<int>({3,1,1,2,1,1,0,0})));
    floorFigures.push_back(FloorFigure(float(2300),Array<int>({2,0,2,2,1,2,3,3})));
    floorFigures.push_back(FloorFigure(float(2720),Array<int>({0,0,1,1,2,2,3,3,1,1,1,1,1,1,1,1})));
    floorFigures.push_back(FloorFigure(float(3000),Array<int>({1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1})));
    floorFigures.push_back(FloorFigure(float(3100),Array<int>({3,0,1,1,2,2,0,2,1,1,0,0,0,2,2,3})));
    floorFigures.push_back(FloorFigure(float(3200),Array<int>({1,3,2,2,0,0,1,3,3,1,3,1,2,1,0,1})));
    floorFigures.push_back(FloorFigure(float(3300),Array<int>({0,1,3,1,0,1,2,2,0,2,2,0,2,0,1,1})));
    floorFigures.push_back(FloorFigure(float(3620),Array<int>({2,0,1,1,2,3,2,3,1,0,1,2,3,0,1,0})));
    floorFigures.push_back(FloorFigure(float(4000),Array<int>({0,0,1,1,2,2,2,3,2,2,1,1,2,0,0,1})));
    floorFigures.push_back(FloorFigure(float(4320),Array<int>({0,1,2,1,2,3,2,3,1,2,1,3,1,2,1,0})));
    floorFigures.push_back(FloorFigure(float(4420),Array<int>({2,0,1,1,2,2,3,3,1,2,1,1,2,1,1,1})));
    floorFigures.push_back(FloorFigure(float(4500),Array<int>({0,3,1,1,2,2,0,1,1,2,3,1,1,2,1,3})));
    floorFigures.push_back(FloorFigure(float(5000),Array<int>({3,3,3,3,2,2,2,1,1,1,1,3,3,3,3,3})));
     
    while (System::Update())
    {
        //挙動
        if(isStart && isAlive)
        {
            //当たり判定
            for(auto& i : step(floorFigures.size()))
            {
                if(floorFigures[i].alive())
                {
                    size_t arraySize = floorFigures[i].getArraySize();
                    float figureSize = Window::Width()/arraySize;
                    
                    for(auto j : step(arraySize))
                    {
                        Vec2 pos(figureSize/2 + j*figureSize,floorFigures[i].getPos()+floorPosY);
                        Circle floorCircle(pos, figureSize);
                        Circle playerCircle(player+Vec2(0,size),size/2);

                        if(floorCircle.intersects(playerCircle))
                        {
                            if(floorFigures[i].alive() && floorFigures[i].getFigure(j) == rotateCount)
                            {
                                if(i<floorFigures.size()-1)
                                {
                                    floorRange = floorFigures[i+1].getPos() - floorFigures[i].getPos();
                                }
                                else
                                {
                                    floorRange = 0;
                                }
                                
                                ++rotateCount;
                                if(maxRotateCount<=rotateCount) rotateCount = 0;
                                floorFigures[i].dead();
                                stopwatch.start();
                                force = jumpForce;
                                fallForce += jumpRange;
                                
                                effect.add([center = pos](double t)
                                {
                                    Circle(center, 40 + t * 400).drawFrame(10, 0, AlphaF(1.0 - t));
                                    return t < 1.0;
                                });
                                
                                sounds[Random(0,20)].playOneShot(1.0);
                            }
                            else
                            {
                                isAlive = false;
                                resultStopwatch.start();
                                effect.add([center = pos](double t)
                               {
                                   Circle(center, 40 + t * 400).drawFrame(10, 0, ColorF(1.0,0,0), AlphaF(1.0 - t));
                                   return t < 1.0;
                               });
                            }
                        }
                    }
                }
            }
            
            //プレイヤーの移動処理
            float velocityTemp = player.y;
            if((player.y - velocityY) + force < 0)
            {
                ++score;
            }
            player.y += (player.y - velocityY) + force;
            velocityY = velocityTemp;
            force = fallForce;
        
            Vec2 dir(KeyRight.pressed() - KeyLeft.pressed(), 0);
            if (!dir.isZero()) player.moveBy(dir.setLength(speed));
            player.clamp(Rect(0,-500,Window::Width(),Window::Height()+1000));
            
            //床の移動処理
            if(stopwatch.isStarted())
            {
                const double t = Min(stopwatch.ms() / 500.0, 1.0);
                
                floorPosY = tempFloorPos-floorRange*t;
                
                if (t == 1.0)
                {
                    tempFloorPos = floorPosY;
                    stopwatch.reset();
                    stopwatch.pause();
                }
            }
        }
        else
        {
            if(KeyRight.down() || KeyLeft.down())
            {
                isStart = true;
            }
        }
        
        //描画
        if(!isStart)
        {
            font(U"Press ← or → Key").drawAt(Window::Center()-Vec2(0,Window::Height()/4));
        }
        
        if(isAlive)
        {
            RectF(player-Vec2(size/2,size/2),size).rotated(45_deg).draw().drawFrame(0,flameSize,ColorF(1.0, 0.4, 0.3));
        
            for (const auto i : step(maxRotateCount))
            {
                const Vec2 pos = OffsetCircular(player, size*3, 180_deg + 360_deg/maxRotateCount*(i-rotateCount));
                drawFigure(i,rotateCount,pos,size*2);
            }
            
            if(isStart)font(U"Score : ",score).draw(0,0,ColorF(1.0,0,0));
        }
        
        for(auto& floorFigure: floorFigures)
        {
            if(floorFigure.alive())
            {
                size_t arraySize = floorFigure.getArraySize();
                float figureSize = Window::Width()/arraySize;
            
                for(auto i : step(arraySize))
                {
                    Vec2 pos(figureSize/2 + i*figureSize,floorFigure.getPos()+floorPosY);
                    drawFigure(floorFigure.getFigure(i), rotateCount, pos, figureSize/2);
                }
            }
        }
        
        effect.update();
        
        if(!isAlive)
        {
            if(resultStopwatch.isStarted())
            {
                const double t = Min(resultStopwatch.ms() / 500.0, 1.0);
                
                resultPosY = Window::Height() - Window::Height()*t;
                
                if (t == 1.0)
                {
                    resultStopwatch.pause();
                }
            }
            
            RectF(0,resultPosY,Window::Size()).draw(ColorF(0.8));
            font(U"GameOver").drawAt(Window::Center()-Vec2(0,Window::Height()/4)+Vec2(0,+resultPosY),ColorF(0.8,0,0));
            font(U"Score : ",score).drawAt(Window::Center()+Vec2(0,resultPosY),ColorF(0.8,0,0));
        }
    }
}
