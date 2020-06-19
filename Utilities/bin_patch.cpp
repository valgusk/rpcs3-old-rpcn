﻿#include "bin_patch.h"
#include "File.h"
#include "Config.h"

LOG_CHANNEL(patch_log);

static const std::string patch_engine_version = "1.1";
static const std::string yml_key_enable_legacy_patches = "Enable Legacy Patches";

template <>
void fmt_class_string<YAML::NodeType::value>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](YAML::NodeType::value value)
	{
		switch (value)
		{
		case YAML::NodeType::Undefined: return "Undefined";
		case YAML::NodeType::Null: return "Null";
		case YAML::NodeType::Scalar: return "Scalar";
		case YAML::NodeType::Sequence: return "Sequence";
		case YAML::NodeType::Map: return "Map";
		}

		return unknown;
	});
}

template <>
void fmt_class_string<patch_type>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](patch_type value)
	{
		switch (value)
		{
		case patch_type::invalid: return "invalid";
		case patch_type::load: return "load";
		case patch_type::byte: return "byte";
		case patch_type::le16: return "le16";
		case patch_type::le32: return "le32";
		case patch_type::le64: return "le64";
		case patch_type::bef32: return "bef32";
		case patch_type::bef64: return "bef64";
		case patch_type::be16: return "be16";
		case patch_type::be32: return "be32";
		case patch_type::be64: return "be64";
		case patch_type::lef32: return "lef32";
		case patch_type::lef64: return "lef64";
		}

		return unknown;
	});
}

patch_engine::patch_engine()
{
	const std::string patches_path = fs::get_config_dir() + "patches/";

	if (!fs::create_path(patches_path))
	{
		patch_log.fatal("Failed to create path: %s (%s)", patches_path, fs::g_tls_error);
	}
}

std::string patch_engine::get_patch_config_path()
{
#ifdef _WIN32
	return fs::get_config_dir() + "config/patch_config.yml";
#else
	return fs::get_config_dir() + "patch_config.yml";
#endif
}

static void append_log_message(std::stringstream* log_messages, const std::string& message)
{
	if (log_messages)
		*log_messages << message << std::endl;
};

bool patch_engine::load(patch_map& patches_map, const std::string& path, bool importing, std::stringstream* log_messages)
{
	append_log_message(log_messages, fmt::format("Reading file %s", path));

	// Load patch file
	fs::file file{ path };

	if (!file)
	{
		// Do nothing
		return true;
	}

	// Interpret yaml nodes
	auto [root, error] = yaml_load(file.to_string());

	if (!error.empty() || !root)
	{
		append_log_message(log_messages, "Fatal Error: Failed to load file!");
		patch_log.fatal("Failed to load patch file %s:\n%s", path, error);
		return false;
	}

	// Load patch config to determine which patches are enabled
	bool enable_legacy_patches;
	patch_config_map patch_config;

	if (!importing)
	{
		patch_config = load_config(enable_legacy_patches);
	}

	std::string version;
	bool is_legacy_patch = false;

	if (const auto version_node = root["Version"])
	{
		version = version_node.Scalar();

		if (version != patch_engine_version)
		{
			append_log_message(log_messages, fmt::format("Error: Patch engine target version %s does not match file version %s", patch_engine_version, version));
			patch_log.error("Patch engine target version %s does not match file version %s in %s", patch_engine_version, version, path);
			return false;
		}

		append_log_message(log_messages, fmt::format("Patch file version: %s", version));

		// We don't need the Version node in local memory anymore
		root.remove("Version");
	}
	else if (importing)
	{
		append_log_message(log_messages, fmt::format("Error: Patch engine target version %s does not match file version %s", patch_engine_version, version));
		patch_log.error("Patch engine version %s: No 'Version' entry found for file %s", patch_engine_version, path);
		return false;
	}
	else
	{
		patch_log.warning("Patch engine version %s: Reading legacy patch file %s", patch_engine_version, path);
		is_legacy_patch = true;
	}

	bool is_valid = true;

	// Go through each main key in the file
	for (auto pair : root)
	{
		const auto& main_key = pair.first.Scalar();

		// Use old logic and yaml layout if this is a legacy patch
		if (is_legacy_patch)
		{
			struct patch_info info{};
			info.hash      = main_key;
			info.enabled   = enable_legacy_patches;
			info.is_legacy = true;

			if (!read_patch_node(info, pair.second, root, log_messages))
			{
				is_valid = false;
			}

			// Find or create an entry matching the key/hash in our map
			auto& container = patches_map[main_key];
			container.hash      = main_key;
			container.is_legacy = true;
			container.patch_info_map["legacy"] = info;
			continue;
		}

		// Use new logic and yaml layout

		if (const auto yml_type = pair.second.Type(); yml_type != YAML::NodeType::Map)
		{
			append_log_message(log_messages, fmt::format("Error: Skipping key %s: expected Map, found %s", main_key, yml_type));
			patch_log.error("Skipping key %s: expected Map, found %s (file: %s)", main_key, yml_type, path);
			is_valid = false;
			continue;
		}

		// Skip Anchors
		if (main_key == "Anchors")
		{
			continue;
		}

		if (const auto patches_node = pair.second["Patches"])
		{
			if (const auto yml_type = patches_node.Type(); yml_type != YAML::NodeType::Map)
			{
				append_log_message(log_messages, fmt::format("Error: Skipping Patches: expected Map, found %s (key: %s)", yml_type, main_key));
				patch_log.error("Skipping Patches: expected Map, found %s (key: %s, file: %s)", yml_type, main_key, path);
				is_valid = false;
				continue;
			}

			// Find or create an entry matching the key/hash in our map
			auto& container = patches_map[main_key];
			container.is_legacy = false;
			container.hash      = main_key;
			container.version   = version;

			// Go through each patch
			for (auto patches_entry : patches_node)
			{
				// Each key in "Patches" is also the patch description
				const std::string description = patches_entry.first.Scalar();

				// Find out if this patch was enabled in the patch config
				const bool enabled = patch_config[main_key][description];

				// Compile patch information

				if (const auto yml_type = patches_entry.second.Type(); yml_type != YAML::NodeType::Map)
				{
					append_log_message(log_messages, fmt::format("Error: Skipping Patch key %s: expected Map, found %s (key: %s)", description, yml_type, main_key));
					patch_log.error("Skipping Patch key %s: expected Map, found %s (key: %s, file: %s)", description, yml_type, main_key, path);
					is_valid = false;
					continue;
				}

				struct patch_info info {};
				info.enabled     = enabled;
				info.description = description;
				info.hash        = main_key;
				info.version     = version;

				if (const auto title_node = patches_entry.second["Title"])
				{
					info.title = title_node.Scalar();
				}

				if (const auto serials_node = patches_entry.second["Serials"])
				{
					info.serials = serials_node.Scalar();
				}

				if (const auto author_node = patches_entry.second["Author"])
				{
					info.author = author_node.Scalar();
				}

				if (const auto patch_version_node = patches_entry.second["Version"])
				{
					info.patch_version = patch_version_node.Scalar();
				}

				if (const auto notes_node = patches_entry.second["Notes"])
				{
					info.notes = notes_node.Scalar();
				}

				if (const auto patch_node = patches_entry.second["Patch"])
				{
					if (!read_patch_node(info, patch_node, root, log_messages))
					{
						is_valid = false;
					}
				}

				// Insert patch information
				container.patch_info_map[description] = info;
			}
		}
	}

	return is_valid;
}

patch_type patch_engine::get_patch_type(YAML::Node node)
{
	u64 type_val = 0;

	if (!node || !node.IsScalar() || !cfg::try_to_enum_value(&type_val, &fmt_class_string<patch_type>::format, node.Scalar()))
	{
		return patch_type::invalid;
	}

	return static_cast<patch_type>(type_val);
}

bool patch_engine::add_patch_data(YAML::Node node, patch_info& info, u32 modifier, const YAML::Node& root, std::stringstream* log_messages)
{
	if (!node || !node.IsSequence())
	{
		append_log_message(log_messages, fmt::format("Skipping invalid patch node %s. (key: %s)", info.description, info.hash));
		patch_log.error("Skipping invalid patch node %s. (key: %s)", info.description, info.hash);
		return false;
	}

	const auto type_node  = node[0];
	auto addr_node        = node[1];
	const auto value_node = node[2];

	const auto type = get_patch_type(type_node);

	if (type == patch_type::invalid)
	{
		const auto type_str = type_node && type_node.IsScalar() ? type_node.Scalar() : "";
		append_log_message(log_messages, fmt::format("Skipping patch node %s: type '%s' is invalid. (key: %s)", info.description, type_str, info.hash));
		patch_log.error("Skipping patch node %s: type '%s' is invalid. (key: %s)", info.description, type_str, info.hash);
		return false;
	}

	if (type == patch_type::load)
	{
		// Special syntax: anchors (named sequence)

		// Most legacy patches don't use the anchor syntax correctly, so try to sanitize it.
		if (info.is_legacy)
		{
			if (const auto yml_type = addr_node.Type(); yml_type == YAML::NodeType::Scalar)
			{
				if (!root)
				{
					patch_log.fatal("Trying to parse legacy patch with invalid root."); // Sanity Check
					return false;
				}

				const auto anchor = addr_node.Scalar();
				const auto anchor_node = root[anchor];

				if (anchor_node)
				{
					addr_node = anchor_node;
					append_log_message(log_messages, fmt::format("Incorrect anchor syntax found in legacy patch: %s (key: %s)", anchor, info.hash));
					patch_log.warning("Incorrect anchor syntax found in legacy patch: %s (key: %s)", anchor, info.hash);
				}
				else
				{
					append_log_message(log_messages, fmt::format("Anchor not found in legacy patch: %s (key: %s)", anchor, info.hash));
					patch_log.error("Anchor not found in legacy patch: %s (key: %s)", anchor, info.hash);
					return false;
				}
			}
		}

		// Check if the anchor was resolved.
		if (const auto yml_type = addr_node.Type(); yml_type != YAML::NodeType::Sequence)
		{
			append_log_message(log_messages, fmt::format("Skipping sequence: expected Sequence, found %s (key: %s)", yml_type, info.hash));
			patch_log.error("Skipping sequence: expected Sequence, found %s (key: %s)", yml_type, info.hash);
			return false;
		}

		// Address modifier (optional)
		const u32 mod = value_node.as<u32>(0);

		bool is_valid = true;

		for (const auto& item : addr_node)
		{
			if (!add_patch_data(item, info, mod, root, log_messages))
			{
				is_valid = false;
			}
		}

		return is_valid;
	}

	struct patch_data p_data{};
	p_data.type   = type;
	p_data.offset = addr_node.as<u32>(0) + modifier;

	// Use try/catch instead of YAML::Node::as<T>(fallback) in order to get an error message
	try
	{
		switch (p_data.type)
		{
		case patch_type::bef32:
		case patch_type::lef32:
		case patch_type::bef64:
		case patch_type::lef64:
		{
			p_data.value.double_value = value_node.as<f64>();
			break;
		}
		default:
		{
			p_data.value.long_value = value_node.as<u64>();
			break;
		}
		}
	}
	catch (const std::exception& e)
	{
		const std::string error_message = fmt::format("Skipping patch data entry: [ %s, 0x%.8x, %s ] (key: %s) %s",
			p_data.type, p_data.offset, value_node.IsScalar() && value_node.Scalar().size() ? value_node.Scalar() : "?", info.hash, e.what());
		append_log_message(log_messages, error_message);
		patch_log.error("%s", error_message);
		return false;
	}

	info.data_list.emplace_back(p_data);

	return true;
}

bool patch_engine::read_patch_node(patch_info& info, YAML::Node node, const YAML::Node& root, std::stringstream* log_messages)
{
	if (!node)
	{
		append_log_message(log_messages, fmt::format("Skipping invalid patch node %s. (key: %s)", info.description, info.hash));
		patch_log.error("Skipping invalid patch node %s. (key: %s)" HERE, info.description, info.hash);
		return false;
	}

	if (const auto yml_type = node.Type(); yml_type != YAML::NodeType::Sequence)
	{
		append_log_message(log_messages, fmt::format("Skipping patch node %s: expected Sequence, found %s (key: %s)", info.description, yml_type, info.hash));
		patch_log.error("Skipping patch node %s: expected Sequence, found %s (key: %s)", info.description, yml_type, info.hash);
		return false;
	}

	bool is_valid = true;

	for (auto patch : node)
	{
		if (!add_patch_data(patch, info, 0, root, log_messages))
		{
			is_valid = false;
		}
	}

	return is_valid;
}

void patch_engine::append(const std::string& patch)
{
	load(m_map, patch);
}

void patch_engine::append_global_patches()
{
	// Legacy patch.yml
	load(m_map, fs::get_config_dir() + "patch.yml");

	// New patch.yml
	load(m_map, fs::get_config_dir() + "patches/patch.yml");

	// Imported patch.yml
	load(m_map, fs::get_config_dir() + "patches/imported_patch.yml");
}

void patch_engine::append_title_patches(const std::string& title_id)
{
	if (title_id.empty())
	{
		return;
	}

	// Legacy patch.yml
	load(m_map, fs::get_config_dir() + "data/" + title_id + "/patch.yml");

	// New patch.yml
	load(m_map, fs::get_config_dir() + "patches/" +  title_id + "_patch.yml");
}

std::size_t patch_engine::apply(const std::string& name, u8* dst) const
{
	return apply_patch<false>(name, dst, 0, 0);
}

std::size_t patch_engine::apply_with_ls_check(const std::string& name, u8* dst, u32 filesz, u32 ls_addr) const
{
	return apply_patch<true>(name, dst, filesz, ls_addr);
}

template <bool check_local_storage>
std::size_t patch_engine::apply_patch(const std::string& name, u8* dst, u32 filesz, u32 ls_addr) const
{
	if (m_map.find(name) == m_map.cend())
	{
		return 0;
	}

	size_t applied_total = 0;
	const auto& container = m_map.at(name);

	// Apply modifications sequentially
	for (const auto& [description, patch] : container.patch_info_map)
	{
		if (!patch.enabled)
		{
			continue;
		}

		size_t applied = 0;

		for (const auto& p : patch.data_list)
		{
			u32 offset = p.offset;

			if constexpr (check_local_storage)
			{
				if (offset < ls_addr || offset >= (ls_addr + filesz))
				{
					// This patch is out of range for this segment
					continue;
				}
				
				offset -= ls_addr;
			}

			auto ptr = dst + offset;

			switch (p.type)
			{
			case patch_type::invalid:
			case patch_type::load:
			{
				// Invalid in this context
				continue;
			}
			case patch_type::byte:
			{
				*ptr = static_cast<u8>(p.value.long_value);
				break;
			}
			case patch_type::le16:
			{
				*reinterpret_cast<le_t<u16, 1>*>(ptr) = static_cast<u16>(p.value.long_value);
				break;
			}
			case patch_type::le32:
			{
				*reinterpret_cast<le_t<u32, 1>*>(ptr) = static_cast<u32>(p.value.long_value);
				break;
			}
			case patch_type::lef32:
			{
				*reinterpret_cast<le_t<u32, 1>*>(ptr) = std::bit_cast<u32, f32>(static_cast<f32>(p.value.double_value));
				break;
			}
			case patch_type::le64:
			{
				*reinterpret_cast<le_t<u64, 1>*>(ptr) = static_cast<u64>(p.value.long_value);
				break;
			}
			case patch_type::lef64:
			{
				*reinterpret_cast<le_t<u64, 1>*>(ptr) = std::bit_cast<u64, f64>(p.value.double_value);
				break;
			}
			case patch_type::be16:
			{
				*reinterpret_cast<be_t<u16, 1>*>(ptr) = static_cast<u16>(p.value.long_value);
				break;
			}
			case patch_type::be32:
			{
				*reinterpret_cast<be_t<u32, 1>*>(ptr) = static_cast<u32>(p.value.long_value);
				break;
			}
			case patch_type::bef32:
			{
				*reinterpret_cast<be_t<u32, 1>*>(ptr) = std::bit_cast<u32, f32>(static_cast<f32>(p.value.double_value));
				break;
			}
			case patch_type::be64:
			{
				*reinterpret_cast<be_t<u64, 1>*>(ptr) = static_cast<u64>(p.value.long_value);
				break;
			}
			case patch_type::bef64:
			{
				*reinterpret_cast<be_t<u64, 1>*>(ptr) = std::bit_cast<u64, f64>(p.value.double_value);
				break;
			}
			}

			++applied;
		}

		if (container.is_legacy)
		{
			patch_log.notice("Applied legacy patch (<- %d)", applied);
		}
		else
		{
			patch_log.notice("Applied patch (description='%s', author='%s', patch_version='%s', file_version='%s') (<- %d)", description, patch.author, patch.patch_version, patch.version, applied);
		}

		applied_total += applied;
	}

	return applied_total;
}

void patch_engine::save_config(const patch_map& patches_map, bool enable_legacy_patches)
{
	const std::string path = get_patch_config_path();
	patch_log.notice("Saving patch config file %s", path);

	fs::file file(path, fs::rewrite);
	if (!file)
	{
		patch_log.fatal("Failed to open patch config file %s", path);
		return;
	}

	YAML::Emitter out;
	out << YAML::BeginMap;

	// Save "Enable Legacy Patches"
	out << yml_key_enable_legacy_patches << enable_legacy_patches;

	// Save 'enabled' state per hash and description
	patch_config_map config_map;

	for (const auto& [hash, container] : patches_map)
	{
		if (container.is_legacy)
		{
			continue;
		}

		for (const auto& [description, patch] : container.patch_info_map)
		{
			config_map[hash][description] = patch.enabled;
		}

		if (config_map[hash].size() > 0)
		{
			out << hash;
			out << YAML::BeginMap;

			for (const auto& [description, enabled] : config_map[hash])
			{
				out << description;
				out << enabled;
			}

			out << YAML::EndMap;
		}
	}
	out << YAML::EndMap;

	file.write(out.c_str(), out.size());
}

static void append_patches(patch_engine::patch_map& existing_patches, const patch_engine::patch_map& new_patches)
{
	for (const auto& [hash, new_container] : new_patches)
	{
		if (existing_patches.find(hash) == existing_patches.end())
		{
			existing_patches[hash] = new_container;
			continue;
		}

		auto& container = existing_patches[hash];

		for (const auto& [description, new_info] : new_container.patch_info_map)
		{
			if (container.patch_info_map.find(description) == container.patch_info_map.end())
			{
				container.patch_info_map[description] = new_info;
				continue;
			}

			auto& info = container.patch_info_map[description];

			const auto version_is_bigger = [](const std::string& v0, const std::string& v1, const std::string& hash, const std::string& description)
			{
				std::add_pointer_t<char> ev0, ev1;
				const double ver0 = std::strtod(v0.c_str(), &ev0);
				const double ver1 = std::strtod(v1.c_str(), &ev1);

				if (v0.c_str() + v0.size() == ev0 && v1.c_str() + v1.size() == ev1)
				{
					return ver0 > ver1;
				}

				patch_log.error("Failed to compare patch versions ('%s' vs '%s') for %s: %s", v0, v1, hash, description);
				return false;
			};

			if (!version_is_bigger(new_info.patch_version, info.patch_version, hash, description))
			{
				continue;
			}

			if (!new_info.patch_version.empty()) info.patch_version = new_info.patch_version;
			if (!new_info.title.empty())         info.title         = new_info.title;
			if (!new_info.serials.empty())       info.serials       = new_info.serials;
			if (!new_info.author.empty())        info.author        = new_info.author;
			if (!new_info.notes.empty())         info.notes         = new_info.notes;
			if (!new_info.data_list.empty())     info.data_list     = new_info.data_list;
		}
	}
}

bool patch_engine::save_patches(const patch_map& patches, const std::string& path)
{
	fs::file file(path, fs::rewrite);
	if (!file)
	{
		patch_log.fatal("save_patches: Failed to open patch file %s", path);
		return false;
	}

	YAML::Emitter out;
	out << YAML::BeginMap;
	out << "Version" << patch_engine_version;

	for (const auto& [hash, container] : patches)
	{
		out << YAML::Newline << YAML::Newline;
		out << hash << YAML::BeginMap;
		out << "Patches" << YAML::BeginMap;

		for (auto [description, info] : container.patch_info_map)
		{
			out << description;
			out << YAML::BeginMap;

			if (!info.title.empty())         out << "Title"   << info.title;
			if (!info.serials.empty())       out << "Serials" << info.serials;
			if (!info.author.empty())        out << "Author"  << info.author;
			if (!info.patch_version.empty()) out << "Version" << info.patch_version;
			if (!info.notes.empty())         out << "Notes"   << info.notes;

			out << "Patch";
			out << YAML::BeginSeq;

			for (const auto& data : info.data_list)
			{
				if (data.type == patch_type::invalid || data.type == patch_type::load)
				{
					// Unreachable with current logic
					continue;
				}

				out << YAML::Flow;
				out << YAML::BeginSeq;
				out << fmt::format("%s", data.type);
				out << fmt::format("0x%.8x", data.offset);

				switch (data.type)
				{
				case patch_type::lef32:
				case patch_type::bef32:
				case patch_type::lef64:
				case patch_type::bef64:
				{
					// Using YAML formatting seems good enough for now
					out << data.value.double_value;
					break;
				}
				default:
				{
					out << fmt::format("0x%.8x", data.value.long_value);
					break;
				}
				}

				out << YAML::EndSeq;
			}

			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	out << YAML::EndMap;

	file.write(out.c_str(), out.size());

	return true;
}

bool patch_engine::import_patches(const patch_engine::patch_map& patches, const std::string& path)
{
	patch_engine::patch_map existing_patches;

	if (load(existing_patches, path, true))
	{
		append_patches(existing_patches, patches);
		return save_patches(existing_patches, path);
	}

	return false;
}

patch_engine::patch_config_map patch_engine::load_config(bool& enable_legacy_patches)
{
	enable_legacy_patches = true; // Default to true

	patch_config_map config_map;

	const std::string path = get_patch_config_path();
	patch_log.notice("Loading patch config file %s", path);

	if (fs::file f{ path })
	{
		auto [root, error] = yaml_load(f.to_string());

		if (!error.empty())
		{
			patch_log.fatal("Failed to load patch config file %s:\n%s", path, error);
			return config_map;
		}

		// Try to load "Enable Legacy Patches" (default to true)
		if (auto enable_legacy_node = root[yml_key_enable_legacy_patches])
		{
			enable_legacy_patches = enable_legacy_node.as<bool>(true);
			root.remove(yml_key_enable_legacy_patches); // Remove the node in order to skip it in the next part
		}

		for (auto pair : root)
		{
			auto& hash = pair.first.Scalar();
			auto& data = config_map[hash];

			if (const auto yml_type = pair.second.Type(); yml_type != YAML::NodeType::Map)
			{
				patch_log.error("Error loading patch config key %s: expected Map, found %s (file: %s)", hash, yml_type, path);
				continue;
			}

			for (auto patch : pair.second)
			{
				const auto description = patch.first.Scalar();
				const auto enabled     = patch.second.as<bool>(false);

				data[description] = enabled;
			}
		}
	}

	return config_map;
}
