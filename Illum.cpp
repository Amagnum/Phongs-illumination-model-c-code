// 	Sirusbeck algo

#include <bits/stdc++.h>
#include <Eigen>

/*
g++ -I ./lib/eigen-3.3.9/Eigen Illum.cpp -o eill
./eill < ill.in
*/

using namespace Eigen;
using namespace std;


double ambIllum(double Kamb, double Iamb){
	double ans = Kamb*Iamb;
	cout<<"Ambient Illum: "<<ans<<endl;
	return ans;
}


double diffIllum(double Kd, double Ip,double fatt, Vector3d nor, Vector3d l){
	nor.normalize();
	l.normalize();
	double n_l = nor.dot(l);
	
	double ans = fatt*Kd*Ip*n_l;
	cout<<"Diff Illum: "<<ans<<endl;
	return ans;
}

double phongIllum(double Iamb,double Kamb, double Ip, double Kd,double Ks,double fatt, double refC, Vector3d nor, Vector3d l,Vector3d v){
	nor.normalize();
	l.normalize();
	v.normalize();
	
	cout<<"Normal :"<<nor.transpose()<<endl;
	cout<<"Light :"<<l.transpose()<<endl;
	cout<<"Eye :"<<v.transpose()<<endl<<endl;


	double n_l = nor.dot(l);
	double r_v = (2*nor*n_l-l).dot(v);
	
	double spec = fatt*Ip*Ks*pow(r_v,refC);
	double Iph = ambIllum(Kamb,Iamb) + diffIllum(Kd,Ip,fatt,nor,l) + spec;
	cout<<"Spec Illum: "<<spec<<endl;
	cout<<"Phong Illum: "<<Iph<<endl;
	return Iph;
}


int main(){
	
	int N=3;
	string dummy;
    double x,y,z;
    
    Vector3d light;
    cin >> dummy >> x >> y >> z;
    assert(dummy=="light");
    light << x, y, z;
    cout << "Light :"<<light.transpose()<<endl;

    Vector3d eye;
    cin >> dummy >> x >> y >> z;
    assert(dummy=="eye");
    eye << x, y, z;
    cout << "EYE :"<<eye.transpose()<<endl<<endl;

    double Iamb,Kamb,Ip,Kd, Ks, fatt, refC;

    cin>> Iamb >> Kamb >> Ip >> Kd >> Ks >> fatt >> refC;
    cout<<"Iamb,Kamb,Ip,Kd, Ks, fatt, refC :\n"
    << Iamb <<", "<< Kamb <<", "<< Ip <<", "<< Kd <<", "<< Ks <<", "<< fatt <<", "<< refC<<endl<<endl;

    cin >> dummy;
    assert(dummy=="poly");

    vector<Vector3d> poly(N);
    for (int i = 0; i < N; ++i)
    {
    	cin >> x >> y >> z;
    	poly[i](0) = x;
    	poly[i](1) = y;
    	poly[i](2) = z;
    }

    Vector3d centroid;
    centroid << 0,0,0;
    for (int i = 0; i < N; ++i)
    {
    	centroid = centroid + poly[i]; 
    }
    centroid = centroid/3;


    for (int i = 0; i < N; ++i)
    {
    	cout<< "Poly Point "<<i+1<<": "<<poly[i].transpose()<<endl;
    }
    // A -> B -> C

    // Vectors
	vector<Vector3d> vecs(3);    
    vecs[0] = poly[1] - poly[0];
    vecs[1] = poly[2] - poly[1];
    vecs[2] = poly[0] - poly[2];

    vector<Vector3d> nors(3);    
    nors[0] = vecs[2].cross(vecs[0]);
	nors[1] = vecs[0].cross(vecs[1]);
    nors[2] = vecs[1].cross(vecs[2]);
    // ABC

    Vector3d avgNormal = nors[0] + nors[1] + nors[2];
    avgNormal.normalize();
    cout<< "Avg Normal: "<<avgNormal.transpose()<<endl<<endl;

    vector<double> Illums(3,0);
    for (int i = 0; i < 3; ++i)
    {
    	cout << "Point "<<i+1<<": \n";
    	double png = phongIllum( Iamb, Kamb,  Ip,  Kd, Ks, fatt,  refC,  nors[i],  light-poly[i], eye-poly[i]);
    	Illums[i] = png;
    	cout<<png<<endl<<"............\n"<<endl;
    }

   	cout<< "centroid :" << centroid.transpose()<<endl<<endl;
    double png = phongIllum( Iamb, Kamb,  Ip,  Kd, Ks, fatt,  refC,  avgNormal,  light-centroid, eye-centroid);
   	cout << "Point Cent: "<<png<<endl<<endl;

    
   	cout<<"\n.............. INTP ...................\n\n";
    // Interpolating Things
   	double intpF;
    cin >> intpF;
    cout<<"Y val intpF: "<<intpF<<endl;
    vector<Vector3d> intpNors(3);
    vector<Vector3d> intpCords(3);
    vector<double> intpIllums(3);

    for (int i = 0; i < N; ++i)
    {
    	int i2 = (i+1)%N;
    	intpIllums[i] = Illums[i]-(Illums[i]-Illums[i2])*(poly[i](1)-intpF)/(poly[i](1)-poly[i2](1));
    	intpNors[i] = nors[i]-(nors[i]-nors[i2])*(poly[i](1)-intpF)/(poly[i](1)-poly[i2](1));
    	intpCords[i] = poly[i]-(poly[i]-poly[i2])*(poly[i](1)-intpF)/(poly[i](1)-poly[i2](1));
    }

    for (int i = 0; i < N; ++i)
    {
    	cout<<"Point "<<i<<" - "<<(i+1)%N<<" :\n";
    	cout<<"Intp Illum (Gourmand): "<<intpIllums[i]<<endl;
    	cout<<"Intp Coord: "<<intpCords[i].transpose()<<endl;
    	cout<<"Intp Nor: "<<intpNors[i].transpose()<<endl<<endl;
    	//double png = phongIllum( Iamb, Kamb,  Ip,  Kd, Ks, fatt,  refC,  intpNors[i],  light-poly[i], eye-poly[i]);
    }

	return 0;
}