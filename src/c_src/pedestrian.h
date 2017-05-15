#include <vector>

class Pedestrian{
  public:
    Pedestrian(string);
    void run(const int&);
  private:
    vector<int> path;
    PathFinder path_finder;
}
