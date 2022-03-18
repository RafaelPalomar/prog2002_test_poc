#pragma once
#include <string>
#include "../external/renderdoc/api/replay/renderdoc_replay.h"

struct MeshAttribute {
	MeshFormat mesh;
	rdcstr name;
};

struct MeshData {
	MeshFormat mesh;
	rdcarray<MeshAttribute> attrs;
	int first_index;
	rdcarray<uint32_t> indices;
};

class RenderDocHelper {
public:
	RenderDocHelper(std::string rdc_path);
	~RenderDocHelper();
	bool open_capture();

	void dump_actions();
	size_t drawcalls_count() const;
private:
	ActionDescription find_action_by_name(std::string_view name) const {
		for (auto action : m_controller->GetRootActions()) {
			if (action.GetName(*m_structured_data).c_str() == name) {
				return action;
			}
		}
		return ActionDescription{};
	}
	MeshData get_vsin_mesh_data(ActionDescription action, int first_index = 0, int num_indices = 0);
	rdcarray<MeshAttribute> get_vsin_attrs(int vertexOffset, MeshFormat index_mesh);
	rdcarray<uint32_t> fetch_indices(MeshFormat mesh, int index_offset, int first_index, int num_indices);

	void get_vsin(ActionDescription action, int first_index = 0, int num_indices = 0, int instance = 0, int view = 0);
	bytebuf get_thumbnail() const {
		return m_capture_file->GetThumbnail(FileType::PNG, 0).data;
	}

	bytebuf get_ext_thumbnail() const {
		const auto ext_thumb = m_capture_file->FindSectionByType(SectionType::ExtendedThumbnail);
		return m_capture_file->GetSectionContents(ext_thumb);
	}

private:
	std::string m_rdc_path{};
	ICaptureFile* m_capture_file{ nullptr };
	IReplayController* m_controller{ nullptr };
	const SDFile* m_structured_data{ nullptr };
};
