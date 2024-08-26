#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <gmp.h>
#include "nlohmann/json_fwd.hpp"
#include "utility.h"
#include "phe.h"
#include "globcom.h"

using namespace std;


void phe_test()
{
    FHE::PHE phe(128);
    mpz_t pt;
    mpz_t ct;
    mpz_init(pt);
    mpz_init(ct);
    phe.encrypt(ct, 12);
    phe.decrypt(pt, ct);
    gmp_printf("phe decrypt: %Zd\n", pt);
    phe.encrypt(pt, 11);
    phe.add(pt, pt, ct);
    phe.decrypt(pt, pt);
    gmp_printf("11 + 12 = %Zd\n", pt);
    phe.encrypt(pt, 12);
    phe.mul(pt, pt, 2);
    phe.decrypt(pt, pt);
    gmp_printf("2 * 12 = %Zd\n", pt);
    
    phe.encrypt(ct, 1234567);
    phe.neg(ct, ct);
    phe.decrypt(pt, ct);
    phe.decode(pt, pt);
    gmp_printf("-2 = %Zd\n", pt);
    
    phe.encode_encrypt(ct, -12);
    phe.decode_decrypt(pt, ct);
    gmp_printf("-12 = %Zd\n", pt);
    
    phe.encode_encrypt(ct, -11);
    phe.encode_encrypt(pt, -12);
    phe.sub(ct, ct, pt);
    phe.decode_decrypt(pt, ct);
    gmp_printf("-11 - (-12) = %Zd\n", pt);
    
    phe.encode_encrypt(ct, 6);
    mpz_set_ui(pt, 2);
    phe.div(ct, ct, pt);
    phe.decode_decrypt(pt, ct);
    gmp_printf("6 / 2 = %Zd\n", pt);

    mpz_clear(pt);
    mpz_clear(ct);
}

#define NOT_IGNORE(arg) (string(arg)!="-")
int main(int argc, char * argv[])
{
    // phe_test();
    // submain();
    // return 0;
    string filepath = "../dataset/200-2-100000.json";
    filepath = "../dataset/5-2-100.json";
    string query_path = "../params/query.json";
    string query_type = "d";
    auto print_vector = [](const vector<int>& arr) {
        for (const int i : arr) {
            cout << i << " ";
        }
        cout << endl;
    };
    if (argc > 1 && NOT_IGNORE(argv[1])) {
        filepath = argv[1];
    }
    vector<int> query;
    if (argc > 2 && NOT_IGNORE(argv[2])) {
        query_path = argv[2];
    }
    nlohmann::json qjson;
    ifstream qfile(query_path);
    qfile >> qjson;
    qfile.close();
    qjson.get_to(query);
    
    auto tmpv = utility::parse_dataset_path(filepath);
    if (tmpv.size() == 3) {
        const int d = tmpv[1];
        const int T = tmpv[2];
        if (query.size() != d) {
            query = utility::generate_int_query_random(d, T);
        }
    }
    
    if (argc > 3 && NOT_IGNORE(argv[3])) {
        query_type = argv[3];
    }
    
    cout << "[info] query point is: [";
    for (int i : query) {
        cout << i << ", ";
    }
    cout << "]" << endl;
    
    my_emulater(filepath, query);
    // select query type
    // if (query_type == "d") {
    //     dynamic_skyline_emulater(filepath, query);
    // }
    // else if (query_type == "r") {
    //     reverse_skyline_emulater(filepath, query);
    // }
    // else {
    //     cout << "don't have this skyline query: " << query_type << endl;
    // }
    return 0;
}