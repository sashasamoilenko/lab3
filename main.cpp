
#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>

using namespace std;

float w = 500;
float h = 500;

sf::RenderWindow window(sf::VideoMode(w, h), "Hull");

class Point {
public:
    float x;
    float y;
    double rho() const{
        return x*x + y*y;
    }
};

std::vector<Point> points;

Point operator+ (Point a, Point b){
    return {a.x+b.x,a.y+b.y};
}

Point operator- (Point a, Point b){
    return {a.x-b.x,a.y-b.y};
}

double PolarPhi(Point a){
    return atan2(1.0*a.y,1.0*a.x);
}

bool f(Point a, Point b)
{
    if (abs(PolarPhi(a) - PolarPhi(b)) == 0)
        return a.rho() < b.rho();
    return PolarPhi(a) < PolarPhi(b);
}

bool x_coord(Point a, Point b)
{
    return a.x < b.x;
}

bool operator != (Point a, Point b)
{
    return !(a.x == b.x && a.y == b.y);
}

bool LeftRotate(Point a, Point b, Point c)
{
    Point q = b - a, w = c - b;
    return q.x * w.y > w.x * q.y;
}

void drawPoint(float x, float y) {
    sf::CircleShape shape(3.f);
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setPosition(x,y);
    window.draw(shape);
}

void drawLine(float x1, float y1, float x2, float y2) {
    sf::Vertex line[] =
            {
                    sf::Vertex(sf::Vector2f(x1, y1)),
                    sf::Vertex(sf::Vector2f(x2, y2))
            };

    window.draw(line, 2, sf::Lines);
}

vector<Point> scatter_points(int count) {
    std::vector<Point> result;
    for (int i = 0; i < count; ++i) {
        float x = ((float)rand()) / RAND_MAX * w;
        float y = ((float)rand()) / RAND_MAX * h;
        result.push_back({x,y});
    }
    return result;
}

vector<Point> Graham(vector<Point> points, int n){
    Point NullPoint;
    int hull;
    std::vector<Point> result;

    for(int i = 1; i < n; ++i)
    {
        if (points[i].x < points[0].x){
            swap(points[i],points[0]);
        }
        if ((points[i].x == points[0].x) && (points[i].y < points[0].y)){
            swap(points[i],points[0]);
        }
    }

    NullPoint = points[0];
    for(int i = 0; i < n; ++i){
        points[i] = points[i] - NullPoint;
    }

    sort(points.begin()+1,points.end(),f);
    points.push_back(points[0]);

    hull = 1;
    for(int i = 2; i < n; ++i){
        while( (hull > 0) && not(LeftRotate(points[hull-1],points[hull],points[i]))) {
            hull -= 1;
        }
        hull += 1;
        points[hull] = points[i];
    }

    for(int i = 0; i <= hull; ++i){
        points[i] = points[i] + NullPoint;
        result.push_back({points[i].x, points[i].y});
    }
    return result;
}

vector<Point> Endr_Jarv(vector<Point> points, int n) {
    Point hull;
    int number;
    std::vector<Point> up_points, down_points, result;

    sort(points.begin(),points.end(),x_coord);

    down_points.push_back({points[0].x, points[0].y});
    up_points.push_back({points[n-1].x, points[n-1].y});
    for (int i=1; i < n-1; ++i){
        if (LeftRotate(points[0],points[n-1],points[i])){
            down_points.push_back({points[i].x, points[i].y});
        }
        else up_points.push_back({points[i].x, points[i].y});
    }
    down_points.push_back({points[n-1].x, points[n-1].y});
    up_points.push_back({points[0].x, points[0].y});

    result.push_back({points[0].x, points[0].y});
    hull = down_points[0];
    number = 0;

    while (hull != points[n-1]){
         int next = (number + 1) % up_points.size();
         for (int i=0; i < down_points.size(); ++i){
             if ((LeftRotate(down_points[number],down_points[next],down_points[i]))){
                 next = i;
             }
         }
        number = next;
        hull = down_points[next];
        result.push_back({down_points[next].x, down_points[next].y});
    }

    hull = up_points[n-1];
    number = 0;

    while (hull != points[0]){
        int next = (number + 1) % up_points.size();
        for (int i=0; i < up_points.size(); ++i){
            if ((LeftRotate(up_points[number],up_points[next],up_points[i]))){
                next = i;
            }
        }
        number = next;
        hull = up_points[next];
        result.push_back({up_points[next].x, up_points[next].y});
    }

    return result;
}


int main()
{
    int n=30;
    vector<Point> HULL;

    points = scatter_points(n);

     //HULL = Graham(points, n);
    HULL = Endr_Jarv(points, n);

   while (window.isOpen())
   {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }
        window.clear();

        for(Point p : points) {
            drawPoint(p.x, p.y);
        }

       for (int i=0; i < n; ++i){
           drawLine(HULL[i].x, HULL[i].y, HULL[i+1].x, HULL[i+1].y);
       }

       window.display();

    }

    return 0;
}
