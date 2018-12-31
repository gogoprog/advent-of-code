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

template <typename T> std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
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
using ReverseReplacements = std::map<Molecule, Atom>;

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

int minSize{10000};
std::set<Molecule> processed;

bool rprocess(const ReverseReplacements &revReplacements, const Molecule &current, const Molecule &target,
              const int steps) {
    processed.insert(current);

    for (const auto &rep : revReplacements) {
        auto it = current.begin();
        const auto &m = rep.first;

        while ((it = std::search(it, current.end(), m.begin(), m.end())) != current.end()) {
            auto copied = current;
            auto first = copied.begin() + (it - current.begin());
            copied.erase(first, first + m.size());
            copied.insert(first, rep.second);

            if (copied.size() == 1) {
                if (copied == target) {
                    std::cout << "part2: " << steps << std::endl;
                    return true;
                }
            }

            if (rep.second != "e") {
                if (processed.find(copied) != processed.end()) {
                    return false;
                }
                if (rprocess(revReplacements, copied, target, steps + 1)) {
                    return true;
                }
            }
            ++it;
        }
    }

    return false;
}

void processFile(const std::string filename, const std::string input2) {
    std::cout << "\nFile " << filename << std::endl;
    std::ifstream f(filename);
    std::string line, str;
    Replacements replacements{};
    ReverseReplacements revReplacements{};
    Atom atom;
    Molecule molecule;

    while (std::getline(f, line)) {
        std::istringstream iss(line);
        molecule.clear();

        iss >> atom >> str >> str;

        parseMolecule(molecule, str);

        replacements[atom].push_back(molecule);
        revReplacements[molecule] = atom;
    }

    f.close();

    Molecule inputMolecule;
    parseMolecule(inputMolecule, input2);
    std::set<Molecule> allMolecules;
    int count{0};

    for (size_t a{0}; a < inputMolecule.size(); ++a) {
        auto &atom = inputMolecule[a];
        auto it = replacements.find(atom);
        if (it != std::end(replacements)) {
            for (const auto &r : it->second) {
                auto copied = inputMolecule;
                auto p = copied.begin() + a;
                copied.erase(p);
                copied.insert(p, r.begin(), r.end());

                ++count;
                allMolecules.insert(copied);
            }
        }
    }

    std::cout << "part1: " << allMolecules.size() << " / " << count << std::endl;

    Molecule start;
    parseMolecule(start, "e");
    minSize = 10000;
    processed.clear();
    rprocess(revReplacements, inputMolecule, start, 1);
}

int main() {

    processFile("input2.txt", "HOH");
    processFile("input2.txt", "HOHOHO");

    auto input =
        "CRnCaCaCaSiRnBPTiMgArSiRnSiRnMgArSiRnCaFArTiTiBSiThFYCaFArCaCaSiThCaPBSiThSiThCaCaPTiRnPBSiThRnFArArCaCaSiThCa"
        "SiThSiRnMgArCaPTiBPRnFArSiThCaSiRnFArBCaSiRnCaPRnFArPMgYCaFArCaPTiTiTiBPBSiThCaPTiBPBSiRnFArBPBSiRnCaFArBPRnSi"
        "RnFArRnSiRnBFArCaFArCaCaCaSiThSiThCaCaPBPTiTiRnFArCaPTiBSiAlArPBCaCaCaCaCaSiRnMgArCaSiThFArThCaSiThCaSiRnCaFYC"
        "aSiRnFYFArFArCaSiRnFYFArCaSiRnBPMgArSiThPRnFArCaSiRnFArTiRnSiRnFYFArCaSiRnBFArCaSiRnTiMgArSiThCaSiThCaFArPRnFA"
        "rSiRnFArTiTiTiTiBCaCaSiRnCaCaFYFArSiThCaPTiBPTiBCaSiThSiRnMgArCaF";

    processFile("input.txt", input);

    return 0;
}
