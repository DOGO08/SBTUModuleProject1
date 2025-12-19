#include <iostream>
#include <cmath>
#include <vector>
#include "gnuplot-iostream.h"
using namespace std;

const double G = 6.6743e-11;
const double PI = 3.14159265359;
const double g = 9.8;
const double radiusEarth = 6.371e6;

// Function prototypes
void projectileMotion();
void asteroidCollision();
void ballisticPendulum();
void satelliteMotion();

int main() {
    int choice;

    do {
        cout << "\n********** Welcome to the Module Project **********\n";
        cout << "                                ***                    " << endl;
        cout << "                             **********                " << endl;
        cout << "                          ****  **********             " << endl;
        cout << "                       *********   **********          " << endl;
        cout << "                   **********  *****  **********       " << endl;
        cout << "                 *********  ************ **********    " << endl;
        cout << "                *******  ******************  ****    " << endl;
        cout << "                ****** **********  **********  *****  " << endl;
        cout << "                ***** *******  ***********  ********  " << endl;
        cout << "                ***** ****** **********  ***********  " << endl;
        cout << "                ***** ***** ********  **************  " << endl;
        cout << "                ***** ***** ******* *********** ****  " << endl;
        cout << "                ***** ***** ****** ********  *******  " << endl;
        cout << "                ***** ***** ****** *****  **********  " << endl;
        cout << "                ***** ***** ****** *********** *****  " << endl;
        cout << "                ***** ***** ****** ******** ********  " << endl;
        cout << "                *********** ****** ***** **********   " << endl;
        cout << "                  ********* ****** ***************    " << endl;
        cout << "                      ***** ****** ************       " << endl;
        cout << "                         ** ****** ********           " << endl;
        cout << "                             ***** *****              " << endl;
        cout << "                                ** **                 " << endl;

        cout << "1. Projectile Motion\n";
        cout << "2. Asteroid Collision\n";
        cout << "3. Ballistic Pendulum\n";
        cout << "4. Satellite Motion\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            projectileMotion();
            break;
        case 2:
            asteroidCollision();
            break;
        case 3:
            ballisticPendulum();
            break;
        case 4:
            satelliteMotion();
            break;
        case 5:
            cout << "Exiting the program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please select a valid option.\n";
        }
    } while (choice != 5);

    return 0;
}

void projectileMotion() {
    double initialSpeed, angle, height;

    cout << "Enter the initial speed of the projectile (m/s): ";
    cin >> initialSpeed;
    cout << "Enter the angle of projection (degrees): ";
    cin >> angle;
    cout << "Enter the height from which the projectile is launched (m): ";
    cin >> height;

    // Convert the angle to radians
    double radianAngle = angle * PI / 180.0;

    // Calculate horizontal and vertical velocity components
    double horizontalVelocity = initialSpeed * cos(radianAngle);
    double verticalVelocity = initialSpeed * sin(radianAngle);

    // Calculate maximum height
    double maxHeight = height + (verticalVelocity * verticalVelocity) / (2 * g);

    // Calculate the time of flight (upward and downward)
    double timeToTop = verticalVelocity / g; // Time to reach maximum height
    double totalTime = timeToTop + sqrt(2 * maxHeight / g); // Total time

    // Calculate the range
    double range = horizontalVelocity * totalTime;

    // Print the results
    cout << "Horizontal velocity (m/s): " << horizontalVelocity << endl;
    cout << "Vertical velocity (m/s): " << verticalVelocity << endl;
    cout << "Maximum height reached (m): " << maxHeight << endl;
    cout << "Total time of flight (s): " << totalTime << endl;
    cout << "Range of the projectile (m): " << range << endl;

    // Plot the projectile motion
    vector<pair<double, double>> x_t, y_t, vx_t, vy_t;
    double discriminant = verticalVelocity * verticalVelocity + 2 * g * height;

    if (discriminant < 0) {
        cout << "Error: Invalid parameters for the projectile motion.\n";
        return;
    }

    double timeToHit = (-verticalVelocity + sqrt(discriminant)) / g;

    for (double t = 0; t <= timeToHit; t += 0.01) {
        double x = horizontalVelocity * t;
        double y = height + verticalVelocity * t - 0.5 * g * t * t;
        if (y < 0) break;

        double vx = horizontalVelocity;
        double vy = verticalVelocity - g * t;

        x_t.emplace_back(t, x);
        y_t.emplace_back(t, y);
        vx_t.emplace_back(t, vx);
        vy_t.emplace_back(t, vy);
    }

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist");

    gp << "set multiplot layout 2, 2 title 'Projectile Motion Analysis'\n";

    gp << "set title 'X vs Time'\n";
    gp << "set xlabel 'Time (s)'\n";
    gp << "set ylabel 'Horizontal Position (m)'\n";
    gp << "plot '-' with lines title 'X(t)'\n";
    gp.send1d(x_t);

    gp << "set title 'Y vs Time'\n";
    gp << "set xlabel 'Time (s)'\n";
    gp << "set ylabel 'Vertical Position (m)'\n";
    gp << "plot '-' with lines title 'Y(t)'\n";
    gp.send1d(y_t);

    gp << "set title 'Vx vs Time'\n";
    gp << "set xlabel 'Time (s)'\n";
    gp << "set ylabel 'Horizontal Velocity (m/s)'\n";
    gp << "plot '-' with lines title 'Vx(t)'\n";
    gp.send1d(vx_t);

    gp << "set title 'Vy vs Time'\n";
    gp << "set xlabel 'Time (s)'\n";
    gp << "set ylabel 'Vertical Velocity (m/s)'\n";
    gp << "plot '-' with lines title 'Vy(t)'\n";
    gp.send1d(vy_t);

    gp << "unset multiplot\n";
}

void asteroidCollision() {
    double v_A1, angleA, angleB;

    cout << "Enter the initial speed of Asteroid A (m/s): ";
    cin >> v_A1;
    cout << "Enter the angle of Asteroid A after collision (degrees): ";
    cin >> angleA;
    cout << "Enter the angle of Asteroid B after collision (degrees): ";
    cin >> angleB;

    double theta_A = (angleA * PI) / 180.0;
    double theta_B = (angleB * PI) / 180.0;
    double sinA = sin(theta_A), cosA = cos(theta_A);
    double sinB = sin(theta_B), cosB = cos(theta_B);
    double v_B2 = v_A1 / (cosA * (sinB / sinA) + cosB);
    double v_A2 = v_B2 * (sinB / sinA);

    double initialKineticEnergy = 0.5 * v_A1 * v_A1;
    double combinedKineticEnergy = 0.5 * v_A2 * v_A2 + 0.5 * v_B2 * v_B2;

    cout << "\nInitial Kinetic Energy: " << initialKineticEnergy << " J\n";
    cout << "Combined Kinetic Energy after Collision: " << combinedKineticEnergy << " J\n";

    cout << "After collision speeds:\n";
    cout << "Asteroid A: " << v_A2 << " m/s\n";
    cout << "Asteroid B: " << v_B2 << " m/s\n";

    vector<pair<double, double>> trajectoryA, trajectoryB;
    for (double t = 0; t <= 10; t += 0.1) {
        trajectoryA.emplace_back(v_A2 * cos(theta_A) * t, v_A2 * sin(theta_A) * t);
        trajectoryB.emplace_back(v_B2 * cos(theta_B) * t, v_B2 * sin(theta_B) * t);
    }

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist");
    gp << "set title 'Asteroid Collision Trajectories'\n";
    gp << "set xlabel 'X Position (m)'\n";
    gp << "set ylabel 'Y Position (m)'\n";
    gp << "set grid\n";
    gp << "plot '-' with lines title 'Asteroid A', '-' with lines title 'Asteroid B'\n";
    gp.send1d(trajectoryA);
    gp.send1d(trajectoryB);
}

void ballisticPendulum() {
    double bulletMass, bulletSpeed, pendulumMass, pendulumLength;

    cout << "Enter the bullet mass (kg): ";
    cin >> bulletMass;
    cout << "Enter the bullet speed (m/s): ";
    cin >> bulletSpeed;
    cout << "Enter the pendulum mass (kg): ";
    cin >> pendulumMass;
    cout << "Enter the pendulum length (m): ";
    cin >> pendulumLength;

    double totalMass = bulletMass + pendulumMass;
    double velocityAfterCollision = (bulletMass * bulletSpeed) / totalMass;
    double height = (velocityAfterCollision * velocityAfterCollision) / (2 * g);

    if (height > pendulumLength) height = pendulumLength;

    cout << "\nResults:\n";
    cout << "Velocity after collision: " << velocityAfterCollision << " m/s\n";
    cout << "Height reached by pendulum: " << height << " m\n";

    vector<pair<double, double>> path;
    double maxAngle = asin(height / pendulumLength);
    for (double theta = -maxAngle; theta <= maxAngle; theta += 0.01) {
        path.emplace_back(pendulumLength * sin(theta), -pendulumLength * cos(theta));
    }

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist");
    gp << "set title 'Pendulum Swing Path'\n";
    gp << "set xlabel 'Horizontal Position (m)'\n";
    gp << "set ylabel 'Vertical Position (m)'\n";
    gp << "set grid\n";
    gp << "plot '-' with lines title 'Swing Path'\n";
    gp.send1d(path);
}

void satelliteMotion() {
    double altitude, planetaryMass;

    cout << "Enter the altitude of the satellite above Earth's surface (m): ";
    cin >> altitude;
    cout << "Enter the planetary mass (kg, default for Earth is 5.972e24): ";
    cin >> planetaryMass;

    double orbitalRadius = radiusEarth + altitude;
    double orbitalVelocity = sqrt((G * planetaryMass) / orbitalRadius);
    double orbitalPeriod = 60 * (2 * PI * sqrt(pow(orbitalRadius, 3) / (G * planetaryMass)));

    cout << "\nOrbital Velocity: " << orbitalVelocity << " m/s\n";
    cout << "Orbital Period: " << orbitalPeriod / 60 << " seconds\n";

    vector<pair<double, double>> trajectory;
    for (double t = 0; t <= orbitalPeriod; t += orbitalPeriod / 1000) {
        double x = orbitalRadius * cos(2 * PI * t / orbitalPeriod);
        double y = orbitalRadius * sin(2 * PI * t / orbitalPeriod);
        trajectory.emplace_back(x, y);
    }

    Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\" -persist");
    gp << "set title 'Satellite Orbit Trajectory'\n";
    gp << "set xlabel 'X Position (m)'\n";
    gp << "set ylabel 'Y Position (m)'\n";
    gp << "set grid\n";
    gp << "plot '-' with lines title 'Orbit'\n";
    gp.send1d(trajectory);
}
