
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <Eigen/Dense>
#include <StarTrackBase.h>


#define NUM_CAL_WASH 0


int main(int argc, char **argv)
{
    std::vector<Vector3d> tpoints;
    std::vector<Vector3d> mpoints;

//    StarTrackBase starTrack(NUM_CAL_WASH, 4);
    StarTrackBase starTrack(NUM_CAL_WASH, 1);

    Vector3d point;

/*
    // Aluminum 8-lug 3-sided pattern cube
    // Side A
    point << 0.16,     0.2489,    0.0;   tpoints.push_back(point);
    point << 0.16f,    0.0711,    0.0;   tpoints.push_back(point);
    point << 0.2489,   0.16,      0.0;   tpoints.push_back(point);
    point << 0.0711,   0.16,      0.0;   tpoints.push_back(point);
    point << 0.222738, 0.222738,  0.0;   tpoints.push_back(point);
    point << 0.097262, 0.097262,  0.0;   tpoints.push_back(point);
    point << 0.222738, 0.097262,  0.0;   tpoints.push_back(point);
    point << 0.097262, 0.222738f, 0.0;   tpoints.push_back(point);
    // Side B
    point << 0.16,     0.0, 0.0711;      tpoints.push_back(point);
    point << 0.16,     0.0, 0.2489;      tpoints.push_back(point);
    point << 0.2489,   0.0, 0.16;        tpoints.push_back(point);
    point << 0.0711,   0.0, 0.16;        tpoints.push_back(point);
    point << 0.222738, 0.0, 0.097262;    tpoints.push_back(point);
    point << 0.097262, 0.0, 0.222738;    tpoints.push_back(point);
    point << 0.222738, 0.0, 0.222738;    tpoints.push_back(point);
    point << 0.097262, 0.0, 0.097262;    tpoints.push_back(point);
    // Side C
    point << 0.0, 0.16,     0.0711;      tpoints.push_back(point);
    point << 0.0, 0.16,     0.2489;      tpoints.push_back(point);
    point << 0.0, 0.0711,   0.16;        tpoints.push_back(point);
    point << 0.0, 0.2489,   0.16;        tpoints.push_back(point);
    point << 0.0, 0.097262, 0.097262;    tpoints.push_back(point);
    point << 0.0, 0.222738, 0.222738;    tpoints.push_back(point);
    point << 0.0, 0.097262, 0.222738;    tpoints.push_back(point);
    point << 0.0, 0.222738, 0.097262;    tpoints.push_back(point);
*/

    // 8.5x11" Landscape "Paper" Points
    point << 0.0, 0.0, 0.0; tpoints.push_back(point); // center
    point << -0.2, 0.2, 0.0; tpoints.push_back(point); // top-left
    point << -0.2, -0.2, 0.0; tpoints.push_back(point); // bottom-left
    point << 0.2, -0.2, 0.0; tpoints.push_back(point); // bottom-right
    point << 0.2, 0.2, 0.0; tpoints.push_back(point); // top-right

    for (int i = 0; i < tpoints.size(); i++) {
        starTrack.add_truth_point(tpoints[i]);
    }

//    // Measured points
//    // First 3 are Jake's calibration points
//    point << -1.798019522243272e6, -4.536312368985481e6, 4.095677423413508e6;    mpoints.push_back(point);
//    point << -1.798019743950901e6, -4.536312321795086e6, 4.095677267588068e6;    mpoints.push_back(point);
//    point << -1.798019558614969e6, -4.536312481261311e6, 4.095677183419921e6;    mpoints.push_back(point);
//    // Actual cube sequence
//    point << -1.798019517203591e6, -4.536312415089283e6, 4.095677367936790e6;    mpoints.push_back(point); // 0
//    point << -1.798019685237269e6, -4.536312399602084e6, 4.095677316938862e6;    mpoints.push_back(point); // 1
//    point << -1.798019613336576e6, -4.536312342770044e6, 4.095677402316733e6;    mpoints.push_back(point); // 2
//    point << -1.798019589426526e6, -4.536312467577970e6, 4.095677283128023e6;    mpoints.push_back(point); // 3
//    point << -1.798019552224239e6, -4.536312374336401e6, 4.095677401048793e6;    mpoints.push_back(point); // 4
//    point << -1.798019650285735e6, -4.536312442918447e6, 4.095677285715976e6;    mpoints.push_back(point); // 5
//    point << -1.798019663628433e6, -4.536312367143728e6, 4.095677366267579e6;    mpoints.push_back(point); // 6
//    point << -1.798019545139160e6, -4.536312437969652e6, 4.095677324591906e6;    mpoints.push_back(point); // 7
//    point << -1.798019731808595e6, -4.536312290106419e6, 4.095677227078727e6;    mpoints.push_back(point); // 8
//
//    //point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9
//    //point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10
//
//    // TEST: flip the order of points 9 and 10
//    point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10
//    point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9
//
//
//    point << -1.798019684970558e6, -4.536312300180818e6, 4.095677107640639e6;    mpoints.push_back(point); // 11
//    point << -1.798019725482318e6, -4.536312239756521e6, 4.095677254010813e6;    mpoints.push_back(point); // 12
//    point << -1.798019658327266e6, -4.536312242092346e6, 4.095677098753882e6;    mpoints.push_back(point); // 13
//    point << -1.798019665405295e6, -4.536312165045403e6, 4.095677169059504e6;    mpoints.push_back(point); // 14
//    point << -1.798019686159439e6, -4.536312336958287e6, 4.095677160899903e6;    mpoints.push_back(point); // 15
//    point << -1.798019509950534e6, -4.536312453775291e6, 4.095677167013520e6;    mpoints.push_back(point); // 16
//    point << -1.798019461375211e6, -4.536312330086507e6, 4.095677060349145e6;    mpoints.push_back(point); // 17
//    point << -1.798019575886232e6, -4.536312381392281e6, 4.095677087361727e6;    mpoints.push_back(point); // 18
//    point << -1.798019413027543e6, -4.536312400510028e6, 4.095677140208910e6;    mpoints.push_back(point); // 19
//    point << -1.798019575590895e6, -4.536312421692585e6, 4.095677131866417e6;    mpoints.push_back(point); // 20
//    point << -1.798019432348833e6, -4.536312351114717e6, 4.095677083674763e6;    mpoints.push_back(point); // 21
//    point << -1.798019543646745e6, -4.536312323787866e6, 4.095677061641934e6;    mpoints.push_back(point); // 22
//    point << -1.798019464446433e6, -4.536312428227114e6, 4.095677175492271e6;    mpoints.push_back(point); // 23

    //point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9
    //point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10

    // TEST: flip the order of points 9 and 10
/*    point << -1.798019713899294e6, -4.536312172564074e6, 4.095677227658298e6;    mpoints.push_back(point); // 10
    point << -1.798019682635105e6, -4.536312172986366e6, 4.095677116288032e6;    mpoints.push_back(point); // 9


    point << -1.798019684970558e6, -4.536312300180818e6, 4.095677107640639e6;    mpoints.push_back(point); // 11
    point << -1.798019725482318e6, -4.536312239756521e6, 4.095677254010813e6;    mpoints.push_back(point); // 12
    point << -1.798019658327266e6, -4.536312242092346e6, 4.095677098753882e6;    mpoints.push_back(point); // 13
    point << -1.798019665405295e6, -4.536312165045403e6, 4.095677169059504e6;    mpoints.push_back(point); // 14
    point << -1.798019686159439e6, -4.536312336958287e6, 4.095677160899903e6;    mpoints.push_back(point); // 15
    point << -1.798019509950534e6, -4.536312453775291e6, 4.095677167013520e6;    mpoints.push_back(point); // 16
    point << -1.798019461375211e6, -4.536312330086507e6, 4.095677060349145e6;    mpoints.push_back(point); // 17
    point << -1.798019575886232e6, -4.536312381392281e6, 4.095677087361727e6;    mpoints.push_back(point); // 18
    point << -1.798019413027543e6, -4.536312400510028e6, 4.095677140208910e6;    mpoints.push_back(point); // 19
    point << -1.798019575590895e6, -4.536312421692585e6, 4.095677131866417e6;    mpoints.push_back(point); // 20
    point << -1.798019432348833e6, -4.536312351114717e6, 4.095677083674763e6;    mpoints.push_back(point); // 21
    point << -1.798019543646745e6, -4.536312323787866e6, 4.095677061641934e6;    mpoints.push_back(point); // 22
    point << -1.798019464446433e6, -4.536312428227114e6, 4.095677175492271e6;    mpoints.push_back(point); // 23
*/
    point << -1804335.6630418838, -4496337.463669791, 4136331.7836131733; mpoints.push_back(point);
    point << -1804335.5724848504, -4496337.484435457, 4136331.760549534; mpoints.push_back(point);
    point << -1804335.6431861257, -4496337.398065903, 4136331.847297133; mpoints.push_back(point);
    point << -1804335.5794941061, -4496337.598529494, 4136331.7002633847; mpoints.push_back(point);
    point << -1804335.5024080335, -4496337.599716969, 4136331.7400787952; mpoints.push_back(point);


    starTrack.print_truth_points();

    // Simulate sequence by adding each measured point
    int N;
    for (int i = 0; i < mpoints.size(); i++) {
        N = starTrack.add_measurement_point(mpoints[i]);
        std::cout << "Total number of measured points: " << N << std::endl;
        if (i >= NUM_CAL_WASH) {
            starTrack.processPoints();
        }
    }

    return 0;

}