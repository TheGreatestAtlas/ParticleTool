#include "KS_Particle_To_Aod.hpp"

ParticleEndData::ParticleEndData() :
gamerate(0)
{}


ParticleEndClass::ParticleEndClass(ParticleFileVersionInfo arg_prt_file_version_info)
    : m_prt_file_version_info(arg_prt_file_version_info)
{
}

void ParticleEndClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    m_prt_file_version_info = arg_prt_file_version_info;
}


void ParticleEndClass::GetFromPgAndTwFile(uint8_t* buff, size_t& curr_offset)
{

    BinReader::GetBinTab<float>(m_particle_end_data.end_particle_values, 
                                m_prt_file_version_info.size_of_end_bonus_data, 
                                buff, 
                                curr_offset);

    m_particle_end_data.gamerate = 0;
}

void ParticleEndClass::GetFromE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_particle_end_data.gamerate = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_end_data.end_particle_values, 
                                m_prt_file_version_info.size_of_end_bonus_data, 
                                buff,
                                curr_offset);
}

void ParticleEndClass::GetFromKsFile(uint8_t* buff, size_t& curr_offset)
{
    m_particle_end_data.gamerate = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
}

void ParticleEndClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case ks_particles_emiter:
        GetFromKsFile(buff, curr_offset);
    break;

    case dynamic_particle:
    break;


    case particle_gen_particle:
    case two_worlds_particle:
        GetFromPgAndTwFile(buff, curr_offset);
    break;

    case e2160_particle:
        GetFromE2160File(buff, curr_offset);
    break;


    default: break;
    }

}


void ParticleEndClass::WriteAsPgAndTwAndE2160Format(stringstream& output) const
{
    AW::WrtVal("gameRate", m_particle_end_data.gamerate, output, one_quad);

    for (int i = 0; i < m_prt_file_version_info.size_of_end_bonus_data; ++i)
    {
        AW::WrtVal("endValue_" + to_string(i), m_particle_end_data.end_particle_values[i], output, one_quad);
    }
}

void ParticleEndClass::WriteAsKsFormat(stringstream& output) const
{
    AW::WrtVal("gameRate", m_particle_end_data.gamerate, output, one_quad);
}


void ParticleEndClass::WriteToFile(stringstream& output)
{
    switch (m_prt_file_version_info.particle_version)
    {

    case ks_particles_emiter:
        WriteAsKsFormat(output);
    break;

    case dynamic_particle:
    break;


    case e2160_particle:
    case two_worlds_particle:
    case particle_gen_particle:
        WriteAsPgAndTwAndE2160Format(output);
    break;

    default: break;
    }

}