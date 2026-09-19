#include "tree.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

int main() {
  // Read long sample XML from file
  std::ifstream ifs("tests/sample_long.xml");
  assert(ifs.is_open());
  std::string xml((std::istreambuf_iterator<char>(ifs)),
                  std::istreambuf_iterator<char>());
  ifs.close();

  Tree t(xml);

  // Minify -> file
  bool ok = t.write_minify_to_file("tests/out_minify.xml");
  assert(ok);
  std::ifstream ifm("tests/out_minify.xml");
  assert(ifm.is_open());
  std::string mincontent((std::istreambuf_iterator<char>(ifm)),
                         std::istreambuf_iterator<char>());
  ifm.close();
  assert(mincontent.size() > 0);

  // Prettify -> file
  ok = t.write_prettify_to_file("tests/out_pretty.xml");
  assert(ok);
  std::ifstream ifp("tests/out_pretty.xml");
  assert(ifp.is_open());
  std::string pretty((std::istreambuf_iterator<char>(ifp)),
                     std::istreambuf_iterator<char>());
  ifp.close();
  assert(pretty.size() > 0);

  // JSON -> file
  ok = t.write_json_to_file("tests/out.json");
  assert(ok);
  std::ifstream ifj("tests/out.json");
  assert(ifj.is_open());
  std::string json((std::istreambuf_iterator<char>(ifj)),
                   std::istreambuf_iterator<char>());
  ifj.close();
  assert(json.size() > 0);

  std::cout << "tests/test_tree: OK\n";
  return 0;
}
