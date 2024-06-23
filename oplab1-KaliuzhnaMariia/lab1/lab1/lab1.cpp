#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class ErrorNoFile
{
    string str = "ErrorNoFile";
public:
    ErrorNoFile(string s) : str(s) { }
    void Message()
    {
        cout << "ErrorNoFile " << str << endl;
    }
};

class ErrorAlgo {
public:
    ErrorAlgo() { }
    void Message() {
        cout << "Error in function: switching to Algorithm 2." << endl;
    }
};

string determineFile(double& x) {
    if (fabs(x) <= 1) return "dat_X_1_1.dat";
    else if (x < -1) {
        x = 1 / x;
        return "dat_X_00_1.dat";
    }
    else if (x > 1) {
        x = 1 / x;
        return "dat_X_1_00.dat";
    }
}

//!Yes! T
double T(double x) throw ( ErrorNoFile) {
    try {
        string filename = determineFile(x);
        ifstream is(filename);
        if (!is) throw ErrorNoFile(filename);

        double xi, Ti, xi1, Ti1, result = 0;
        is >> xi1 >> Ti1;
        if (xi1 == x) result = Ti1;
        else {
            while (!is.eof()) {
                xi = xi1;
                Ti = Ti1;
                is >> xi1 >> Ti1;
                if (xi < x && x < xi1) {
                    result = Ti + (Ti1 - Ti) * (x - xi) / (xi1 - xi);
                    break;
                }
                if (xi1 == x) {
                    result = Ti1;
                    break;
                }
            }
        }
        is.close();
        return result;
    }
    catch (ErrorNoFile& e) {
        throw;
    }
}

//!Yes! U
double U(double x) throw ( ErrorNoFile){
    try {
        string filename = determineFile(x);
        ifstream is(filename);
        if (!is) throw ErrorNoFile(filename);

        double xi, Ui, xi1, Ui1, result = 0;
        is >> xi1 >> Ui1;
        if (xi1 == x) {
            result = Ui1;
        }
        else {
            while (!is.eof()) {
                xi = xi1;
                Ui = Ui1;
                is >> xi1 >> Ui1;
                if (xi < x && x < xi1) {
                    result = Ui + (Ui1 - Ui) * (x - xi) / (xi1 - xi);
                    break;
                }
                if (xi1 == x) {
                    result = Ui1;
                    break;
                }
            }
        }

        is.close();
        return result;
    }
    catch (ErrorNoFile& e) {
        throw;
    }
}

//Gold
double Gold(double x, double y) throw (ErrorAlgo) {
    if (x > y && y != 0) {
        return x / y;
    }
    else if (x < y && x != 0) {
        return y / x;
    }
    else{
        throw ErrorAlgo();
    }
}

//!Yes! Gold1
double Gold1(double x, double y) {
    if (x > y && fabs(y) > 0.1) return x / y;
    else if (x <= y && fabs(x) > 0.1) return y / x;
    else if (x < y && fabs(x) > 0.1) return 0.15;
    else if (y == 0) return 0.1;
}

//!Yes! Glr
double Glr(double x, double y) throw (ErrorAlgo) {
    double exp = sqrt(x * x + y * y - 4);
    if (fabs(x) < 1) return x;
    else if (fabs(x) >= 1 && fabs(y) < 1) return y;
    else if (fabs(x) >= 1 && fabs(y) >= 1 && exp > 0.1) return y / exp;
    else throw ErrorAlgo();
}

//!Yes! Glr1
double Glr1(double x, double y) {
    if (fabs(x) < 1) return x;
    else return y;
}

//!Yes! Srz
double Srz(double x, double y, double z) {
    if (x > y) return T(x) + U(z) - T(y);
    else return T(y) + U(y) - U(z);
}

//!Yes! Grs
double Grs(double x, double y) {
    double p1 = 0.1389 * Srz(x + y, Gold(x, y), Glr(x, x * y));
    double p2 = 1.8389 * Srz(x - y, Gold(y, x / 5), Glr(5 * x, x * y));
    double p3 = 0.83 * Srz(x - 0.9, Glr(y, x / 5), Gold(5 * y, y));
    return p1 + p2 + p3;
}

//!Yes! Grs1
double Grs1(double x, double y) {
    double p1 = 0.14 * Srz(x + y, Gold(x, y), Glr1(x, x * y));
    double p2 = 1.83 * Srz(x - y, Gold(y, x / 5), Glr1(4 * x, x * y));
    double p3 = 0.83 * Srz(x, Glr1(y, x / 4), Gold(4 * y, y));
    return p1 + p2 + p3;
}

//!Yes! Algorithm
double fun(double x, double y, double z) throw (ErrorAlgo, ErrorNoFile) {
    double temp;
    try {
        temp = x * x * Grs(y, z) + y * y * Grs(x, z) + 0.33 * x * y * Grs(x, z);
    }
    catch (ErrorAlgo& e) {
        temp = x * Grs1(x, y) + y * Grs1(y, z) + z * Grs1(z, x);
    }
    catch (ErrorNoFile& e) {
        temp = 1.3498 * z + 2.2362 * y - 2.348 * x * y;
    }
    return temp;
}

//Main part
int main() {
    double x, y, z, result;
    cout << "Enter x, y, z values: ";
    cin >> x >> y >> z;
    try {
        result = fun(x, y, z);
        cout << "Result: " << result << endl;

    }
    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}