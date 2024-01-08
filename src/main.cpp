#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <wex.h>                // https://github.com/JamesBremner/windex/blob/master/include/wex.h
#include "cStarterGUI.h"
#include "cxy.h"                // https://github.com/JamesBremner/raven-set/blob/master/cxy.h

class cWrap
{
public:
    void generate1();

    // https://en.wikipedia.org/wiki/Gift_wrapping_algorithm
    void wrap();

    void draw( wex::shapes& S);

    void text();
    void test();

private:
    std::vector<cxy> vPoints;
    std::vector<cxy> vWrap;
};

void cWrap::test()
{
    double a = cxy::clockwise(
        cxy(0, -1),
        cxy(0, 0),
        cxy(10, 0));
}
void cWrap::generate1()
{

    vPoints = {{0, 10},
               {0, 0},
               {3, 3},
               {5, 1},
               {10, 10},
               {10, 0}};
}

void cWrap::wrap()
{
    vWrap.clear();

    // find leftmost point
    cxy leftmost = vPoints[0];
    for (cxy &p : vPoints)
        if (p.x < leftmost.x)
            leftmost = p;
    vWrap.push_back(leftmost);

    // wrap around points
    cxy prev = cxy(leftmost.x, leftmost.y - 1);
    cxy last = leftmost;
    for (;;)
    {
        // loop over points
        cxy next = vPoints[0];
        double amin = 10;
        for (cxy &maybe : vPoints)
        {

            double a =
                2 * 3.142 - cxy::clockwise(
                                prev,
                                last,
                                maybe);

            // std::cout
            //         << "prev " << prev.x <<" "<< prev.y
            //         << " last " << last.x << " " << last.y
            //         << " maybe " << maybe.x << " " << maybe.y
            //         << " a " << a << "\n";

            if (a < 0.01)
                continue;

            // save point with least clockwise turn
            if (a < amin)
            {
                amin = a;
                next = maybe;
            }
        }
        // check if back at start
        if (next == leftmost)
            return;

        // save point
        vWrap.push_back(next);
        prev = last;
        last = next;
        // std::cout << "\twrap " << next.x << " " << next.y << "\n";
    }
}
void cWrap::text()
{
    for (cxy &p : vWrap)
        std::cout << p.x << " " << p.y << ", ";
    std::cout << "\n";
}
void cWrap::draw( wex::shapes& S)
{
    int scale = 20;
    int xoff = 10;
    int yoff = 210;
    for( cxy& p : vPoints )
    {
        S.circle(xoff+scale * p.x,yoff-scale * p.y,5);
    }
    cxy prev = vWrap.back();
    for( cxy& p : vWrap )
    {
        S.line({
            xoff+(int)scale*prev.x,yoff-(int)scale*prev.y,
            xoff+(int)scale*p.x,yoff-(int)scale*p.y});
        prev = p;
    }
}
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          lb(wex::maker::make<wex::label>(fm))
    {
        wrap.generate1();
        wrap.wrap();
        wrap.text();

        fm.events().draw([&](PAINTSTRUCT &ps)
                         {
            wex::shapes S( ps );
            wrap.draw( S ); });

        show();
        run();
    }

private:
    wex::label &lb;

    cWrap wrap;
};

main()
{
    cGUI theGUI;
    return 0;
}
