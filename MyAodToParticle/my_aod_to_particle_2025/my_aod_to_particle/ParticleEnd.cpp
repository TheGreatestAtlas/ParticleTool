#include "my_aod_to_particle.hpp"

ParticleEndData::ParticleEndData() :
	gamerate(0)
{
}


ParticleEndClass::ParticleEndClass(ParticleFileVersionInfo arg_prt_file_version_info)
	: m_prt_file_version_info(arg_prt_file_version_info)
{
}

void ParticleEndClass::GetFromFile(string& arg_line)
{
	AU::GetOneValue<uint32_t>(arg_line, "gameRate", "%d", m_particle_end_data.gamerate);

	for (int i = 0; i < m_prt_file_version_info.size_of_end_bonus_data; ++i)
		AU::GetOneValue<float>(arg_line, "endValue_" + to_string(i), "%f", m_particle_end_data.end_particle_values[i]);
	
}

void ParticleEndClass::WriteToFile(vector<char>& output_file_buff)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case ks_particles_emiter:
        WriteToKsFormat(output_file_buff);
    break;

    case dynamic_particle:
    break;


    case particle_gen_particle:
    case two_worlds_particle:
        WriteToPgAndTwFormat(output_file_buff);
    break;

    case e2160_particle:
        WriteToE2160Format(output_file_buff);
    break;


    default: break;
    }

}

void ParticleEndClass::WriteToPgAndTwFormat(vector<char>& output_file_buff) const
{
    BW::WriteTabFromTo(output_file_buff, m_particle_end_data.end_particle_values, 0, m_prt_file_version_info.size_of_end_bonus_data - 1);
}

void ParticleEndClass::WriteToE2160Format(vector<char>& output_file_buff) const
{
    BW::WriteValue(output_file_buff, m_particle_end_data.gamerate);

    BW::WriteTabFromTo(output_file_buff, m_particle_end_data.end_particle_values, 0, m_prt_file_version_info.size_of_end_bonus_data - 1);
}

void ParticleEndClass::WriteToKsFormat(vector<char>& output_file_buff) const
{
    BW::WriteValue(output_file_buff, m_particle_end_data.gamerate);
}