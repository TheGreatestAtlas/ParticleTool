#include "my_aod_to_particle.hpp"
#include "MshCustomBlockVectors.hpp"

IntroductionHeaderClass::IntroductionHeaderClass(ParticleFileVersionInfo arg_prt_file_version_info, 
											     string arg_extra_data_file_path,
											     ostream& arg_o) :
	m_prt_file_version_info(arg_prt_file_version_info), m_extra_data_file_path(arg_extra_data_file_path), m_o(arg_o)
{
}

void IntroductionHeaderClass::GetAndWriteToFile(vector<char>& output_file_buff, bool is_child)
{
	InputBinFile my_extra_data_file(m_extra_data_file_path, m_o, "binary");
	my_extra_data_file.Informations();

	if ((my_extra_data_file.GetIsOpened() == false) && (my_extra_data_file.GetIsOpenNow() == false))
	{
		m_o << m_extra_data_file_path
			<< " was not found." << endl 
			<< "The program will attempt to generate headers and initial mesh data blocks..." 
			<< std::endl;

		switch (m_prt_file_version_info.particle_version)
		{
		case dynamic_particle:
			WriteToDynamicParticleFormat(output_file_buff, is_child);
		break;

		case ks_particles_emiter:
			WriteToKsParticleEmiterFormat(output_file_buff, is_child);
		break;

		case e2160_particle:
		break;

		case particle_gen_particle:
		case two_worlds_particle:
			WriteToPgAndTwFormat(output_file_buff);
		break;

		}
	}

	if ((my_extra_data_file.GetIsOpened() == true) && (my_extra_data_file.GetIsOpenNow() == false))
	{
		m_o << m_extra_data_file_path << " was found and opened for compilation." << endl
			<< endl;

		CppCompilator my_extra_data_cpp_compilator(my_extra_data_file.m_file_buffer);
		my_extra_data_cpp_compilator.CompileCppFileToBin(output_file_buff);
	}

}

void IntroductionHeaderClass::WriteToDynamicParticleFormat(vector<char>& output_file_buff, bool is_child)
{
	uint8_t magic_bytes_format[] = { 'M', 'E', 'S', 'H' };
	vector<uint32_t> custom_block_data = {};

	if (!is_child)
	{
		uint8_t header_ff_a1_d0_30[] = { 0xFF, 0xA1, 0xD0, 0x30 };
		
		const uint32_t particle_file_type_value = static_cast<uint32_t>(m_prt_file_version_info.particle_version);

		//header
		BW::WriteValue(output_file_buff, header_ff_a1_d0_30);
		BW::WriteValue(output_file_buff, particle_file_type_value);

		//GUID
		BW::GenerateAndWriteGuid(output_file_buff);
		
		CBU::SetCustomBlockVersionForNotChild(output_file_buff, custom_block_data, m_prt_file_version_info.dynamic_particle_version);
	}

	if (is_child)
	{
		CBU::SetCustomBlockVersionForChild(output_file_buff, custom_block_data, m_prt_file_version_info.dynamic_particle_version);
	}

	BW::WriteValue(output_file_buff, magic_bytes_format);
	BW::WriteVector(output_file_buff, custom_block_data);
}

void IntroductionHeaderClass::WriteToKsParticleEmiterFormat(vector<char>& output_file_buff, bool is_child)
{
	if (!is_child)
	{
		uint8_t header_ff_a1_d0_30[] = { 0xFF, 0xA1, 0xD0, 0x30 };

		const uint32_t particle_file_type_value = static_cast<uint32_t>(m_prt_file_version_info.particle_version);

		uint8_t magic_bytes_format[] = { 'M', 'E', 'S', 'H' };

		//header
		BW::WriteValue(output_file_buff, header_ff_a1_d0_30);
		BW::WriteValue(output_file_buff, particle_file_type_value);

		//GUID
		BW::GenerateAndWriteGuid(output_file_buff);
		
		//Magic bytes MESH
		BW::WriteValue(output_file_buff, magic_bytes_format);

		BW::WriteVector(output_file_buff, ks_particle_emiter_custom_block_data);
	}
}


void IntroductionHeaderClass::WriteToPgAndTwFormat(vector<char>& output_file_buff) const
{
	uint8_t pg_and_tw_format_header[] = { 'P', 'R', 0, 0 };
	const uint8_t tw_unique_format_value = 2;

	if (m_prt_file_version_info.particle_version == particle_gen_particle)
	{
		BW::WriteValue(output_file_buff, pg_and_tw_format_header);

		return;
	}

	if (m_prt_file_version_info.particle_version == two_worlds_particle)
	{
		pg_and_tw_format_header[3] = tw_unique_format_value;

		BW::WriteValue(output_file_buff, pg_and_tw_format_header);

		return;
	}
}

