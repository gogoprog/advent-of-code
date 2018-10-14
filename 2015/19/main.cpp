#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    if (!v.empty()) {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

using Atom = std::string;
using Molecule = std::vector<Atom>;
using Replacements = std::map<Atom, std::vector<Molecule>>;

void parseMolecule(Molecule &result, const std::string input) {
    Atom atom;
    for (char c : input) {
        if (c >= 'A' && c <= 'Z') {
            if (!atom.empty()) {
                result.push_back(atom);
                atom.clear();
            }
        }

        atom.push_back(c);
    }

    if (!atom.empty()) {
        result.push_back(atom);
    }
}

/* void process(std::set<Molecule> & allMolecules, const Replacements &
 * replacements) */

void processFile(const std::string filename, const std::string input2) {
    std::cout << "\nFile " << filename << std::endl;
    std::ifstream f(filename);
    std::string line, str;
    Replacements replacements;
    Atom atom;
    Molecule molecule;

    while (std::getline(f, line)) {
        std::istringstream iss(line);
        molecule.clear();

        iss >> atom >> str >> str;

        parseMolecule(molecule, str);

        replacements[atom].push_back(molecule);
    }

    f.close();

    Molecule inputMolecule;
    parseMolecule(inputMolecule, input2);
    std::set<Molecule> allMolecules;

    for (size_t a{0}; a < inputMolecule.size(); ++a) {
        auto &atom = inputMolecule[a];
        auto it = replacements.find(atom);
        if (it != std::end(replacements)) {
            for (const auto &r : it->second) {
                auto copied = inputMolecule;
                auto p = copied.begin() + a;
                copied.erase(p);
                copied.insert(p, r.begin(), r.end());

                if (allMolecules.find(copied) == allMolecules.end()) {
                    allMolecules.insert(copied);
                }
            }
        }
    }

    std::cout << allMolecules.size() << std::endl;
}

int main() {

    processFile(
        "input.txt",
        "CRnCaCaCaSiRnBPTiMgArSiRnSiRnMgArSiRnCaFArTiTiBSiThFYCaFArCaCaSiThCaPB"
        "SiThSiThCaCaPTiRnPBSiThRnFArArCaCaSiThCaSiThSiRnMgArCaPTiBPRnFArSiThCa"
        "SiRnFArBCaSiRnCaPRnFArPMgYCaFArCaPTiTiTiBPBSiThCaPTiBPBSiRnFArBPBSiRnC"
        "aFArBPRnSiRnFArRnSiRnBFArCaFArCaCaCaSiThSiThCaCaPBPTiTiRnFArCaPTiBSiAl"
        "ArPBCaCaCaCaCaSiRnMgArCaSiThFArThCaSiThCaSiRnCaFYCaSiRnFYFArFArCaSiRnF"
        "YFArCaSiRnBPMgArSiThPRnFArCaSiRnFArTiRnSiRnFYFArCaSiRnBFArCaSiRnTiMgAr"
        "SiThCaSiThCaFArPRnFArSiRnFArTiTiTiTiBCaCaSiRnCaCaFYFArSiThCaPTiBPTiBCa"
        "SiThSiRnMgArCaF");

    return 0;
}
