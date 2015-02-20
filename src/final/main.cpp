#include <cstdio>
#include "modele.h"

void print_vec(vector<double> v)
{
    for(double a: v)
        printf("%lf\n", a);
}

int main()
{
    modele g(1, 1);
    vector<double> a,b;
    a.push_back(0.3);
    b.push_back(-0.8);
    t_exemple e = make_pair(a,-0.8), f = make_pair(b,0.4);
    vector<t_exemple> ens;
    ens.push_back(e);
    //ens.push_back(f);
    
    printf("%lf %lf\n", g.poids[0][0], g.poids[1][0]); 
    printf("%lf\n", g.cout(ens));
    cout << g.grad_hess_cout(ens).first;
    g.optimise(20,ens);
    printf("Final %lf %lf %lf\n", g.poids[0][0], g.poids[1][0], g.cout(ens)); 
    cout << g.grad_hess_cout(ens).first;
  
  // determinant
  
  // inverse
    //mat B = inv(A); 
}
