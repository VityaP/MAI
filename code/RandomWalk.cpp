#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;


int main(){

    srand(time(NULL));

    for(int steps = 0; steps < 30; ++steps){

        long long greaterThenFive = 0;
        long double percent = 0.0;
        
        int maxMonteCarloIteration = 1000000;
        for(int iterMonteCarlo = 0; iterMonteCarlo < maxMonteCarloIteration; ++iterMonteCarlo){
            long long x = 0;
            long long y = 0;
            long long dx, dy;
            for(int i = 0; i < steps; ++i){
                int direction = rand() % 4;
                if( direction == 0 ){
                    dx = 0;
                    dy = -1;
                }
                if( direction == 1 ){
                    dx = 0;
                    dy = 1;
                }
                if( direction == 2 ){
                    dx = -1;
                    dy = 0;
                }
                if( direction == 3 ){
                    dx = 1;
                    dy = 0;
                }
                x += dx;
                y += dy;
            }
            if( abs(x) + abs(y) > 5 ){
                ++greaterThenFive;
            }
        }
        percent = (long double) 100 * greaterThenFive / maxMonteCarloIteration;
        cout << "for " << steps << " steps percent is " << percent << "\n";
    }

    return 0;
}