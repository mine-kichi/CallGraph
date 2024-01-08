// CallGraph.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

// Graphviz
#define GVDLL
#include <graphviz/gvc.h>


// ファイルからファイル名を抽出し、再帰的に検索する関数
void searchFileNames(const std::string& filename, std::vector<std::string>& foundFiles) {
    std::ifstream file(filename);
    std::string line;
    std::regex filePattern("ここに適切なファイル名の正規表現");

    while (std::getline(file, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, filePattern)) {
            for (const auto& match : matches) {
                std::string foundFile = match.str();
                foundFiles.push_back(foundFile);

                // 再帰的に検索
                searchFileNames(foundFile, foundFiles);
            }
        }
    }
}

int main()
{
    std::cout << "Hello World!\n";
    std::vector<std::string> foundFiles;
    searchFileNames("指定のファイル名", foundFiles);

    // 結果の表示
    for (const auto& file : foundFiles) {
        std::cout << file << std::endl;
    }

    const std::vector<std::string> nodes
    {
            u8"a",
            u8"b",
            u8"c",
            u8"d",
            u8"e",
            u8"f",
            u8"g",
            u8"h",
            u8"i"
    };

    const std::vector<std::tuple<int, int, std::string>> edges
    {
        {0, 1, u8"1"},
        { 0, 2, u8"1" },
        { 1, 4, u8"1" },
        { 2, 1, u8"2" },
        { 2, 3, u8"2" },
        { 2, 3, u8"3" },
        { 2, 8, u8"2" },
        { 4, 2, u8"4" },
        { 4, 5, u8"5" },
        { 5, 4, u8"6" },
        { 6, 4, u8"7" },
        { 6, 7, u8"8" },
        { 7, 4, u8"9" },
        { 8, 4, u8"10" }
    };

    // グラフ作成

    // 2番目の引数
    // 有向グラフはAgdirected、無向グラフはAgundirected
    Agraph_t* g = agopen(nullptr, Agdirected, nullptr);

    // フォント指定
    agattr(g, AGNODE, const_cast<char*>("fontname"), "Meiryo");
    agattr(g, AGEDGE, const_cast<char*>("fontname"), "Meiryo");

    // node
    std::unordered_map<std::string, Agnode_t*> node;

    for (const std::string& n : nodes)
    {
        // 2番目の引数はID
        // 既存の場合、そのAgnode_tを返す
        // IDを自動採番する場合はnullptr

        // 3番目の引数
        // IDが存在しない場合
        // trueはAgnode_tを新規作成
        // falseはnullptrを返す
        node[n] = agnode(g, nullptr, true);

        // ラベル
        // 設定しない場合、IDを表示
        agsafeset(node[n], const_cast<char*>("label"), n.c_str(), "");
    }

    // nodeの形状の、デフォルト以外を指定
    agsafeset(node[nodes[2]], const_cast<char*>("shape"), "doublecircle", "");
    agsafeset(node[nodes[4]], const_cast<char*>("shape"), "doublecircle", "");

    // edge
    for (const std::tuple<int, int, std::string>& e : edges)
    {
        // 2番目の引数 -> 3番目の引数
        // 各nodeのAgnode_t

        // 4番目の引数はedgeを識別するキー
        // nullptrは毎回異なるキーとして扱う

        // 2,3,4番目の引数の組み合わせが既存の場合、同じedge

        // 5番目の引数
        // 組み合わせが存在しない場合
        // trueはedgeを新規作成
        // falseはnullptrを返す
        Agedge_t* edge = agedge(g, node[nodes[std::get<0>(e)]], node[nodes[std::get<1>(e)]], nullptr, true);

        // ラベル
        agsafeset(edge, const_cast<char*>("label"), std::get<2>(e).c_str(), "");

    }

    // dotフォーマットにレイアウト
    GVC_t* gvc = gvContext();
    gvLayout(gvc, g, "dot");

    // デバッグのため、内部にて作成されたdotをファイル出力
    //gvRenderFilename(gvc, g, "canon", "test.dot");

    // PNGにてファイル出力
    // bmp、gif、jpg、pdf、svg、tifに変更すると、それぞれのフォーマットにて出力
    gvRenderFilename(gvc, g, "png", "test.png");

    // 解放
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    return 0;
}
