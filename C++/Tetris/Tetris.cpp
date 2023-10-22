#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>
#include <windows.h>

using namespace std;

const int width = 12;
const int height = 20;
bool gameOver;
int score;
int x, y;
int nScreenWidth = 80;
int nScreenHeight = 30;
vector<int> tetromino;
wstring tetrominos[7];

int Rotate(int px, int py, int r)
{
    int pi = 0;
    switch (r % 4)
    {
        case 0: // 0 degrees
            pi = py * 4 + px;
            break;

        case 1: // 90 degrees
            pi = 12 + py - (px * 4);
            break;

        case 2: // 180 degrees
            pi = 15 - (py * 4) - px;
            break;

        case 3: // 270 degrees
            pi = 3 - py + (px * 4);
            break;
    }

    return pi;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    // All Field cells >0 are occupied
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            // Get index into piece
            int pi = Rotate(px, py, nRotation);

            // Get index into field
            int fi = (nPosY + py) * width + (nPosX + px);

            // Check that test is in bounds. Note out of bounds does
            // is allowed, it is sometimes convenient to use "oob" 
            // piece to check for bounclary conditions
            if (nPosX + px >= 0 && nPosX + px < width)
            {
                if (nPosY + py >= 0 && nPosY + py < height)
                {
                    // In Bounds so do collision check
                    if (tetrominos[nTetromino][pi] != L'.' && tetrominos[nTetromino][pi] != 0 && field[fi] != 0)
                        return false; // fail on first hit
                }
            }
        }
    }

    return true;
}

int main()
{
    // Create Screen Buffer
    wstring screen;
    screen.resize(nScreenWidth * nScreenHeight);

    for (int x = 0; x < nScreenWidth; x++)
        for (int y = 0; y < nScreenHeight; y++)
            screen[y * nScreenWidth + x] = L' ';

    tetrominos[0].append(L"..X...X...X...X.");
    tetrominos[1].append(L"..X..XX...X.....");
    tetrominos[2].append(L".....XX..XX.....");
    tetrominos[3].append(L"..X..XX..X......");
    tetrominos[4].append(L".X...XX...X.....");
    tetrominos[5].append(L".X...X...XX.....");
    tetrominos[6].append(L"..X...X..XX.....");

    // Game Logic
    int nCurrentPiece = 0;
    int nCurrentRotation = 0;
    int nCurrentX = width / 2;
    int nCurrentY = 0;

    bool bKey[4];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCounter = 0;
    bool bForceDown = false;

    score = 0;

    gameOver = false;

    while (!gameOver)
    {
        // Timing =======================
        this_thread::sleep_for(50ms); // Small Step = 1 Game Tick
        nSpeedCounter++;
        bForceDown = (nSpeedCounter == nSpeed);

        // Input ========================
        for (int k = 0; k < 4; k++)
        {
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[k]))) != 0;
        }

        // Game Logic ===================
        // Handle player movement
        nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
        nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
        nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

        // Rotate, but latch to stop wild spinning
        if (bKey[3])
        {
            nCurrentRotation += (bRotateHold ? 0 : 1);
            bRotateHold = true;
        }
        else
        {
            bRotateHold = false;
        }

        // Force the piece down the playfield if it's time
        if (bForceDown)
        {
            // Update difficulty every 50 pieces
            if (score % 50 == 0)
            {
                if (nSpeed >= 10)
                    nSpeed--;
            }

            // Test if piece can be moved down
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                // It can, so update screen
                nCurrentY++;
            }
            else
            {
                // It can't! Lock the piece in place
                for (int px = 0; px < 4; px++)
                {
                    for (int py = 0; py < 4; py++)
                    {
                        if (tetrominos[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                            screen[(nCurrentY + py) * nScreenWidth + (nCurrentX + px)] = L'X';
                    }
                }

                // Check for line clearance
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < height - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < width - 1; px++)
                            bLine &= (screen[(nCurrentY + py) * nScreenWidth + px]) != L' ';

                        if (bLine)
                        {
                            // Remove Line, set to =
                            for (int px = 1; px < width - 1; px++)
                                screen[(nCurrentY + py) * nScreenWidth + px] = L'=';
                        }
                    }
                }

                score += 25;
                if (score % 50 == 0)
                    nSpeed++;

                // Pick New Piece
                nCurrentX = width / 2;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = rand() % 7;

                // If piece can't fit, game over
                gameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }

            nSpeedCounter = 0;
        }

        // Display ========================
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[screen[(y + 2) * nScreenWidth + (x + 2)]];

        // Draw Field
        for (int x = 0; x < width; x++)
            for (int y = 0; y < height; y++)
                screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[field[y * width + x]];

        // Draw Current Piece
        for (int px = 0; px < 4; px++)
        {
            for (int py = 0; py < 4; py++)
            {
                if (tetrominos[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = L'A';
            }
        }

        // Display Frame
        wprintf(&screen[0]);
        wprintf(L"\nScore:%8d", score);
    }

    wprintf(L"\nGame Over! Score: %d\n", score);

    system("pause");
    return 0;
}
