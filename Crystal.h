#pragma once
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>

struct Vector2{
    int x, y;
};

class Crystal{
    //public methods
public:
    // constructor
    explicit Crystal(Vector2 size);
    // destructor
    ~Crystal()= default;
    // start the timer for the crystal growth
    void StartTimer() {_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());}
    void CrystalComplete();
    void RandomWalk();
    void BeginGrowth();

    // public variables
public:
    int currentCrystalSize;
    int maxCrystalSize;
    int currentSpawnRadius;
    int spawnBuffer;
    float timeTaken;
    bool isComplete = false;
    bool isGrowing = false;
    int _width, _height;
    std::vector<unsigned char> _crystalArray;

    // private variables
private:
    Vector2 _seedLocation{};
    Vector2 _currentWalkPosition{};
    std::chrono::milliseconds _startTime{};
    std::mt19937 _gen;

    //private methods
private:
    // GetIndex returns the index of the 1D array from the 2D array coordinates
    int GetIndex(Vector2* coord) const { return coord->x + (coord->y * _width); }
    // GetXY returns the 2D array coordinates from the 1D array index
    [[nodiscard]] Vector2 GetXY(int index) const { return  Vector2{index % _width, index / _width};}
    // wrap the coordinates around the edges of the array
    void WrapCoordinates(Vector2* coord) const;
    // overload of wrap coordinates that takes a square wrapping envelope of side length radius*2 centred on the seed
    void WrapCoordinates(Vector2* coord, int radius) const;
    //TODO swap this to float?!? make fast!?
    double getRandomNumber(double lowerBound, double upperBound);
    int getRandomInt(int lowerBound, int upperBound);
    // get a random point on a circle of given radius centred on the given centre
    Vector2 GetRandomPointOnCircle(int radius, Vector2* centre);
    // move the given vector2 one unit in a random direction (up, down, left, right)
    void RandomStep(Vector2* position);
    void AddToCrystal(Vector2* coord);
    void MarkSurroundingPoints(Vector2* coord);
    bool EvaluatePoint(Vector2* coord);
    void UpdateRadius(Vector2* coord);
};
