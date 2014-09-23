#include <fstream>
#include <glog/logging.h>
#include "narray/io/file_loader.h"
#include "op/impl/basic.h"
#include "system/minerva_system.h"

using namespace std;

namespace minerva {

void FileLoaderOp::Execute(const DataList&, const DataList& outputs, const Context& context) {
  CHECK_EQ(context.impl_type, ImplType::kBasic) << "file loader operator only has basic implementation";
  closure.loader->Load(closure.fname, closure.size, outputs);
}

string FileLoaderOp::Name() const {
  stringstream ss;
  ss << "load(" << closure.fname << ")";
  return ss.str();
}

void SimpleFileLoader::Load(const string& fname, const Scale& size, const DataList& out_shards) {
  CHECK_EQ(out_shards.size(), 1) << "(simple file loader) #outputs is wrong";
  size_t numvalue = size.Prod();
  float* ptr = new float[numvalue];
  ifstream fin(fname.c_str());
  fin.read(reinterpret_cast<char*>(ptr), numvalue * sizeof(float));
  fin.close();
  memcpy(out_shards[0].data(), ptr, numvalue * sizeof(float));
  delete[] ptr;
}

}  // end of namespace minerva

