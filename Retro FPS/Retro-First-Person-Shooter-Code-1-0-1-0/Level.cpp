#include "Level.h"
#include "UnitEnemy.h"


const char* Level::levelData{
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXC    2HAC    XXh a wXXXXXXXXXX                  XXXXXXh a wXXXXXXXXXXXXXXX              XXXXXX     HA          XXXX         HHHHX"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XXXXXXXXXXX XX     X    XXXXX                  XXXXXX                   X              XXXXXX                 XXXX         XXXAX"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX CXXC    1X C X              X                  XXXXXX           HHHHHH  XXXXX          XXXXXX    XXXX    C    XXXX    C    XXXAX"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XX X    X C X           CC X   XXXXXXXXXXXX   XXXXXX   X       HHHHHH  X   X    X     XXXXXX    XXXX         XXXX         XXAAX"
    "X                                 X   X  XXCX1  1X H X              X   X    HH    X            X                 X            XXXXXX    XXXX                      XXXXX"
    "X    XXX             H    XX          X XXX XHHAAX H X     X    X   X   X    AA    X CCCHHHAAA  X       AAAAAA  X   X          XXXXXX    XXXX                          X"
    "X    X       1 AX         CX        C X XX   XXXXX A       XXXXXX   X   X    CC    X CCCHHHAAA  X       AAAAAA  XXXXX          XXXXXX            XXXX               C  X"
    "X          1   AX     A   X         C X C     C XX A       X            X XXXXXX X X            X               X              XXXXXX     C      XAA2  2      XXXX     X"
    "X            1 CX                 X C XXXXX XXX XX         X  C       C X      X 1 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX              XXXXXX            XHH2   2     XXXX11111X"
    "X   XX     1 HHCX        XXX      X A XC CX X   XXXXXXXXXXXX            XXXXXX X1 1XXXX                                                          XXXX         XXXX     X"
    "X    X   XXXXXXXX          XX     X A X X X X XXX 2  AAX       XX   XXXXXXCCCCCX   XXXX              C                1 1 1 1            XXXX                 XXXX     X"
    "X    X          X                 X A X X X X XXX    XXX    XXXXX   XXXXXXXXXXXX   XXXX                               H H A A            XXXX                          X"
    "X    X     H A                    X H X X X X X    1 HHX     1 HX C      2  HA     XXXX                                                  XXXX                      XXHAX"
    "X                                 X H X X X X X X    XXX 1 X  1HX   X              XXXX      XXXX        XXXX        XXXX        XXXX    XXXXXXXX        XXXX      XXHAX"
    "X         X         X22X          X H  CXC CX   X 2  CCX  1 CCAAX        2  HA     XXXX     X1HH1X      X2AC1X      X2112X      XH21AX      XC   X      X    X     XXHAX"
    "X         XX        XXXX          XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX             XX    A     XX          XX      1   XX          XX          XX XXXXX"
    "X          XX                X2   X       X2                               2XC    CXXXX             XX          XX          XX      0   XX          XX          XX     X"
    "X                 A           X   X  C  X                                   XC 1  CXXXX                  XXXX        XXXX        XXXX        XXXX        XXXX          X"
    "X                           H  X  X  C    X1                               1XC 1   XXXX                 X   CX      X    X      X   CX      X    X      X   CX         X"
    "X     X        XXXXX    X         X  C  XXXXXXXXXXXXXXXXX     XXXXXXXXXXXXX XXXXXX XXXX     XXXXXX              XX          XX          XX          XX          XX     X"
    "X                       X         X  C  XXXXXXXXXXXXXXXXX  A  XXXXXXXXXXXXXC AAHH CXXXX      CC 2X              XX          XX          XX          XX          XX     X"
    "X     H                 X         X  A  XXXXXXXXXXXXXXXXX  A  XXXXXXXXXXXXXXXXXXXXXXXXX         2X                                                                     X"
    "X           XXX                   X  A  XXXXXXXXXXXXXXXXX  H  XXXXXXXXXXXXXXXXXXXXXXXXX          X       C                                                 XXXXXX      X"
    "X                       XX        X  A  XXXXXXXXXXXXXXXXX  H  XXXXXXXXXXXXXXXXXXXXXXXXX          X              XXXXXX           C                         XH1         X"
    "X       X      2X                 X  A  XXXXXXXXXXXXXXXXX  C  XXXXXXXXXXXXXXXXXXXXXXXXX          X              XC                                  C      XH1         X"
    "X       X       X    C            X  H      hXXXXXXXXXXXX  C  XXXXXXXXXXXXXXXXXXXXXXXXX                         X 2 2                      XXXXXX          XH1         X"
    "X                             XXXXX  H       XXXXXXXXXXXX     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX         X             XXXXXX       XH              XA1         X"
    "X   X       XX          X     X   X  H      aXXXXXXXXXXXX                    0        X     AAAAA     X         X 2 2            HAX       XH              XA1         X"
    "X     C                         X    H       XXXXXXXXXXXX   1  X X1X X X X0X X X    X   X   HHHHH     X         X                HAX       XA           XXXXXX    C    X"
    "X                             X   X         wXXXXXXXXXXXX                             X     CCCCC     X                       C  HAX       XA           XH2            X"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XXX                          HAX       X            XH             X"
    "X              XXXXXXXC C C CXC C                              X XC CX     XXXXXXXXXX H A X     X     X                            X                    XA2            X"
    "X              X     X       X                            X    X X X X  A  XXXXXXXXXX H A X     X     X                                                 XA             X"
    "X   S      C         X X     X XX       X                X     X X X X     XXXXXXXXXX H A X     X     X                 XXXXXX                          X        XXXXXXX"
    "X              X     XCX 2   X XX       X                XC    X X X X     XXXXXXXXXX H A X     X  2  X     XXXXXX      X CC X                                   X HA  X"
    "X              X     X XCHAHCX XX                        X     XC CXC     CXXXXXXXXXX H A X 1 1 X     X     X    X      X HH X        XXXXXX                     X HA  X"
    "XXXXXXXXXXXXXXXX  0  X XCHAHCX XX                         X    XXXXXXXXXXXXXXXXXXXXXX H A X     X     X  C  X    X   C  X AA X        X    X         XXXXXX   C  X     X"
    "X        X     X     XCX     X XX       X   X   X   X          X            XXXXXXXXX     X     X     X     X    X      X    X    C   X    X         XHHAAX      X     X"
    "X  CCCC  X  A     H  X Xh a wX XXXXX XXXX   X   X   X    X     X    1       XXXXXXXXX XXX X XXX X XXX X     X    X      X    X        X    X    C    X1111X      X     X"
    "X        X     X     X XXXXXXX XX       X                      X            XXXXXXXXX     X     X     X     X    X      2    2        X    X         X    X            X"
    "X   X A XXXX XXXXXXXXXC   C   CXX  HHH  X            1         XXX     XX      XXXXXXX   XXX   XXX   XX                               X    X         X    X         1  X"
    "X0 0X         XXXXXXXXXXXXXXXXXXX       X                AAX     X      X  CCC Xh a wX                X                                              X    X            X"
    "X   X    0    XXX                  CCC  X         0        X          2 X  CCC                        X                                                             1  X"
    "X   X         XXX       1      XX       X                HHX     X      X  CCC XH A CX                X       000000000000                                             X"
    "XX XXXXXXXXXXXXXX  0 X    X    XX  AAA  X            0         XXX     XX      XXXXXXX   XXX   XXX   XX                          XXXXXX                          C     X"
    "X          X  XXX              XX       X                      X            XXXXXXXXX     X     X     X          XXXXXX          X    X                                X"
    "X HH AA CC X  XXX    X    X    XXXXX XXXX   X   X   X    X     X    1       XXXXXXXXX XXX X XXX X XXX X          X F  X     C    X    X       XXXXXX                   X"
    "X          X  XXX              XX       X   X   X   X          X            XXXXXXXXX     X     X     X          X    X          X    X       X    X       XXXXXX      X"
    "XXXXXXXXX XXXXXXX    X    X    XX                         X    XXXXXXXXXXXXXXXXXXXXXX  0  X     X     X          X 22 X          X1111X    C  X    X       X    X      X"
    "XXX            XX              XX                        X     XXXXXXXXXXXXXXXXXXXXXX     X     X     X          X2  2X          X    X       X    X  1 1  X    X      X"
    "XXX      X      X    X    X    XX       X                XC    XXXXXXXXXXXXXXXXXXXXXX 0 0 X     X 1 0 X          X    X                       X1212X       X    X      X"
    "XXX             X              XX       X                X1    XXXXXXXXXXXXXXXXXXXXXX     X     X     X     C                                 X    X       X    X      X"
    "XXX             X    X    X 0  XX       0         1       X    XXXXXXXXXXXXXXXXXXXXXX     X     X     X                                                    X    X      X"
    "XXX             X              XX                              XXXXXXXXXXXXXXXXXXXXXX CCC X     X AAA X                    XXXXXX                                      X"
    "XXX      1      X              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX     XXXXXXX    XXXXXX          X HH X                  XXXXXXX       XXXXXXX"
    "XXXXX         XXX              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXCCCCCC     XXXXXXX    X AA X          X    X                  XC   CX       X     X"
    "XXX           XXX              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    X2  2X     C    X1  1X         C        X     X   C   X  C  X"
    "XXX  XXXXXXXXXXXX              XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    X    X          X    X                  X     X       X  A  X"
    "XXX                            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    X    X          X    X                  X     X       X  H  X"
    "XXX  C H A H C                 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX    X    X                                  X     X       X     X"
    "XXX                            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                                                                X"
    "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
};

const int Level::levelWidth = 168;
const size_t Level::levelSize = strlen(levelData);





bool Level::isBlockAtPos(int x, int y) {
    int index = x + y * levelWidth;
    if (index > -1 && index < levelSize)
        return (levelData[index] == symbolWall);

    return false;
}



void Level::setupAllEnemiesAndPickups(SDL_Renderer* renderer, Vector2D& posStart, Vector2D& posFinish, 
    std::vector<std::shared_ptr<UnitEnemy>>& listUnitEnemies, std::vector<std::shared_ptr<Pickup>>& listPickups) {
    for (int index = 0; index < levelSize; index++) {
        //The current position in the level in Vector2D form.
        Vector2D pos(
            (float)(index % levelWidth) + 0.5f,
            (float)(index / levelWidth) + 0.5f);

        //Check if any of the following symbols are found and modify the input positions and lists as required.
        switch (levelData[index]) {
        case symbolStart:
            posStart = pos;
            break;
        case symbolFinish:
            posFinish = pos;
            break;

        case symbolEnemySmall:
            UnitEnemy::addUnitEnemyToListUsingTemplate(renderer, pos, 0, listUnitEnemies);
            break;
        case symbolEnemyMedium:
            UnitEnemy::addUnitEnemyToListUsingTemplate(renderer, pos, 1, listUnitEnemies);
            break;
        case symbolEnemyLarge:
            UnitEnemy::addUnitEnemyToListUsingTemplate(renderer, pos, 2, listUnitEnemies);
            break;

        case symbolHealth:
            listPickups.push_back(std::make_shared<Health>(renderer, pos));
            break;
        case symbolCoin:
            listPickups.push_back(std::make_shared<Coin>(renderer, pos));
            break;
        case symbolAmmo:
            listPickups.push_back(std::make_shared<Ammo>(renderer, pos));
            break;

        case symbolUpgradeHealth:
            listPickups.push_back(std::make_shared<UpgradeHealth>(renderer, pos));
            break;
        case symbolUpgradeAmmo:
            listPickups.push_back(std::make_shared<UpgradeAmmo>(renderer, pos));
            break;
        case symbolUpgradeWeaponSpeed:
            listPickups.push_back(std::make_shared<UpgradeWeaponSpeed>(renderer, pos));
            break;
        }
    }
}