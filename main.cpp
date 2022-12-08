
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
    double len2() const{
        return x*x + y*y;
    }
};

std::vector<Point> points, UpP, DownP;

Point operator+ (Point a, Point b){
    return {a.x+b.x,a.y+b.y};
}

Point operator- (Point a, Point b){
    return {a.x-b.x,a.y-b.y};
}

double PolarPhi(Point a){
    return atan2(1.0*a.y,1.0*a.x);
}

int f(Point a, Point b)
{
    if (abs(PolarPhi(a) - PolarPhi(b)) == 0)
        return a.len2() < b.len2();
    return PolarPhi(a) < PolarPhi(b);
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

float scale_val(float a, float b, float c, float d, float t) {
    // [a,b] => [c,d]
    return (t-a)/(b-a)*(d-c) + c;
}

void drawFuncGraph(float a, float b, float c, float d, float (*func)(float x)) {
    float x = a;
    float y;
    float dh = (b-a)/500;

    float x1 = scale_val(a, b, 0, w, x);

    y=func(x);
    float y1 = scale_val(c, d, 0, h, y);

    x += dh;
    while (x <= b) {
        float x2 = scale_val(a, b, 0, w, x);
        y=func(x);
        float y2 = scale_val(c, d, 0, h, y);

        drawLine(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;

        x += dh;
    }
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

vector<Point> upper_points(Point a, Point b, vector<Point> points, int n) {
    int upcounter = 0;
    std::vector<Point> result;
    for (int i = 0; i < n; ++i){
        if (LeftRotate(a,b,points[i])){
            result.push_back({points[i].x, points[i].y});
            upcounter += 1;
        }
    }
    return result;
}

vector<Point> down_points(Point a, Point b, vector<Point> points, int n) {
    int downcounter = 0;
    std::vector<Point> result;
    for (int i = 0; i < n; ++i){
        if (LeftRotate(b,a,points[i])){
            result.push_back({points[i].x, points[i].y});
            downcounter += 1;
        }
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
    ++n;

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

float test_func(float x) {
    return sin(2*x*x);
}

int main()
{
    int n=20, hull, upcounter, downcounter;
    Point NullPoint, c, LeftP, RightP, UpHull, DownHull;
    vector<Point> HULL;

    //float v = 0.01;

    //float y = 0;

    points = scatter_points(n);
/*
    LeftP = points[1];
    RightP = points[2];

    for (int i=0; i < n; ++i){
        if (points[i].x < LeftP.x){
            LeftP = points[i];
        }
        else if (points[i].x > RightP.x){
            RightP = points[i];
        }
    }

    UpP = upper_points(LeftP, RightP, points, n);
    DownP = down_points(LeftP, RightP, points, n);
*/

   HULL = Graham(points, n);

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

   //     drawLine(0, y, w, y);

    //    drawFuncGraph(-5,5, -2, 2, test_func);

        for(Point p : points) {
            drawPoint(p.x, p.y);
        }

       for (int i=0; i < n; ++i){
           drawLine(HULL[i].x, HULL[i].y, HULL[i+1].x, HULL[i+1].y);
       }

        //c = points[1]+points[2];
        //drawPoint( c.x/2, c.y/2);
/*
       UpHull = RightP;
       for (int i=0; i < upcounter; ++i){
           int counter = 0;
           for (int j=0; j < upcounter; ++j){
               if (not(LeftRotate(UpHull, UpP[i], UpP[j]))){
                   break;
               }
               counter += 1;
           }
           if (counter = upcounter){
               drawLine(UpHull.x, UpHull.y, UpP[i].x, UpP[i].y);
               UpHull = UpP[i];
           }
       }
*/
        window.display();

    //    y += 0.01;
    }

    return 0;
}
 /*
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
*/