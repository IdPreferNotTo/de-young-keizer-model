#include <iostream>
#include <vector>
#include <random>
#include <pwd.h>
#include <cmath>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class DYK {
private:
    int i = 0;
    int j = 0;
    int k = 0;

public:
    DYK() = default;

    int get_i() const { return i; }

    int get_j() const { return j; }

    int get_k() const { return k; }


    void change_i() {
        if(i == 0){
            i = 1;
        }
        else{
            i = 0;
        }
    }
    void change_j() {
        if(j == 0){
            j = 1;
        }
        else{
            j = 0;
        }
    }
    void change_k() {
        if(k == 0){
            k = 1;
        }
        else{
            k = 0;
        }
    }
};


int main(int argc, char *argv[]) {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;

    std::string path;
    path = "../out/";

    int N = 30;
    double a1 = 400;
    double a2 = 0.2;
    double a3 = 400;
    double a4 = 0.2;
    double a5 = 20;
    double d1 = 0.13;
    double d2 = 1.049;
    double d3 = 0.9434;
    double d4 = 0.1445;
    double d5 = 0.08234;
    double b1 = a1*d1;
    double b2 = a2*d2;
    double b3 = a3*d3;
    double b4 = a4*d4;
    double b5 = a5*d5;


    std::vector<double> Cs{0.01, 0.01585, 0.02512, 0.03981, 0.06310}; //{0.1, 0.1585, 0.2512, 0.3981, 0.6310, 1., 1.585, 2.512, 3.981, 6.310, 10.};
    double C = Cs[4];
    std::cout << C << std::endl;
    char parameters_DYK[200];
    std::sprintf(parameters_DYK, "DYK_C%.2e_N%d.dat", C, N);
    std::string out_file_DYK_channel;
    out_file_DYK_channel = path + parameters_DYK;
    std::ofstream file_DYK_channel;
    file_DYK_channel.open(out_file_DYK_channel);
    if (!file_DYK_channel.is_open()) {
        std::cout << "Could not open DYK file at: " << out_file_DYK_channel << std::endl;
        std::cout << "This is where I am: " << std::string(homedir) << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> uniform_double(0, 1);

    std::vector<DYK> dyk_channels;
    for (int i = 0; i < N; i++) {
        DYK dyk_channel;
        dyk_channels.push_back(dyk_channel);
    }

    double t = 0;
    double dt = 0.001;
    double t_max = 20000;

    double n;
    double rnd1, rnd2;
    int i, j, k;
    double trans_rate_i, trans_rate_j, trans_rate_k, trans_rate_total;

    a2 = a2*C;
    a4 = a4*C;
    a5 = a5*C;
    while (t < t_max) {
        n = 0;
        for (auto &dyk_channel: dyk_channels) {
            i = dyk_channel.get_i();
            j = dyk_channel.get_j();
            k = dyk_channel.get_k();
            if(i==1 and j==1 and k==0) {
                n += 1;
            }
            rnd1 = uniform_double(generator);

            if (i == 0 and j==0 and k==0) {
                trans_rate_i = a1;
                trans_rate_j = a5;
                trans_rate_k = a4;
            } else if (i == 0 and j==0 and k==1) {
                trans_rate_i = a3;
                trans_rate_j = a5;
                trans_rate_k = b4;
            } else if (i == 0 and j==1 and k==0){
                trans_rate_i = a1;
                trans_rate_j = b5;
                trans_rate_k = a4;
            } else if (i == 0 and j==1 and k==1) {
                trans_rate_i = a3;
                trans_rate_j = b5;
                trans_rate_k = b4;
            } else if (i == 1 and j==0 and k==0) {
                trans_rate_i = b1;
                trans_rate_j = a5;
                trans_rate_k = a2;
            } else if (i == 1 and j==0 and k==1) {
                trans_rate_i = b3;
                trans_rate_j = a5;
                trans_rate_k = b2;
            } else if (i == 1 and j==1 and k==0){
                trans_rate_i = b1;
                trans_rate_j = b5;
                trans_rate_k = a2;
            } else {
                trans_rate_i = b3;
                trans_rate_j = b5;
                trans_rate_k = b2;
            }
            trans_rate_total = trans_rate_i + trans_rate_j + trans_rate_k;
            if (rnd1 < trans_rate_total * dt) {
                rnd2 = uniform_double(generator);
                if (rnd2 < trans_rate_i / (trans_rate_total)) {
                    dyk_channel.change_i();
                }
                else if (rnd2 < (trans_rate_i + trans_rate_j) / (trans_rate_total)) {
                    dyk_channel.change_j();
                }
                else {
                    dyk_channel.change_k();
                }
            }
        }

                // V += (-gK * (nK /Nk)*(V - vK) - gNa * (nNa/Nna)*(V - vNa) - gL*(V - vL) + mu) * dt / C;
        t += dt;

        // file_voltage << t << " " << V << " " << nNa << " " << nK << "\n";


        file_DYK_channel << t;
        int x;
        for (auto &dyk_channel: dyk_channels) {
            if (dyk_channel.get_i() == 1 and dyk_channel.get_j() == 1 and dyk_channel.get_k() == 0) {
                x = 1;
            } else {
                x = 0;
            }
            file_DYK_channel << " " << x;
        }
        file_DYK_channel << "\n";
    }
}
