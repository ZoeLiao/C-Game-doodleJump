#include <SFML/Graphics.hpp>//畫面設計
#include <SFML/Audio.hpp>//嵌入音樂
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>

using namespace sf;
using namespace std;

struct point
{ int x,y;};

string int2str(int &);

int main()
{
    srand(time(0));
    
    //設置畫面視窗
    float windowWidth = 400;
    float windowHeight = 533;
    RenderWindow app(VideoMode(windowWidth, windowHeight), "Doodle Game!");
    
    //調整速度
    app.setFramerateLimit(60);
    
    //設置texture，將圖片輸入
    Texture t1,t2,t3,t4;
    t1.loadFromFile("路徑.background.png");
    t2.loadFromFile("路徑.platform.png");
    t3.loadFromFile("路徑.doodleLeft.png");
    t4.loadFromFile("路徑.coin.png");
    
    Sprite sBackground(t1), sPlat(t2), sPers(t3),sCoin(t4);

    //輸入字型
    Font font;
    string score;
    string floorNum;
    int tempScore = 0;

    if (!font.loadFromFile("路徑.sansation.ttf"))
    {
        return EXIT_FAILURE;
    }

    
    //輸入音樂
    Music backgroundMusic;//背景音樂
    if (!backgroundMusic.openFromFile("路徑.背景音樂.ogg"))
    {
        return EXIT_FAILURE;
    }
    Music jumpMusic;//doodle跳躍時的音樂
    if (!jumpMusic.openFromFile("路徑.跳躍時的音樂.ogg"))
    {
        return EXIT_FAILURE;
    }
    Music coinMusic;//doodle吃硬幣時的音樂
    if (!coinMusic.openFromFile("路徑.吃硬幣時的音樂.ogg"))
    {
        return EXIT_FAILURE;
    }
    
    Music gameOverMusic;//gameover音樂
    if (!gameOverMusic.openFromFile("路徑.gameOver.ogg"))
    {
        return EXIT_FAILURE;
    }
    
    backgroundMusic.play(); // 播放背景音樂
    backgroundMusic.setLoop(true); // 反覆播放背景音樂
    
    //用於隨機設置位置的point
    point plat[20];
    for (int i=0;i<10;i++)
    {
        plat[i].x=rand()%400;
        plat[i].y=rand()%533;
    }
    
    int x=100,y=100,h=200;//x,y:doodle的位置；h距離天花板的高度，當y>h時畫面會改變、平台會重新分布
    float dx=0,dy=0;
    float height = 0;
    int floor = 0;
    int coinposition = 0;
    
    //遊戲開始
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Right) && x<350)//doodle往右但不會超出左右邊界
        {
            x+=3;
            t3.loadFromFile("路徑.doodleRight.png");//臉轉向右
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Left) && -30<x)//doodle往左但不會超出左右邊界
        {
            x-=3;
            t3.loadFromFile("/Users/appleuser/Desktop/PD/PD/images/doodleLeft.png");//臉轉向左
        }
        
        dy+=0.2;
        y+=dy;
        
        sPers.setPosition(x,y);
        
        //畫背景、還有doodle
        sBackground.setPosition(0,-3800+height);//讓背景隨著高度變化，因為畫面為第四象限，故畫面初始位置為-3800
        app.draw(sBackground);
        app.draw(sPers);
        
        //顯示原始分數
        Text text("score ", font, 20);
        Text textScore(score, font, 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(10,0);
        textScore.setPosition(80,0);
        app.draw(text);
        
        //顯示樓層
        Text textFloor("floor ", font, 20);//顯示文字"floor"
        textFloor.setFillColor(sf::Color::White);
        textFloor.setPosition(300,0);
        app.draw(textFloor);

        Text countFloor(floorNum, font, 20);//顯示目前的樓層數
        countFloor.setFillColor(sf::Color::White);
        countFloor.setPosition(360,0);
        app.draw(countFloor);
        
        //設置硬幣的位置、如果doodle吃到硬幣則加分
        
        sCoin.setPosition(plat[coinposition].x+15,plat[coinposition].y-30);
        app.draw(sCoin);
        sCoin.setScale(Vector2f(0.1, 0.1));//調整硬幣大小，原圖很大，所以要縮小尺寸
        
        if(sPers.getGlobalBounds().intersects(sCoin.getGlobalBounds()))
        {
            tempScore ++;
            coinMusic.play();// 吃硬幣時播放吃硬幣音樂
            coinposition = 21;//將硬幣位置設在平台array以外的位置，讓硬幣消失
        }
        
        //當doodle跳的高度足夠時，畫面更新、平台重新分配
        if (y<h)
        {
            //用於確保硬幣出現的時間點不會太晚，將硬幣設在平台更新時最高的平台
            int heightest = 400;//找出最高的平台位置
            int heightestNum = 0;//記錄最高平台的編號
            
            //重新分配平台、找出最高的平台
            for (int i=0;i<10;i++)
            {
                y=h;
                plat[i].y=plat[i].y-dy;
                if(plat[i].y<heightest)
                {
                    heightest = plat[i].y;
                    heightestNum = i;
                }
                
                if (plat[i].y>533)
                {
                    plat[i].y=0;
                    plat[i].x=rand()%400;
                }
            }
            
            //當h改變時，高度++(因為畫面為第四象限所以要另外設高度height)，重新設置硬幣
            height+=float(h)/2000;
            if(int(height/15)>floor)
            {
                floor = height/15;//計算樓層
                if(coinposition > 20)
                {
                    coinposition --;
                }
                else
                {
                    coinposition = heightestNum;//將硬幣位置設在最高的平台
                }

            }
            
            floorNum = int2str(floor);//將int轉為string
            app.draw(countFloor);
    
        }
        
        for (int i=0;i<10;i++)
        {
            if ((x+50>plat[i].x) && (x+20<plat[i].x+68) && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0))
            {
                dy=-10;
                jumpMusic.play(); // 彈跳時播放彈跳音樂
            }
        }
        
        //隨機設置平台
        for (int i=0;i<10;i++)
        {
            sPlat.setPosition(plat[i].x,plat[i].y);
            app.draw(sPlat);
            
        }
        
        score = int2str(tempScore);//將int的分數tempScore轉為string
        app.draw(textScore);

        
        //GameOver
        if (y>500)
        {
            backgroundMusic.pause();
            gameOverMusic.play();

            app.clear();//清空畫面
            Text gameOver("Game Over!! ", font, 50);
            gameOver.setFillColor(sf::Color::White);
            gameOver.setPosition(70,200);
            app.draw(gameOver);
            
        }
        app.display();
    }
    
    return 0;
}

//用於將int轉為string
string int2str(int &i)
{
    string s;
    stringstream ss(s);
    ss << i;
    return ss.str();
}
