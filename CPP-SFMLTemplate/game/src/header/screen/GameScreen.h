#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../player/MainCharacterBullet.h"
#include <SFML/Graphics.hpp>
#include "../../../../library/src/header/MaterialButton.h"
#include "../App.h"
#include "../npc/NPCharacter.h"
#include "../player/MainCharacter.h"
#include "../game/GameMap.h"

using namespace std;
using namespace sf;

class GameScreen
{

private:
    RectangleShape gameMenuScreenBackground;
    App *mApp;
    MainCharacter *mMainCharacter;
    GameMap *mGameMap;
    vector<MainCharacterBullet *> mMainCharacterBullets;
    vector<NPCharacter *> mNPCharacters;
    int mColorEndPortal;
    bool mColorAscending;
    int mBackgroundR, mBackgroundG, mBackgroundB;

public:
    GameScreen()
    {

        mColorEndPortal = 77;
        mColorAscending = true;

        mBackgroundR = 32;
        mBackgroundG = 165;
        mBackgroundB = 227;

    }

    ~GameScreen()
    {

    }

public:

    void draw(RenderWindow &window)
    {

        int i, j, mGameOffsetY;
        RectangleShape item;
        RectangleShape mEndPortal;

        mGameOffsetY = mApp->getGameOffsetY();

        gameMenuScreenBackground.setFillColor(Color(mBackgroundR, mBackgroundG, mBackgroundB));
        gameMenuScreenBackground.setSize(Vector2f((float) window.getSize().x, (float) window.getSize().y));
        window.draw(gameMenuScreenBackground);

        vector < ItemModel * > mLvlItems = mGameMap->getItemsByLvl();
        for (ItemModel *mItem : mLvlItems)
        {
            item.setFillColor(Color(168, 12, 147, 150));
            item.setPosition(Vector2f((float) mItem->getStartPos().x, (float) mItem->getStartPos().y + mGameOffsetY));
            item.setSize(Vector2f((float) mItem->getSize().x, (float) mItem->getSize().y));
            window.draw(item);
        }

        vector < ItemModel * > mEndPortals = mGameMap->getEndPortalByLvl();
        for (ItemModel *mEndPortalItem : mEndPortals)
        {
            mEndPortal.setFillColor(Color(232, 21, mColorEndPortal));
            if (mColorAscending)
            {
                mColorEndPortal++;
                if (mColorEndPortal == 155)
                {
                    mColorAscending = false;
                }
            } else
            {
                mColorEndPortal--;
                if (mColorEndPortal == 77)
                {
                    mColorAscending = true;
                }
            }
            mEndPortal.setPosition(Vector2f((float) mEndPortalItem->getStartPos().x,
                                            (float) mEndPortalItem->getStartPos().y + mGameOffsetY));
            mEndPortal.setSize(Vector2f((float) mEndPortalItem->getSize().x, (float) mEndPortalItem->getSize().y));
            window.draw(mEndPortal);
        }

        for (NPCharacter *mNPCharacter : mNPCharacters)
        {
            window.draw(mNPCharacter->getSprite());
        }

        window.draw(mMainCharacter->getSprite());

        for (MainCharacterBullet *mMainCharacterBullet : mMainCharacterBullets)
        {
            window.draw(mMainCharacterBullet->getSprite(mGameOffsetY));
        }

    }

    void initNPCs()
    {
        vector < ItemModel * > mNPCs = mGameMap->getNPCByLvl();
        NPCharacter *mNPCharacter;
        for (ItemModel *mNPC : mNPCs)
        {
            mNPCharacter = new NPCharacter(
                    mApp->getGameMap(),
                    mNPC->isFacingRight(),
                    mNPC->getStartPos().x,
                    mNPC->getStartPos().y,
                    mNPC->getArea(),
                    mApp->getImageLoader()
            );
            mNPCharacters.push_back(mNPCharacter);
        }
    }

    void setApp(App *app)
    {
        this->mApp = app;
        mMainCharacter = mApp->getMainCharacter();
        mGameMap = mApp->getGameMap();
        mMainCharacter->setGameMap(mApp->getGameMap());
    }

    void shootBullets(int type)
    {

        MainCharacterBullet *mMainCharacterBullet = new MainCharacterBullet();
        Vector2f mPosition;
        mPosition.x = mMainCharacter->getCharacterPosition().x;
        mPosition.y = mMainCharacter->getGameHeight();
        if (type == 1)
        {
            mPosition.y -= 53;
            if (mMainCharacter->faceRight())
            {
                mPosition.x = mPosition.x + 15;
            } else
            {
                mPosition.x = mPosition.x - 15;
            }
            mMainCharacterBullet->init(
                    mPosition,
                    type,
                    mMainCharacter->faceRight(),
                    1,
                    4.0f * 1.4f,
                    0.3f
            );
        } else if (type == 2)
        {
            mPosition.y -= 55;
            if (mMainCharacter->faceRight())
            {
                mPosition.x = mPosition.x + 15;
            } else
            {
                mPosition.x = mPosition.x - 15;
            }
            mMainCharacterBullet->init(
                    mPosition,
                    type,
                    mMainCharacter->faceRight(),
                    2,
                    4.0f * 0.8f,
                    0.4f
            );
        }
        mMainCharacterBullets.push_back(mMainCharacterBullet);

    }

    void inputListener(Event event)
    {
        if (event.key.code == Keyboard::Space)
        {
            mMainCharacter->jump(650.0f);
        }
        if (event.key.code == Keyboard::X)
        {
            system("pause");
        }
        if (event.key.code == Keyboard::Left || event.key.code == Keyboard::A)
        {
            mMainCharacter->move(-20.0f);
        } else if (event.key.code == Keyboard::Right || event.key.code == Keyboard::D)
        {
            mMainCharacter->move(20.0f);
        }
        if (event.key.code == Keyboard::Num1 || event.key.code == Keyboard::Numpad1)
        {
            shootBullets(1);
        } else if (event.key.code == Keyboard::Num2 || event.key.code == Keyboard::Numpad2)
        {
            shootBullets(2);
        }
    }

    void update(float speed)
    {

        int i, j, mGameOffsetY;
        RectangleShape mEndPortal;
        mGameOffsetY = mMainCharacter->getGameOffsetY();

        mMainCharacter->update(speed);

    }

    void initNewLvl()
    {
        mMainCharacterBullets.clear();
        mNPCharacters.clear();
        mMainCharacter->reset();

        mColorEndPortal = 77;
        mColorAscending = true;

        initNPCs();
    }

};
