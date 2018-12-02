#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

int main() {
    const std::string input = R"(
ayitmcjvlhedbsyoqfzukjpxwt
agirmcjvlheybsyogfzuknpxxt
wgirmcjvlvedbsyoqfzujnpxwt
agizmcjvlhedbsyoqfzuenlxwt
aryrmcjvlheebsyoqfzuknpxwt
agirmcjelhedbsyoqfzukosxwt
azirmcjvlhedbsooqfzuknpxvt
agirmcjvffedbsyoqfzudnpxwt
agilmcjvlhedbsyrqfzuknpxrt
agirmcjvlhndbsyoofzukcpxwt
awirmcjvlhedbsyoqfzuknpxlz
aghrmcjmlhewbsyoqfzuknpxwt
apirmcjvlmedbsyoqfzcknpxwt
jgiricjvlhedbsyrqfzuknpxwt
abirmcjvlbedbsyoqfzuknpxwo
agirmcjvlhedbsyojfzuknpgkt
agicmclvlhedbmyoqfzuknpxwt
aslrzcjvlhedbsyoqfzuknpxwt
agiqmcjvlhedbsymqfzurnpxwt
agirmcjvlwedbsyoqfzuknfxmt
agiumcjvlhedbsyoqfzuknpbyt
xgirxcjvlwedbsyoqfzuknpxwt
bgtrvcjvlhedbsyoqfzuknpxwt
afirmcjvlpedbsyoqvzuknpxwt
agirmcjjvhedbsyoqfzukmpxwt
ggirmcjvlhedbsyoqfzukypxvt
agirmdjulhekbsyoqfzuknpxwt
agirmcjqlhedbsyoqfztknixwt
agirmcjvjhedbsyomfduknpxwt
agirmcjvlhedbgyoqfzuknpxtq
agirmvjvlhbdbsyfqfzuknpxwt
agirmcjvlhedbsyatfzbknpxwt
agirmcjvlrlybsyoqfzuknpxwt
agirmajvlhedbsqovfzuknpxwt
abinmcrvlhedbsyoqfzuknpxwt
agnrmcjvlhedbsyurfzuknpxwt
agirmpjvlhedbsyoqezuknpxct
agirmxjvlhedbsgoqjzuknpxwt
agirmcjvlhehbstoqfzuknpxht
qgirmcjvlhepcsyoqfzuknpxwt
tgirmcjvlhkdbsyoqszuknpxwt
agirmcjvdhedbscoqftuknpxwt
agbrmcjvlhedbsyoqfzukqpxwj
agurmcjvlhedbsyaqfzuknpxmt
agirmcjvohudbsyoqfmuknpxwt
agirmcjvlhekbsyoqfbuktpxwt
agirmcjvlhedhsyoqfzugnnxwt
agirmcjvlhedbsyjqyzuknpxft
agirmcjvlhedbsymufznknpxwt
agirmcjhlheubsyoqfzuknpxmt
agirmcjvlhwdbsywqfzwknpxwt
agirmcjvljedbsgqqfzuknpxwt
aglrmcjelhedbsyoqfzuknpxkt
agxrmcjvlhxdbsyoqfquknpxwt
agirmcjvnhedbsyoqfzuenfxwt
agirmcjvlhedbsyoqfzatnqxwt
agirmcvvlhedbsboqfzuknuxwt
agirncjvlhezbsyoqfzulnpxwt
agiamcjvdiedbsyoqfzuknpxwt
agirmcjvwhedbskoqfzhknpxwt
agiwmcjflhedbsyoqfzulnpxwt
agirmcjvlhedboyoqfzuknpjwl
agivmcjslhedbsyoqfzdknpxwt
agirmcjvlcedbsyoqfzukepxyt
akirmcjvlhjdbssoqfzuknpxwt
agvrmcjvldedmsyoqfzuknpxwt
agirecjvlhidbsyoqfzukbpxwt
abirmcjvlhjdbsyoqfkuknpxwt
agirmcjelhedbfyoqfzuknpxwj
agirmcjvlhedbbyoqrzukwpxwt
akirmcjvlhedbsyoyfzuknplwt
agirmcjvlhedbsydsfzuknpxwq
agirrcjvlhedbsyoqazuknpmwt
aeirmcjvlhedbsyoqfvuknpwwt
akirmcjvlhedbsyoqpzudnpxwt
agijmcjvlhedbsyuqfzunnpxwt
agirmcjilhedasyoqizuknpxwt
agirmczvlhzdbsyoqfzuknpxwx
agirmcjvlhehbsyoifzuknpxwo
agirwcjvlhedbsyoqfzuenpxst
agirmcjvlhedbsyoquzuknhxft
agirmcqvlkedbsyoqfzrknpxwt
agirmcqvlhenbsyoqfzuknpuwt
agirmcjvleedbsyoqfzhhnpxwt
agirmcjvlhembsyrqfauknpxwt
agirmcjvlhedbssoqflcknpxwt
aqirmcjvlnedbsyoqfzuknpxpt
agirmcjqlhedbxpoqfzuknpxwt
fgirmcjvlhedbsyoqfzukqpqwt
aggrmcjvlhpdbsyoqfzuknpxjt
agirmwjvlhedbsywqfzuknpzwt
agirmcailhembsyoqfzuknpxwt
aglrmcjvlhxdbsyoqfzuknpxet
xgirmcjvlhzdbsyoqfzukrpxwt
agvrmcjvuhedbsyoqfzuknpxgt
agikmcjvlhecbsyoqfzuknpxwr
agyrmcjvlhezbsyoqfouknpxwt
agirmcjvfhjdbsyokfzuknpxwt
agkrmjjvlhedtsyoqfzuknpxwt
agirmgjvlhedbiyoqfzuknpxwv
wcirmcjvlhedbsyoqfzuknpxwo
aairmcjvlhedbstoqfguknpxwt
hgirmcjvlhedwfyoqfzuknpxwt
agirmcjvmhfdbmyoqfzuknpxwt
agirmcjvlhvdbsioqfzuanpxwt
agrrmcjvgsedbsyoqfzuknpxwt
agirmcjvlqetbsysqfzuknpxwt
agirccjvlhedbsyoqfzuknkcwt
agirmqjvlhedbsdoqfzkknpxwt
agirmcjvlheobsyopfzuknpxwg
agirmcjolhedbsyofpzuknpxwt
agirmcjnlhedbsyoqkzukfpxwt
agiumcjvlheabsyoqfzuknpxbt
agipmcjvlhedbsyoqfzukupxwz
atirmcrvlhedbsyoqfnuknpxwt
agirmcjvnhedfkyoqfzuknpxwt
agirmrjvlhedboyoqfzvknpxwt
abhrmcjvlhedbtyoqfzuknpxwt
cbirmcjvlhedbfyoqfzuknpxwt
agirmcjvlhedbsyoqfmwknjxwt
ahirmcjvlhedbsloqfzuknpfwt
agarmjjvlhedbsyoqfzyknpxwt
ajirmcjvlhevjsyoqfzuknpxwt
agirmcjvlhpdbstoqfzuknpewt
agirmcsvlhedbsyoqfbupnpxwt
agirmcjvlhexbsyodfzukqpxwt
auiymcjblhedbsyoqfzuknpxwt
azirmcjvchedbsyoqfziknpxwt
aeirmcjvlhedvsyoqfzuonpxwt
agirmcjvlhedbfyoqfbukjpxwt
ygirmcjvlhidbsyoqfzukncxwt
agirmxpvlhedbsyoqffuknpxwt
ztirmcjvlhedosyoqfzuknpxwt
agirmcjvlhepbsyoqfzuenppwt
agirmcjvshedbsyoqnzaknpxwt
awirmcjvlhydbsyoqfzuknoxwt
ucirmcjvlhedbsyoqfjuknpxwt
agirmwjvlhkbbsyoqfzuknpxwt
agirmcjvldedbsyohfzuknpxzt
agirmcjvwhedbsyoqfznknpxgt
agiricjvlhedxqyoqfzuknpxwt
agirmcjvlhzdbjyoqfzukapxwt
agirmcgvlhedbsyoqfzuknaowt
agidmcjvlhedbsyoqayuknpxwt
agirmcjvlhedisnoqfzuknpxnt
wkjrmcjvlhedbsyoqfzuknpxwt
agirmcjvlhedbuyojfzukxpxwt
agkrmcjvlhedbsybqfzurnpxwt
agirmcjvghedbsyoqfzuknexwj
agirmcjvnhedbsyoqfzuznpxit
agirmcjvlbedbsyoqfiukwpxwt
agirlctvlheabsyoqfzuknpxwt
agirmcjzzhedbsyoqfzcknpxwt
akirmcjvlnedbsyoqfzlknpxwt
agirmdjvlhedpsyoqfzuknpjwt
agiyjcuvlhedbsyoqfzuknpxwt
agirmcbvltedysyoqfzuknpxwt
agirmcjvlhedfdyoqfzubnpxwt
agidmcjvlhedesfoqfzuknpxwt
aeirmcjvlhedqsyoqfxuknpxwt
agifmcjvlhedbsyoqfquknptwt
agidmcjvlhedbsyfqfzuknpxwb
agirvcjvlhedbsroqfzuknjxwt
agirmcqvlhddbsyoqfzuknpxwj
agirmcjvlhmdqsyoqizuknpxwt
atirmcjvltedbsyoqfzuknpxwz
agirxnjvlhedbsyoqfzuknpxkt
agihmcjvlhedbsyoqfzukepxqt
agirmcjvlhedbsmoqzsuknpxwt
agirycjvlhedbuyoqfwuknpxwt
agirmcjvlhedbsyoqfzfkrfxwt
agirzcjvlhedbsyoqfhuknpxnt
agigmcjvlhedbsqnqfzuknpxwt
agirmgzvlhedbsyoqfzuonpxwt
agirmcjvqhedbqyoqfzukqpxwt
anarmcjvlhedbsyocfzuknpxwt
agirmcjuihedbshoqfzuknpxwt
agirdckvlhedbsyoqfzxknpxwt
ugirmujvlhwdbsyoqfzuknpxwt
mgirmcjvlheobsyovfzuknpxwt
agirmcjvghedbsyoqfzufxpxwt
agirmcjvlhedbsyoinzuknuxwt
agirmzjvlhbdbsyoqfzlknpxwt
agivmcjvlhedbsconfzuknpxwt
agirmwfvlhedtsyoqfzuknpxwt
agirmcjvlhedbbyoqrzukncxwt
agirmcjvlhelbsyoqfzupnlxwt
agirmmjvluedqsyoqfzuknpxwt
agjrmcjvlhedbsyaqfcuknpxwt
agiwmcjvlhedbsyoqzzuknpswt
agirxcjvlhedbsyoqfyvknpxwt
agirmljvlhedbsyoqkzuknpxjt
agirmcjvchedbsyoqfzmknyxwt
agirmcjvlhedbsyovfzuynpxwl
agtrmcjvlhedysyoqfzuknexwt
agirmcjvmhedbslonfzuknpxwt
agirmcjfshedbsyoqfziknpxwt
agirmcjvlhedbsygqfzkknpbwt
agyrmcivlhedbsyovfzuknpxwt
agirmcjvghedbsyoqjzuknkxwt
agirmcjvlhedqsyoqfzukspxmt
ayirmcjvhhedbsyomfzuknpxwt
agirmcjvlnembsypqfzuknpxwt
agirmcjqlhedbsyuvfzuknpxwt
agirmcjvlhembsybqfzuknpxwa
agirjcfvlhedbsyoqfuuknpxwt
agirmcjvohedbsyowfzuknxxwt
agirmcjvlhedroyoqfzukncxwt
agrrmijvlhedbsyoqfnuknpxwt
agirmjjvlhsdbsyoqfzumnpxwt
agirrcjvnhedbsyoqfzuktpxwt
agirmcjvlzedjsyoqfzuknpdwt
agirmkjvlhedbsyoqfzxinpxwt
agirmcjvlhedbzyojfzuknpvwt
arirmcjvlheddsyoqfzuknrxwt
agirmcjvlhedbsyoqhzuanpxmt
agirmcjvluedbsyoqozuknwxwt
afirmcjwlhedxsyoqfzuknpxwt
agirmcjvlhefbsyoqfkuinpxwt
agirycjvltedbsypqfzuknpxwt
agirmrxvlhedbsyoqfzeknpxwt
agfrmcqvlhedbsyoqxzuknpxwt
agormcjvuhexbsyoqfzuknpxwt
agyrmcjvehddbsyoqfzuknpxwt
agirmcjvlheqbsynqfzgknpxwt
agirmcjvlhedbsloufwuknpxwt
tgirmcjvlwedbsyoqfzuknpqwt
agirmcjvlhesbzyogfzuknpxwt
agitmdjvlhedpsyoqfzuknpjwt
bgirmejvlhtdbsyoqfzuknpxwt
aginmcjvlhedzsyoqfzuknoxwt
agvrzcjvlhedbsuoqfzuknpxwt
agormcjvlhedbsyoqfzuknpodt
agirmcevlhedbgyojfzuknpxwt
agirmcjblhedboytqfzuknpxwt
qgibmcjvlhedbsyoqfzuknbxwt
agirmcjvlhedbsyoafzutnnxwt
agiamcjvchkdbsyoqfzuknpxwt
agirmcjvehedblyoqwzuknpxwt
agirmcpvlhwdbsyoafzuknpxwt
agirmcjvlhtdbsyoqfzumnpxtt
agirmcjalhegtsyoqfzuknpxwt
agirdijvlhedbsyoqfzutnpxwt
agirmckvlhgdbsyovfzuknpxwt
qgmrmcjvlkedbsyoqfzuknpxwt
agirjcjvlhodbsyoqfzuanpxwt
ajirmcjvlhedbpyoqftuknpxwt
cgirmcjvlhedbsyoqfiuonpxwt
ayirmcjvlhedbsyaqfzuknwxwt
agirmcjvlhedbdyoqbzwknpxwt
)";

    const std::string input2 = R"(
abcdef
bababc
abbcde
abcccd
aabcdd
abcdee
ababab
    )";

    std::stringstream ss(input);
    std::string line;
    std::vector<std::string> lines;
    int r{0};
    int n3{0};
    int n2{0};
    while (std::getline(ss, line)) {
        lines.push_back(line);
        std::map<char, bool> letters;
        bool b2{false};
        bool b3{false};
        for (auto c : line) {
            if (letters[c] == false) {
                letters[c] = true;

                int count{0};
                for (auto c2 : line) {
                    if (c == c2) {
                        ++count;
                    }
                }
                if (count == 2) {
                    b2 = true;
                }
                if (count == 3) {
                    b3 = true;
                }
            }
        }

        if (b2) {
            ++n2;
        }
        if (b3) {
            ++n3;
        }
    }

    r = n2 * n3;

    std::cout << n2 << std::endl;
    std::cout << n3 << std::endl;
    std::cout << r << std::endl;

    for (auto &line : lines) {

        for (auto &line2 : lines) {
            int diff = line.length();
            for (int c{0}; c < line.length(); ++c) {
                if (line[c] == line2[c]) {
                    --diff;
                }
            }

            if (diff == 1) {
                std::cout << line << std::endl;
                std::cout << line2 << std::endl;
                for (int c{0}; c < line.length(); ++c) {
                    if (line[c] == line2[c]) {
                        std::cout << line[c];
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}
