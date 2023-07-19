#include "Crystal.h"

void Crystal::AddToCrystal(Vector2* coord){
    currentCrystalSize++;
//    _crystalArray[GetIndex(coord)] = currentCrystalSize;
    _crystalArray[GetIndex(coord)] = 255;

    Crystal::MarkSurroundingPoints(coord);
}

// marks the surrounding points of the given coord as -1 so that new crystals can be added here if the random walk lands on a -1
void Crystal::MarkSurroundingPoints(Vector2* coord) {
    // sets the surrounding 8 points in the array to -1 if they are not greater than 0 (not already in the crystal)
    int directions[] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, -1, -1, 1, -1, -1, 1};
    for (int i = 0; i < 16; i+=2) {
        int newX = coord->x + directions[i];
        int newY = coord->y + directions[i+1];

        Vector2 newCoord = {newX, newY};
        WrapCoordinates(&newCoord);

        if (_crystalArray[GetIndex(&newCoord)] == 0) {
            _crystalArray[GetIndex(&newCoord)] = 1;
        }
    }
}

// check the given points value in the crystal array. If it is a -1 add that point to the crystal and return true
bool Crystal::EvaluatePoint(Vector2* coord){
    // if the point is marked as -1 then add it to the crystal and return true
    if (_crystalArray[GetIndex(coord)] == 1){
        AddToCrystal(coord);
        UpdateRadius(coord);
        return true;
    }
    // if this point was not marked -1 then return false
    else return false;
}

// returns the distance between two points
int GetDistance(Vector2* coord1, Vector2* coord2){
    return (int) sqrt(pow(coord1->x - coord2->x, 2) + pow(coord1->y - coord2->y, 2));
}

// This function updates the spawn radius for new crystal points based on the newly added point's location
void Crystal::UpdateRadius(Vector2* coord){
    // if the point is outside the current radius then update the radius
    int distance = GetDistance(coord, &_seedLocation);
    if (distance > currentSpawnRadius){
        if (distance > ((std::min(_width, _height)/2) - spawnBuffer)){// if it's out of bounds
            currentSpawnRadius = ((std::min(_width, _height)/2) - spawnBuffer);
            // maximum extent of radius achieved. Stop growing???
        }
        else{
            currentSpawnRadius = distance;
        }
    }
}

// perform a series of random steps from a random starting point until the walk collides with a point in the crystal
void Crystal::RandomWalk(){
    if ((currentCrystalSize >= maxCrystalSize)){
        isGrowing = false;
        CrystalComplete();
    }

    if (!isGrowing) return;

    bool walkComplete = false;

    // start the walk at a random point on a circle around the seed
    _currentWalkPosition = GetRandomPointOnCircle(currentSpawnRadius + spawnBuffer, &_seedLocation);

    while (!walkComplete){
        RandomStep(&_currentWalkPosition);
        WrapCoordinates(&_currentWalkPosition, (currentSpawnRadius + (spawnBuffer / 2)));
        walkComplete = EvaluatePoint(&_currentWalkPosition);
    }
}

// blocking function that grows the crystal until it reaches the desired size
void Crystal::BeginGrowth() {
    StartTimer();
    while(currentCrystalSize < maxCrystalSize){
        RandomWalk();
    }
    CrystalComplete();
}

// sets the isComplete flag and calculates the time taken since _startTime
void Crystal::CrystalComplete(){
    isComplete = true;
    auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    timeTaken =  (float) (endTime.count() - _startTime.count()) / 1000.0f;
}

void Crystal::RandomStep(Vector2 *position){
    int randomDirection = getRandomInt(0, 3);

    switch(randomDirection){
        case 0:
            position->x -= 1;
            return;
        case 1:
            position->x += 1;
            return;
        case 2:
            position->y -= 1;
            return;
        case 3:
            position->y += 1;
            return;
        default:
            return;
    }
}

Vector2 Crystal::GetRandomPointOnCircle(int radius, Vector2 *centre) {

    radius = std::min({radius, _width, _height});

    double randomAngle = getRandomNumber(0.0, 2.0 * 3.14159265359);

    int x = (int)(std::cos(randomAngle) * radius) + centre->x;
    int y = (int)(std::sin(randomAngle) * radius) + centre->x;

    return {x, y};
}

int Crystal::getRandomInt(int lowerBound, int upperBound) {
    std::uniform_int_distribution<int> dis(lowerBound, upperBound);
    return dis(_gen);
}

double Crystal::getRandomNumber(double lowerBound, double upperBound) {
    std::uniform_real_distribution<double> dis(lowerBound, upperBound);
    return dis(_gen);
}

void Crystal::WrapCoordinates(Vector2 *coord, int radius) const{

    // get the coord relative to the seed
    int newX = coord->x - _seedLocation.x;
    int newY = coord->y - _seedLocation.y;

    // pull the coordinate back into the wrapping envelope if it's off the negative side
    if (newX < -radius) newX += radius * 2;
    if (newY < -radius) newY += radius * 2;

    // wrap the coord back into the envelope from the positive side
    if (newX > radius) newX -= radius * 2;
    if (newY > radius) newY -= radius * 2;

    // make the location non-relative and add the seed location back on to get the real position in the array
    coord->x = newX + _seedLocation.x;
    coord->y = newY + _seedLocation.y;
}

void Crystal::WrapCoordinates(Vector2 *coord) const{
    if (coord->x < 0) coord->x += _width;
    if (coord->y < 0) coord->y += _height;

    if (coord->x >= _width) coord->x -= _width;
    if (coord->y >= _height) coord->y -= _height;

}

Crystal::Crystal(Vector2 size) {
    // init variables
    timeTaken = 0;
    currentCrystalSize = 0;
    currentSpawnRadius = 1;
    spawnBuffer = 10;
    maxCrystalSize = 1000;

    _width = size.x;
    _height = size.y;

    _seedLocation = {size.x / 2, size.y / 2};

    // a 1D array that contains the crystal
    _crystalArray = std::vector<unsigned char>(_width * _height, 0);
    std::fill(_crystalArray.begin(), _crystalArray.end(), 0);

    // rng engine
    _gen = std::mt19937(std::random_device{}());

    // add initial seed to the crystal
    AddToCrystal(&_seedLocation);

};
