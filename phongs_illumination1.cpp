// This code is used to calculate the intensity value for a particular point by
// phongs illumination model
#include <bits/stdc++.h>
using namespace std;

void Cross_Product(vector<double>& AB, vector<double>& AC, vector<double>& N_Temp){

    N_Temp[0] = AB[1] * AC[2] - AB[2] * AC[1]; 
    N_Temp[1] = AB[2] * AC[0] - AB[0] * AC[2]; 
    N_Temp[2] = AB[0] * AC[1] - AB[1] * AC[0];

}

double Dot_Product(vector<double>& A, vector<double>& B){
    double dot = 0;
    for(int i=0; i<3; i++) dot += (A[i]*B[i]);
    return dot;
}

void Normalise(vector<double>& A){
    double mod = A[0]*A[0] + A[1]*A[1] + A[2]*A[2];
    mod = sqrt(mod);

    A[0] = A[0]/mod;
    A[1] = A[1]/mod;
    A[2] = A[2]/mod;
}

int main(){
    // Enter the Number Of Surfaces That Contribute To This Point Intensity
    double N = 1;

    vector<double> A(3,0);

    // Enter Point X, Y, Z-Coordinate
    A[0] = 45/3; A[1]=55/3; A[2]=25/3;

    vector<double> Eye(3,0);
    // Enter Eye Coordinates
    Eye[0] = 0; Eye[1]=0; Eye[2]=0;

    vector<double> Light_S(3,0);
    // Enter Light_S Coordinates
    Light_S[0] = -5; Light_S[1]=-5; Light_S[2]=-5;

    double K_a = 0.5;
    double K_d = 0.8;
    double K_s = 0.3;
    double I_p = 100;
    double I_amb = 10;
    double F_att = 0.5;
    double Ref_Exponent = 10;


    vector<vector<double>> Normals;

    // For Each Surface We Want Normal To That Surface
    for(int i=0; i<N; i++){
        // Now For Each Surface We Take Two Points On That Surface B & C
        // Such That (vector AB) * (vector AC) Gives The Outward vector
        // We Take AB * AC, So Enter Points B & C accordingly.

        vector<double> B(3,0);
        vector<double> C(3,0);

        for(int i=0; i<3; i++) cin>>B[i];
        for(int i=0; i<3; i++) cin>>C[i];

        vector<double> AB(3,0);
        AB[0] = B[0] - A[0];
        AB[1] = B[1] - A[1];
        AB[2] = B[2] - A[2];

        vector<double> AC(3,0);
        AC[0] = C[0] - A[0];
        AC[1] = C[1] - A[1];
        AC[2] = C[2] - A[2];

        // We take AB * AC
        vector<double> N_Temp(3,0);
        Cross_Product(AB, AC,N_Temp);
        Normalise(N_Temp);
        Normals.push_back(N_Temp);
    }

    vector<double> N_Final(3,0);

    for(int i=0; i<N; i++){
        N_Final[0] += Normals[i][0];
        N_Final[1] += Normals[i][1];
        N_Final[2] += Normals[i][2];
    }

    Normalise(N_Final);

    // Now Calulate R vector
    
    // Step 1 - Calculate Light Vector & View Vector
    Light_S[0] = Light_S[0] - A[0];
    Light_S[1] = Light_S[1] - A[1];
    Light_S[2] = Light_S[2] - A[2];

    Eye[0] = Eye[0] - A[0];
    Eye[1] = Eye[1] - A[1];
    Eye[2] = Eye[2] - A[2];


    Normalise(Light_S);
    Normalise(Eye);

    cout<<"N Vector => "<<N_Final[0]<<" "<<N_Final[1]<<" "<<N_Final[2]<<endl;
    cout<<"L Vector => "<<Light_S[0]<<" "<<Light_S[1]<<" "<<Light_S[2]<<endl;
    cout<<"V Vector => "<<Eye[0]<<" "<<Eye[1]<<" "<<Eye[2]<<endl;

    vector<double> R(3,0);
    cout<<" R = 2N(N . L) - L"<<endl;

    double dot = Dot_Product(N_Final, Light_S);
    for(int i=0; i<3; i++){
        R[i] = (2*N_Final[i]*dot) - Light_S[i];
    }

    Normalise(R);

    cout<<"R Vector => "<<R[0]<<" "<<R[1]<<" "<<R[2]<<endl;

    cout<<"Intensity Is Given By"<<endl;

    cout<<"I = I_a*K_a + F_att*I_p*K_d*(N.L) + F_att*I_p*K_s*(R.V)^n"<<endl;

    double ambient_I = I_amb*K_a;
    cout<<"Part 1 : I_amb = I_a*K_a = "<<ambient_I<<endl;

    double difussed_I = F_att * I_p * K_d * Dot_Product(N_Final, Light_S);
    cout<<"Part 2 : I_difussed = F_att*I_p*K_d*(N.L) = "<<difussed_I<<endl;

    double specular_I  = F_att*I_p*K_s;
    double tt = Dot_Product(R,Eye);
    tt = pow(tt, Ref_Exponent);
    specular_I = specular_I * tt;

    cout<<"Part 3 : I_specular =  F_att*I_p*K_s*(R.V)^n = "<<specular_I<<endl;

    double total_I = ambient_I + difussed_I + specular_I;
    cout<<"Total Intensity  = "<<total_I<<endl;
}
