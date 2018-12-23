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

void process(const Replacements &replacements, const Molecule &inputMolecule, const Molecule &target, const int steps) {
    for (size_t a{0}; a < inputMolecule.size(); ++a) {
        auto &atom = inputMolecule[a];
        auto it = replacements.find(atom);
        if (it != std::end(replacements)) {
            std::vector<Molecule> newMolecules{};
            for (const auto &r : it->second) {
                auto copied = inputMolecule;
                auto p = copied.begin() + a;
                copied.erase(p);
                copied.insert(p, r.begin(), r.end());

                if (copied.size() < target.size()) {
                    newMolecules.push_back(copied);
                } else if (copied.size() == target.size() && copied == target) {
                    std::cout << target << std::endl;
                    std::cout << steps << std::endl;
                    exit(0);
                }
            }

            for (const auto &newMolecule : newMolecules) {
                process(replacements, newMolecule, target, steps + 1);
            }
        }
    }
}

void rprocess(const ReverseReplacements &revReplacements, const Molecule &current, const Molecule &target,
              const int steps, int &minSteps) {
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

                    if (steps < minSteps) {
                        std::cout << steps << std::endl;
                        minSteps = steps;
                    }
                    ++it;
                    continue;
                }
            }

            if (rep.second != "e") {
                std::cout << copied << std::endl;
                rprocess(revReplacements, copied, target, steps + 1, minSteps);
            }
            ++it;
        }
    }
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
                if (allMolecules.find(copied) == allMolecules.end()) {
                    allMolecules.insert(copied);
                }
            }
        }
    }

    std::cout << allMolecules.size() << " / " << count << std::endl;

    Molecule start;
    parseMolecule(start, "e");
    int minSteps{std::numeric_limits<int>::max()};
    rprocess(revReplacements, inputMolecule, start, 1, minSteps);

    if (minSteps != std::numeric_limits<int>::max()) {
        std::cout << "Found: " << minSteps << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    system("read");
}

int main() {

    processFile("input2.txt", "HOHOHO");

    processFile("input.txt", "CRnPRnCaRnFArArFArAl");

    processFile("input.txt", "CRnCaCaCaSiRnBPTiMgArSiRnSiRnMgArSiRnCaFArTiTiBSiThFYCaFArCaCaSiThCaPB"
                             "SiThSiThCaCaPTiRnPBSiThRnFArArCaCaSiThCaSiThSiRnMgArCaPTiBPRnFArSiThCa"
                             "SiRnFArBCaSiRnCaPRnFArPMgYCaFArCaPTiTiTiBPBSiThCaPTiBPBSiRnFArBPBSiRnC"
                             "aFArBPRnSiRnFArRnSiRnBFArCaFArCaCaCaSiThSiThCaCaPBPTiTiRnFArCaPTiBSiAl"
                             "ArPBCaCaCaCaCaSiRnMgArCaSiThFArThCaSiThCaSiRnCaFYCaSiRnFYFArFArCaSiRnF"
                             "YFArCaSiRnBPMgArSiThPRnFArCaSiRnFArTiRnSiRnFYFArCaSiRnBFArCaSiRnTiMgAr"
                             "SiThCaSiThCaFArPRnFArSiRnFArTiTiTiTiBCaCaSiRnCaCaFYFArSiThCaPTiBPTiBCa"
                             "SiThSiRnMgArCaF");

    return 0;
}
