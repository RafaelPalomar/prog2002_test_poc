#include "renderdoc_helper.hpp"
#include <fmt/core.h>
#include <Windows.h>
#include <map>
//extern "C" {
//#include "../external/struct/include/struct/struct.h"
#include "../external/cppystruct/include/cppystruct.h"
#include "float16.h"
#include "formatpacking.h"
//}

FloatVector InterpretVertex(const byte* data, uint32_t vert,
	uint32_t vertexByteStride, const ResourceFormat& fmt,
	const byte* end, bool& valid) {
	data += vert * vertexByteStride;

	if (data + fmt.ElementSize() > end)
	{
		valid = false;

		if (fmt.compType == CompType::UInt || fmt.compType == CompType::SInt || fmt.compCount == 4)
			return FloatVector(0.0f, 0.0f, 0.0f, 0.0f);

		return FloatVector(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return DecodeFormattedComponents(fmt, data);
}

rdcarray<float> unpack_data(ResourceFormat fmt, bytebuf data, int data_offset);

rdcstr conv(const std::string &s) {
	return { s.c_str(), s.size() };
}

std::string conv(const rdcstr &s) {
	return { s.begin(), s.end() };
}

RenderDocHelper::RenderDocHelper(std::string rdc_path) : m_rdc_path(std::move(rdc_path)) { }

RenderDocHelper::~RenderDocHelper() {
	if (m_capture_file) {
		m_capture_file->Shutdown();
	}
	if (m_controller) {
		m_controller->Shutdown();
	}
	RENDERDOC_ShutdownReplay();
}

bool RenderDocHelper::open_capture() {

	auto global_env = GlobalEnvironment{};
	global_env.enumerateGPUs = false;
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	RENDERDOC_InitialiseReplay(global_env,{});

	m_capture_file = RENDERDOC_OpenCaptureFile();
	if (!m_capture_file) {
		fmt::print("Failed to open capture file");
		return false;
	}

    ReplayStatus st = m_capture_file->OpenFile(conv(m_rdc_path), "rdc",nullptr);
	if (st != ReplayStatus::Succeeded) {
		fmt::print("Failed to open file: {}", m_rdc_path);
		return false;
	}

	if (m_capture_file->LocalReplaySupport() != ReplaySupport::Supported) {
		fmt::print("Local replay not supported\n");
		return false;
	}
	
	const auto& [status, controller] = m_capture_file->OpenCapture(ReplayOptions{}, nullptr);
	if (status != ReplayStatus::Succeeded) {
		fmt::print("Failed to open capture: {}\n", static_cast<int>(status));
		return false;
	}

	m_controller = controller;

	m_structured_data = &m_capture_file->GetStructuredData();

	return true;
}

void print_buf(bytebuf buf, uint32_t stride, ResourceFormat format) {
	int totaloff = 0;
	for (int j = 1; j < 5; j++) {
		for (int i = 0; i < format.compCount; i++) {
			//auto off = i * postvs_data.vertexByteStride + postvs_data.vertexByteOffset;
			int off = totaloff +  i * format.compByteWidth;
			fmt::print("{} ", *(float*)(buf.data() + off));
		}
		fmt::print("\n");
		totaloff += stride;
		if (totaloff == buf.size()) {
			break;
		}

	}
}
void parse_buffer(IReplayController* m_controller, bytebuf data_buf, ResourceFormat format, uint32_t stride) {
	uint32_t idx = 0;

	const auto& pipeline = m_controller->GetPipelineState();
	auto topo = pipeline.GetPrimitiveTopology();
	if (topo == Topology::TriangleFan && pipeline.IsStripRestartEnabled()) {
		topo = Topology::TriangleList;

		if (idx > 2) {
			idx = (idx - 1) * 3 - 1;
		}
	}


	//bool valid = false;
	//auto interpeted = InterpretVertex(data.data(),
	//	0,
	//	stride,
	//	format,
	//	data_end,
	//	valid);
	byte* data = data_buf.data();
	byte* data_end = data + data_buf.size();
	bool valid = false;


	rdcarray<FloatVector> activePrim;
	// Reference for how primitive topologies are laid out:
	// http://msdn.microsoft.com/en-us/library/windows/desktop/bb205124(v=vs.85).aspx
	// Section 19.1 of the Vulkan 1.0.48 spec
	// Section 10.1 of the OpenGL 4.5 spec
	if (topo == Topology::LineList)
	{
		uint32_t v = uint32_t(idx / 2) * 2;    // find first vert in primitive

		activePrim.push_back(InterpretVertex(data, v + 0, stride, format, data_end, valid));
		activePrim.push_back(InterpretVertex(data, v + 1, stride, format, data_end, valid));
	}
	else if (topo == Topology::TriangleList)
	{
		uint32_t v = uint32_t(idx / 3) * 3;    // find first vert in primitive

		activePrim.push_back(InterpretVertex(data, v + 0, stride, format, data_end, valid));
		activePrim.push_back(InterpretVertex(data, v + 1, stride, format, data_end, valid));
		activePrim.push_back(InterpretVertex(data, v + 2, stride, format, data_end, valid));
	}

	for (auto prim : activePrim) {
		fmt::print("{}, {}, {}, {}\n", prim.x, prim.y, prim.z, prim.w);
	}
}
void RenderDocHelper::dump_actions() {
	for (auto action : m_controller->GetRootActions()) {
		fmt::print("{}\n", action.GetName(*m_structured_data).c_str());
	}
	auto action = find_action_by_name("glDrawArrays()");

	m_controller->SetFrameEvent(action.eventId, true);
	auto data = get_vsin_mesh_data(action);

	//auto vsin_attrs = get_vsin_attrs(action.vertexOffset, )
	//get_vsin(action);

	auto postvs_data = m_controller->GetPostVSData(0, 0, MeshDataStage::VSOut);
	auto vertices_data = m_controller->GetBufferData(postvs_data.vertexResourceId, postvs_data.vertexByteOffset, 0);
	auto format = postvs_data.format;

	auto dd = unpack_data(format, vertices_data, 0);
	//print_buf(vertices_data, postvs_data.vertexByteStride, format);

	const auto& pipeline = m_controller->GetPipelineState();
	//auto top = pipeline.GetPrimitiveTopology();

	//bool valid = false;
	//auto interpeted = InterpretVertex(vertices_data.data(),
	//	0,
	//	format.compCount * format.compByteWidth,
	//	format,
	//	vertices_data.data() + vertices_data.size(),
	//	valid);
	//int bpp = 0;
	parse_buffer(m_controller, vertices_data, format, postvs_data.vertexByteStride);
	//	int totaloff = 0;
	//for (int j = 1; j < 4; j++) {
	//	for (int i = 0; i < format.compCount; i++) {
	//		//auto off = i * postvs_data.vertexByteStride + postvs_data.vertexByteOffset;
	//		int off = totaloff +  i * format.compByteWidth;
	//		fmt::print("{} ", *(float*)(vertices_data.data() + off));
	//	}
	//	totaloff += postvs_data.vertexByteStride;
	//	fmt::print("\n");
	//	
	//}
	//postvs_data

	auto frame_info = m_controller->GetFrameInfo();
	auto gl_pipeline_state = m_controller->GetGLPipelineState();
	//gl_pipeline_state->g
	auto vbuffers = pipeline.GetVBuffers();
	auto vertex_inputs = pipeline.GetVertexInputs();
	auto glvinput = gl_pipeline_state->vertexInput;
	for (auto buf : glvinput.vertexBuffers) {
	}
	fmt::print("\n");
	auto glvinput_attribs = glvinput.attributes;
	for (auto attrib : glvinput_attribs) {
		//auto f = attrib.genericValue.floatValue;
		//fmt::print("{}, {}, {}, {}\n", f[0], f[1], f[2], f[3]);
		auto buf = glvinput.vertexBuffers.at(attrib.vertexBufferSlot);
		if (buf.resourceId == ResourceId::Null()) {
			continue;
		}

		vertices_data = m_controller->GetBufferData(buf.resourceId, buf.byteOffset, 0);
		if (vertices_data.empty()) {
			continue;
		}

		print_buf(vertices_data, buf.byteStride, attrib.format);
	}
	//gl_pipeline_state->vertexProcessing;
	//for (auto vinput : vertex_inputs) {
	//	fmt::print("{}\n", vinput.name.c_str());
	//}
	//FloatVector
	//auto vert = m_controller->DebugVertex()
	//	vert->debugger->
	//auto windowing_data = CreateHeadlessWindowingData(100, 100);
	//auto output = m_controller->CreateOutput(windowing_data, ReplayOutputType::Mesh);
	//auto ver = output->PickVertex(0, 0);

	bool success = false;
	//auto components = DecodeFormattedComponents(postvs_data.format, vertices_data.data(), &success);
	int bp = 0;
}

size_t RenderDocHelper::drawcalls_count() const {
	return m_controller->GetRootActions().size();
}

MeshData RenderDocHelper::get_vsin_mesh_data(ActionDescription action, int first_index, int num_indices) {
	const auto ib = m_controller->GetPipelineState().GetIBuffer();

	if (num_indices == 0)
		num_indices = action.numIndices;
	else
		num_indices = std::min<int>(num_indices, action.numIndices);

	const auto ioffs = action.indexOffset * ib.byteStride;

	MeshData mesh{};
	mesh.mesh.numIndices = num_indices;
	mesh.mesh.indexByteOffset = ib.byteOffset + ioffs;
	mesh.mesh.indexByteStride = ib.byteStride;
	mesh.mesh.indexResourceId = ib.resourceId;
	mesh.mesh.baseVertex = action.baseVertex;

	if (ib.byteSize > ioffs)
		mesh.mesh.indexByteSize = ib.byteSize - ioffs;
	else
		mesh.mesh.indexByteSize = 0;

	if (!(action.flags & ActionFlags::Indexed))
		mesh.mesh.indexByteOffset = 0;
	mesh.mesh.indexByteStride = 0;
	mesh.mesh.indexResourceId = ResourceId::Null();

	auto attrs = get_vsin_attrs(action.vertexOffset, mesh.mesh);

	first_index = std::min<int>(first_index, action.numIndices - 1);

	auto indices = fetch_indices(mesh.mesh, 0, first_index, num_indices);

	mesh.attrs = attrs;
	mesh.first_index = first_index;
	mesh.indices = indices;

	return mesh;
}
rdcarray<MeshAttribute> RenderDocHelper::get_vsin_attrs(int vertexOffset, MeshFormat index_mesh) {
	const auto& pipe = m_controller->GetPipelineState();
	auto inputs = pipe.GetVertexInputs();

	rdcarray<MeshAttribute> attrs;
	auto vbs = pipe.GetVBuffers();

	for (auto a : inputs) {
		if (!a.used) {
			continue;
		}

		auto attr = MeshAttribute();
		attr.name = a.name;
		attr.mesh = MeshFormat(index_mesh);

		attr.mesh.vertexByteStride = vbs[a.vertexBuffer].byteStride;
		attr.mesh.instStepRate = a.instanceRate;
		attr.mesh.instanced = a.perInstance;
		attr.mesh.vertexResourceId = vbs[a.vertexBuffer].resourceId;

		auto offs = a.byteOffset + vertexOffset * attr.mesh.vertexByteStride;

		attr.mesh.vertexByteOffset = vbs[a.vertexBuffer].byteOffset + offs;
		attr.mesh.vertexByteSize = std::max<uint64_t>(0, vbs[a.vertexBuffer].byteSize - offs);

		attr.mesh.format = a.format;

		attrs.push_back(attr);
	}

	return attrs;
}
rdcarray<uint32_t> RenderDocHelper::fetch_indices(MeshFormat mesh, int index_offset, int first_index, int num_indices) {

	const auto& pipe = m_controller->GetPipelineState();
	auto restart_idx = pipe.GetStripRestartIndex() & ((1 << (mesh.indexByteStride * 8)) - 1);
	auto restart_enabled = pipe.IsStripRestartEnabled() && IsStrip(pipe.GetPrimitiveTopology());

	rdcarray<uint32_t> indices{ };
	// If we have an index buffer
	if (mesh.indexResourceId != ResourceId::Null()) {
		auto offset = mesh.indexByteStride * (first_index + index_offset);

		auto avail_bytes = mesh.indexByteSize;
		if (avail_bytes > offset) {
			avail_bytes = avail_bytes - offset;
		}
		else {

			avail_bytes = 0;
		}

		auto read_bytes = std::min<uint64_t>(avail_bytes, mesh.indexByteStride * num_indices);

		bytebuf ibdata{};
		// Fetch the data
		if (read_bytes > 0) {
			ibdata = m_controller->GetBufferData(mesh.indexResourceId,
				mesh.indexByteOffset + offset,
				read_bytes);
		}

		// Get the character for the width of index
		std::string index_fmt = "B";
		if (mesh.indexByteStride == 2) {

			index_fmt = "H";
		}
		else if (mesh.indexByteStride == 4) {
			index_fmt = "I";
		}

		auto avail_indices = int((ibdata.size()) / mesh.indexByteStride);

		// Duplicate the format by the number of indices
		index_fmt = "=" + (std::min<int>(avail_indices, num_indices)) + index_fmt;

		// Unpack all the indices
		//indices
		//struct_unpack(ibdata, index_fmt, indices.data());
		//auto [type, code, checksum, p_id, sequence] = pystruct::unpack(index_fmt.c_str(), ibdata);

		for (int i = 0; i < avail_indices; i++) {
			uint32_t index = (uint32_t)(ibdata.data() + i * mesh.indexByteStride);
			indices.push_back(index);
		}
		//auto extra = [];
		if (avail_indices < num_indices) {
			//extra = [None] * (num_indices - avail_indices)

			// Apply the baseVertex offset
			//return[i if restart_enabledand i == restart_idx else i + mesh.baseVertex for i in indices] + extra
		}
	}
	else {
		// With no index buffer, just generate a range
		//return tuple(range(first_index, first_index + num_indices));
		for (int i = 0; i < num_indices; i++) {
			indices.push_back(first_index + i);
		}
	}
	return indices;
}

struct MUInt {
	byte p;
	unsigned char b;
	unsigned short c;
	byte d;
	unsigned int e;
	byte f;
	byte g;
	byte h;
	unsigned long long i;
};
			//// byte, signed char, short, byte, int, byte, byte, byte, long long
			//{CompType::SInt, "xbhxixxxq"},
			//// byte, byte, Float16, byte, float, byte, byte, double
			//{CompType::Float, "xxexfxxxd"},  // only 2, 4 and 8 are valid
struct MFloat {
	byte a;
	byte b;
	Float16 c;
	byte d;
	float e;
	byte f;
	byte g;
	byte h;
	double i;
};
//// Unpack a tuple of the given format, from the data
rdcarray<float> unpack_data(ResourceFormat fmt, bytebuf data, int data_offset) {
	// We don't handle 'special' formats - typically bit-packed such as 10:10:10:2
	if (fmt.Special()) {
		//raise RuntimeError("Packed formats are not supported!")
	}

	std::map<CompType, rdcstr> format_chars = { {
			//                   012345678
			// byte, unsigned char, unsigned short, byte, unsigned int, byte, byte, byte, unsigned long long
			{CompType::UInt,  "xBHxIxxxQ"},
			// byte, signed char, short, byte, int, byte, byte, byte, long long
			{CompType::SInt, "xbhxixxxq"},
			// byte, byte, Float16, byte, float, byte, byte, byte, double
			{CompType::Float, "xxexfxxxd"},  // only 2, 4 and 8 are valid
	} };

	// These types have identical decodes, but we might post - process them
	format_chars[CompType::UNorm] = format_chars[CompType::UInt];
	format_chars[CompType::UScaled] = format_chars[CompType::UInt];
	format_chars[CompType::SNorm] = format_chars[CompType::SInt];
	format_chars[CompType::SScaled] = format_chars[CompType::SInt];

	// We need to fetch compCount components
	auto vertex_format = "=" + fmt.compCount + format_chars[fmt.compType][fmt.compByteWidth];

	if (data_offset >= data.size()) {
		return {};
	}
	rdcarray<float> value;
	MFloat ff = *(MFloat*)data.data();


	// Unpack the data
//	try :
//	value = struct.unpack_from(vertex_format, data, data_offset)
//	except struct.error as ex :

//raise
	auto thing = pystruct::unpack(PY_STRING("=4f"), std::string(data.begin(), data.end()));
	bool valid = false;
	//auto interpeted = InterpretVertex(data.data(), 0, fmt.compCount * fmt.compByteWidth, fmt, data.data() + data.size(), valid);

	// If the format needs post - processing such as normalisation, do that now
	if (fmt.compType == CompType::UNorm) {
		auto divisor = float((1 << (fmt.compByteWidth * 8)) - 1);
		//value = tuple(float(i) / divisor for i in value);
		rdcarray<float> new_value;
		for (int i = 0; i < value.size();i++) {
			new_value.push_back(value.at(i) / divisor);
		}
		value = new_value;
	}
	else if (fmt.compType == CompType::SNorm) {
		auto max_neg = -(1 << (fmt.compByteWidth * 8 - 1));
		auto divisor = -float(max_neg + 1);

		//value = tuple(-1.0 if (i == max_neg) else float(i / divisor) for i in value);
		rdcarray<float> new_value;
		for (int i = 0; i < value.size(); i++) {
			if (i == max_neg) {
				new_value.push_back(-1);
			} else {
				new_value.push_back(float(i / divisor));
			}
		}
		value = new_value;
	}
	else if (fmt.compType == CompType::UScaled || fmt.compType == CompType::SScaled) {
		rdcarray<float> new_value;

		// value = tuple(float(i) for i in value);

		// This just casts to float
		for (int i = 0; i < value.size(); i++) {
			new_value.push_back(value.at(i));
		}
		value = new_value;
	}

	// If the format is BGRA, swap the two components
	if (fmt.BGRAOrder()) {
		rdcarray<float> new_value;
		//value = tuple(value[i] for i in[2, 1, 0, 3]);
		std::array<int, 4> indices{ 2, 1, 0, 3 };
		for (int i = 0; i < value.size(); i++) {
			new_value.push_back(value.at(indices.at(i)));
		}
		value = new_value;
	}
	return value;
}
void decode_mesh_data(IReplayController* controller, rdcarray<uint32_t> indices, rdcarray<uint32_t> display_indices,
	rdcarray<MeshAttribute> attrs,
	int instance = 0,
	int indexOffset = 0) {
	//ret = []
	rdcarray<std::map<rdcstr, int>> ret{};

	std::map<ResourceId, std::pair<uint64_t, uint64_t>> buffer_ranges = {};
	for (auto attr : attrs) {
		auto begin = attr.mesh.vertexByteOffset;
		auto end = std::min<uint64_t>(begin + attr.mesh.vertexByteSize, 0xffffffffffffffff);

		// This could be more optimal if we figure out the lower / upper bounds of any attribute and only fetch the
		// data we need.For each referenced buffer, pick the attribute that references the largest rangeand fetch that
		if (buffer_ranges.find(attr.mesh.vertexResourceId) != buffer_ranges.end()) {

			auto buf_range = buffer_ranges[attr.mesh.vertexResourceId];

			if (buf_range.first < begin)
				begin = buf_range.first;
			if (buf_range.second > end)
				end = buf_range.second;
		}

		buffer_ranges[attr.mesh.vertexResourceId] = std::make_pair(begin, end);

	}
	std::map<ResourceId, bytebuf> buffer_data{};
	for (auto [buf, buf_range] : buffer_ranges) {
		//for buf, buf_range in buffer_ranges.items() :
		buffer_data[buf] = controller->GetBufferData(buf, buf_range.first, buf_range.second - buf_range.first);
	}

	// Calculate the strip restart index for this index width
	int striprestart_index = -1;
	//if controller.GetPipelineState().IsStripRestartEnabled() and attrs[0].mesh.indexResourceId != rd.ResourceId.Null()
	if (controller->GetPipelineState().IsStripRestartEnabled() && attrs[0].mesh.indexResourceId != ResourceId::Null()) {
		striprestart_index = (controller->GetPipelineState().GetStripRestartIndex() &
			((1 << (attrs[0].mesh.indexByteStride * 8)) - 1));
	}

	for (int i = 0; i < indices.size(); i++) {
		auto idx = indices[i];
		std::map<rdcstr, int> vertex = { {
				{"vtx", i},
				{"idx", display_indices[i]}
			} };

		//if striprestart_index is None or idx != striprestart_index :
		if (striprestart_index == -1 || idx != striprestart_index) {
			for (auto attr : attrs) {
				// TODO:
				if (idx == -1) {
					//vertex[attr.name] = None
					continue;

				}

				auto offset = attr.mesh.vertexByteStride * idx;

				if (attr.mesh.instanced) {
					offset = (attr.mesh.vertexByteStride +
						attr.mesh.vertexByteStride * int(instance / std::max<int>(attr.mesh.instStepRate, 1)));
				}

				/*vertex[attr.name]*/
				auto thing = unpack_data(attr.mesh.format, buffer_data[attr.mesh.vertexResourceId],
					attr.mesh.vertexByteOffset + offset -
					buffer_ranges[attr.mesh.vertexResourceId].first);
			}
		}

	}

	//ret.append(vertex)

				//return ret;
}
void RenderDocHelper::get_vsin(ActionDescription action, int first_index, int num_indices, int instance, int view) {
	const auto& ib = m_controller->GetPipelineState().GetIBuffer();

	if (num_indices == 0)
		num_indices = action.numIndices;
	else
		num_indices = std::min<int>(num_indices, action.numIndices);

	auto ioffs = action.indexOffset * ib.byteStride;

	MeshFormat mesh{};
	mesh.numIndices = num_indices;
	mesh.indexByteOffset = ib.byteOffset + ioffs;
	mesh.indexByteStride = ib.byteStride;
	mesh.indexResourceId = ib.resourceId;
	mesh.baseVertex = action.baseVertex;

	if (ib.byteSize > ioffs)
		mesh.indexByteSize = ib.byteSize - ioffs;
	else
		mesh.indexByteSize = 0;

	if (!(action.flags & ActionFlags::Indexed)) {
		mesh.indexByteOffset = 0;
		mesh.indexByteStride = 0;
		mesh.indexResourceId = ResourceId::Null();
	}

	auto attrs = get_vsin_attrs(action.vertexOffset, mesh);

	first_index = std::min<int>(first_index, action.numIndices - 1);

	auto indices = fetch_indices(mesh, 0, first_index, num_indices);

	return decode_mesh_data(m_controller, indices, indices, attrs, 0, 0);
}


//struct HighlightCache
//{
//  HighlightCache() : cacheKey(0), idxData(false) {}
//  //IRemoteDriver *driver = NULL;
//
//  //uint64_t cacheKey;
//
//  //bool idxData;
//  //bytebuf vertexData;
//  //rdcarray<uint32_t> indices;
//
//  //void CacheHighlightingData(uint32_t eventId, const MeshDisplay &cfg);
//
//  //bool FetchHighlightPositions(const MeshDisplay &cfg, FloatVector &activeVertex,
//  //                             rdcarray<FloatVector> &activePrim,
//  //                             rdcarray<FloatVector> &adjacentPrimVertices,
//  //                             rdcarray<FloatVector> &inactiveVertices);
//
//  static FloatVector InterpretVertex(const byte *data, uint32_t vert, uint32_t vertexByteStride,
//                                     const ResourceFormat &fmt, const byte *end, bool &valid);
//
//  FloatVector InterpretVertex(const byte *data, uint32_t vert, const MeshDisplay &cfg,
//                              const byte *end, bool useidx, bool &valid);
//};
//FloatVector HighlightCache::InterpretVertex(const byte *data, uint32_t vert, const MeshDisplay &cfg,
//                                            const byte *end, bool useidx, bool &valid)
//{
//  FloatVector ret(0.0f, 0.0f, 0.0f, 1.0f);
//
//  if(cfg.position.format.compType == CompType::UInt ||
//     cfg.position.format.compType == CompType::SInt || cfg.position.format.compCount == 4)
//    ret.w = 0.0f;
//
//  if(useidx && /*idxData*/ false)
//  {
//    if(vert >= (uint32_t)indices.size())
//    {
//      valid = false;
//      return ret;
//    }
//
//    vert = indices[vert];
//
//    if(SupportsRestart(cfg.position.topology) && cfg.position.topology != Topology::TriangleFan &&
//       cfg.position.allowRestart)
//    {
//      if((cfg.position.indexByteStride == 1 && vert == 0xff) ||
//         (cfg.position.indexByteStride == 2 && vert == 0xffff) ||
//         (cfg.position.indexByteStride == 4 && vert == 0xffffffff))
//      {
//        valid = false;
//        return ret;
//      }
//    }
//  }
//
//  return HighlightCache::InterpretVertex(data, vert, cfg.position.vertexByteStride,
//                                         cfg.position.format, end, valid);
//}




template <>
rdcstr DoStringise(const uint32_t& el)
{
	static char tmp[16];
	memset(tmp, 0, sizeof(tmp));
	snprintf(tmp, 15, "%u", el);
	return tmp;
}


#include "../external/renderdoc/api/replay/pipestate.inl"
#include "../external/renderdoc/api/replay/renderdoc_tostr.inl"
