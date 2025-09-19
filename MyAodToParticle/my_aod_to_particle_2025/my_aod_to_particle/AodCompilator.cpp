#include "my_aod_to_particle.hpp"


//------------------
// Klasa Compilator
//------------------

vector<char> AodCompilator::s_output_file_buffer = {};

particle_type_value AodCompilator::GetParticleVersion() const { return m_particle_type_version; }

void AodCompilator::InitializeFormatHashMaps()
{
	m_list_of_prt_file_ver_info[not_particle] = ParticleFileVersionInfo();
	m_list_of_prt_file_ver_info[dynamic_particle] = ks_dynamic_file_type;
	m_list_of_prt_file_ver_info[ks_particles_emiter] = ks_particle_file_type;
	m_list_of_prt_file_ver_info[e2160_particle] = e2160_particle_edit_file_type;
	m_list_of_prt_file_ver_info[two_worlds_particle] = two_worlds_particle_edit_file_type;
	m_list_of_prt_file_ver_info[particle_gen_particle] = particle_gen_file_type;
}

void AodCompilator::GetPrtVersion(string& arg_line, particle_type_value& dst, ParticleFileVersionInfo& arg_particle_file_version_info)
{
	//DEBUG_PRINT("SPRAWDZAM WERSJE PARTICLESA1\n");

	const string instruction_name = "version ";
	const size_t intruction_name_len = instruction_name.length();

	if (arg_line.substr(0, intruction_name_len) == instruction_name)
	{

		string m_help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);
		dst = static_cast<particle_type_value>(stoull(m_help_str, nullptr, dec_base) + 1);

		DEBUG_PRINT("PRT VERSION = ");
		DEBUG_PRINT(to_string(static_cast<uint32_t>(dst)).c_str());
		DEBUG_PRINT("\n");

		arg_particle_file_version_info = m_list_of_prt_file_ver_info[dst];


		m_is_version_getted = true;
	}
}

void AodCompilator::GetDynamicParticleVersion(string& arg_line,
	particle_type_value& dst,
	ParticleFileVersionInfo& arg_particle_file_version_info)
{
	//DEBUG_PRINT("SPRAWDZAM WERSJE PARTICLESA2\n");

	const string instruction_name = "Dynamic";
	const size_t intruction_name_len = instruction_name.length();

	if (arg_line.substr(0, intruction_name_len) == instruction_name)
	{
		dst = dynamic_particle;
		arg_particle_file_version_info = m_list_of_prt_file_ver_info[dst];

		DEBUG_PRINT("PRT VERSION = ");
		DEBUG_PRINT(to_string(static_cast<uint32_t>(dst)).c_str());
		DEBUG_PRINT("\n");


		m_is_version_getted = true;
	}

}


AodCompilator::AodCompilator(fs::path& arg_input_directory_path, ostream& arg_o) :
	m_input_directory_path(arg_input_directory_path), m_o(arg_o),

	m_instruction(string()),
	m_particle_type_version(not_particle),
	m_is_version_getted(false),
	m_obj_name(string()),
	m_inside_obj_type(string()),
	m_inside_obj_name(string()),
	bracket_counter(-1),
	m_extra_data_file_path(string()),
	m_compilator_flags_cfg_file_path(string()),

	its_pair_particle_emiter(false),
	m_end_of_file_initialize_switch(true)
{
	const char compilator_flags_cfg_file_name[] = "__compilator_flags__.cfg";

	string input_directory_name = this->m_input_directory_path.stem().string();

	string m_input_file_path = (this->m_input_directory_path / (input_directory_name + my_aod_format) ).string();
	m_extra_data_file_path = (this->m_input_directory_path / (input_directory_name + "_extra_data.cpp") ).string();

	m_compilator_flags_cfg_file_path = (this->m_input_directory_path / compilator_flags_cfg_file_name).string();

	InputBinFile aod_file(m_input_file_path, m_o, "text");
	aod_file.Informations();
	aod_file.ExitWhenFileDontExist();

	this->m_file_buffer = aod_file.m_file_buffer;


	InitializeFormatHashMaps();


	string temporary_string_file_buffer(m_file_buffer.begin(), m_file_buffer.end());

	const std::string particles_emiter_text = "ParticlesEmiter";
	const std::string particles_emiter_macro_text = "__PARTICLES_EMITER_MACRO__";

	Utility::ReplaceStringInFileBuffer(temporary_string_file_buffer, particles_emiter_text, particles_emiter_macro_text);

	GlobalQueues::m_effect_names_q = 
		Utility::FindLineStrings(temporary_string_file_buffer.data(), temporary_string_file_buffer.size(), effect_type_name);

	GlobalQueues::m_emiter_names_q = 
		Utility::FindLineStrings(temporary_string_file_buffer.data(), temporary_string_file_buffer.size(), emiter_type_name);

	GlobalQueues::m_particle_names_q = 
		Utility::FindLineStrings(temporary_string_file_buffer.data(), temporary_string_file_buffer.size(), particle_type_name);

	GlobalQueues::m_pair_particle_emiter_names_q = 
		Utility::FindLineStrings(temporary_string_file_buffer.data(), temporary_string_file_buffer.size(), pair_particle_type_name);

	Utility::ReplaceStringInFileBuffer(temporary_string_file_buffer, particles_emiter_macro_text, particles_emiter_text);

	GlobalQueues::m_annotations_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "annotation ");

	GlobalQueues::m_slot_strings_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "slotStrings ");
	GlobalQueues::m_texture_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "texture ");
	GlobalQueues::m_mesh_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "mesh ");
	GlobalQueues::m_bump_texture_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "bumpTexture ");
	GlobalQueues::m_emits_groups_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "emitsGroups ");

	// Only For Dynamic Particle:
	GlobalQueues::m_dynamic_particle_Texture_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "Texture ");
	GlobalQueues::m_dynamic_particle_Mesh_q = Utility::FindLineStrings(m_file_buffer.data(), m_file_buffer.size(), "Mesh ");


	CleanFileBuffer(m_clean_file, true);

}

void AodCompilator::CleanFileBuffer(vector<char>& arg_clean_file, bool change_newline_switch)
{
	bool its_space = false;

	for (size_t i = 0; i < m_file_buffer.size(); ++i)
	{

		if (m_file_buffer[i] == ' ' && its_space == false)
		{
			arg_clean_file.push_back(m_file_buffer[i]);
			its_space = true;
			continue;
		}

		if (m_file_buffer[i] != ' ')
		{
			its_space = false;
		}

		if (its_space == true)
		{
			continue;
		}

		if (m_file_buffer[i] == '\t')
		{
			continue;
		}

		if( (m_file_buffer[i] == '\n') && (change_newline_switch == true) )
		{
			arg_clean_file.push_back(';');
			continue;
		}

		arg_clean_file.push_back(m_file_buffer[i]);
	}


}

// MyAod txt
void AodCompilator::CompileTxtFileToBinBuffer()
{
	for (uint64_t i = 0; i < m_clean_file.size(); ++i)
	{
		m_instruction += m_clean_file[i];

		if (m_clean_file[i] == ';')
		{
			m_instruction.pop_back();

			DEBUG_PRINT("----\n\"");
			DEBUG_PRINT(m_instruction.c_str());
			DEBUG_PRINT("\"\n");

			if (InterpretInstructions(m_instruction) == true)
			{
				break;
			}

			DEBUG_PRINT("----\n");

			m_instruction = string();
		}
	}

	if (m_particle_type_version == dynamic_particle)
	{
		InputBinFile compilator_flags_file(m_compilator_flags_cfg_file_path, m_o, "binary");
		compilator_flags_file.Informations();
		compilator_flags_file.ExitWhenFileDontExist();

		CppCompilator flags_compilator(compilator_flags_file.m_file_buffer);
		flags_compilator.CompileCppFileToBin(s_output_file_buffer);

		m_particle_file_version_info = flags_compilator.GetDynamicParticleVersion();

		IntroductionHeaderClass my_dynamic_prt_introduction_header(m_particle_file_version_info, m_extra_data_file_path, m_o);
		my_dynamic_prt_introduction_header.GetAndWriteToFile(s_output_file_buffer);

		CompileDynamicParticle();
		m_whole_dynamic_particle_container.WriteToFileBuffer(s_output_file_buffer);
	}
	

	if (m_particle_type_version != dynamic_particle)
	{
		IntroductionHeaderClass my_introduction_header(m_particle_file_version_info, m_extra_data_file_path, m_o);
		my_introduction_header.GetAndWriteToFile(s_output_file_buffer);

		m_whole_particle_emiter_container.WriteToFileBuffer(s_output_file_buffer);
	}


}


void AodCompilator::SaveOutputFileBufferToBinFile(string& output_file_name)
{
	fstream output_file;
	output_file.open(output_file_name, std::ios::out | std::ios::binary);

	output_file.write(reinterpret_cast<char*>(&s_output_file_buffer[0]), s_output_file_buffer.size());

	output_file.close();

}



bool AodCompilator::InterpretInstructions(string& instruction)
{	
	if (!m_is_version_getted)
	{
		GetDynamicParticleVersion(instruction, m_particle_type_version, m_particle_file_version_info);
	}

	if (!m_is_version_getted)
	{
		GetPrtVersion(instruction, m_particle_type_version, m_particle_file_version_info);
	}

	switch (m_particle_type_version)
	{
	case dynamic_particle:

		return true;

	break;

	case ks_particles_emiter:
	case e2160_particle:
	case two_worlds_particle:
	case particle_gen_particle:
		InterpretParticlesEmiterInstructions(instruction);
	break;

	default: break;
	}

	return false;
}

void AodCompilator::InterpretParticlesEmiterInstructions(string& instruction)
{
	AU::GetSingleString(instruction, "ParticlesEmiter", m_obj_name);

	if (its_pair_particle_emiter == false)
	{
		AU::GetObjType(instruction, m_inside_obj_type, m_inside_obj_name, effect_type_name, GlobalQueues::m_effect_names_q);
		AU::GetObjType(instruction, m_inside_obj_type, m_inside_obj_name, emiter_type_name, GlobalQueues::m_emiter_names_q);
		AU::GetObjType(instruction, m_inside_obj_type, m_inside_obj_name, particle_type_name, GlobalQueues::m_particle_names_q);
	}

	AU::GetObjType(instruction, m_inside_obj_type, m_inside_obj_name, pair_particle_type_name, GlobalQueues::m_pair_particle_emiter_names_q);

	if (m_inside_obj_type == "Effect")
	{
		DEBUG_PRINT("ITS EFFECT!!!\n");

		CompileObject<EffectClass>(instruction, 
								   m_whole_particle_emiter_container.m_single_effect, 
								   m_whole_particle_emiter_container.m_effects);
	}

	if (m_inside_obj_type == "Emiter")
	{
		DEBUG_PRINT("ITS EMITER!!!\n");

		CompileObject<EmiterClass>(instruction, 
								   m_whole_particle_emiter_container.m_single_emiter,
								   m_whole_particle_emiter_container.m_emiters);
	}

	if (m_inside_obj_type == "Particle")
	{
		DEBUG_PRINT("ITS PARTICLE!!!\n");

		CompileObject<ParticleClass>(instruction,
									 m_whole_particle_emiter_container.m_single_particle,
									 m_whole_particle_emiter_container.m_particles);
	}

	if (m_inside_obj_type == "PairParticleEmiter")
	{
		its_pair_particle_emiter = true;
		DEBUG_PRINT("ITS PAIR PARTICLE EMITER!!!\n");

		CompileObject<PairParticleEmiterClass>(instruction,
											   m_whole_particle_emiter_container.m_single_pair_particle_emiter,
											   m_whole_particle_emiter_container.m_pair_particles_emiters);

	}


	if (m_inside_obj_type == "")
	{
		CompileEndOfFile(instruction, m_whole_particle_emiter_container.m_single_particle_end);

		DEBUG_PRINT("ITS INTRO OR END OF FILE!!!\n");
	}

}


void AodCompilator::CompileEndOfFile(string& instruction, ParticleEndClass& single_particle_end)
{
	if (m_end_of_file_initialize_switch == true)
	{
		single_particle_end = ParticleEndClass(m_particle_file_version_info);

		m_end_of_file_initialize_switch = false;
	}

	single_particle_end.GetFromFile(instruction);


}


void AodCompilator::CompileDynamicParticle()
{
	m_clean_file.clear();

	CleanFileBuffer(m_clean_file, false);

	m_instruction = string();

	m_whole_dynamic_particle_container.m_single_dynamic_particle = 
	DynamicParticleClass(m_particle_file_version_info, this->m_input_directory_path, m_clean_file);

	m_whole_dynamic_particle_container.m_single_dynamic_particle.GetFromFile(m_instruction);
	m_whole_dynamic_particle_container.m_dynamic_particles.push_back( move(m_whole_dynamic_particle_container.m_single_dynamic_particle ) );
}
