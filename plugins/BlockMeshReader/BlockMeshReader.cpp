// Copyright (C) 2010-2011 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#include "Common/Log.hpp"
#include "Common/CBuilder.hpp"

#include "Mesh/BlockMesh/BlockData.hpp"

#include "BlockMeshReader.hpp"
#include "LibBlockMeshReader.hpp"
#include "Parser.hpp"

namespace cf3 {
namespace BlockMeshReader {

using namespace cf3::common;
using namespace cf3::Mesh;
using namespace cf3::Mesh::BlockMesh;

////////////////////////////////////////////////////////////////////////////////

cf3::common::ComponentBuilder < BlockMeshReader, Mesh::CMeshReader, LibBlockMeshReader > aBlockMeshReader_Builder;

//////////////////////////////////////////////////////////////////////////////

BlockMeshReader::BlockMeshReader(const std::string& name): CMeshReader(name)
{

}

std::vector< std::string > BlockMeshReader::get_extensions()
{
  std::vector< std::string > result(1, ".dict");
  return result;
}

void BlockMeshReader::do_read_mesh_into(const cf3::common::URI& path_uri, cf3::Mesh::CMesh& mesh)
{
  boost::filesystem::path path(path_uri.path());
  // if the file is present open it
  boost::filesystem::fstream file;
  if( boost::filesystem::exists(path) )
  {
    CFLog(VERBOSE, "Opening file " <<  path.string() << "\n");
    file.open(path,std::ios_base::in); // exists so open it
  }
  else // doesnt exist so throw exception
  {
     throw boost::filesystem::filesystem_error( path.string() + " does not exist",
                                                boost::system::error_code() );
  }

  BlockData& block_data = create_component<BlockData>("block_data");
  parse_blockmesh_dict(file, block_data);
  block_data.dimension = 3; // TODO add true 2D support
  build_mesh(block_data, mesh);
}

} // BlockMeshReader
} // cf3
