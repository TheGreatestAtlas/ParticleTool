#include "KS_Particle_To_Aod.hpp"

//--------------------
// Metody CustomBlock
//--------------------
CustomBlockClass::CustomBlockClass(ParticleFileVersionInfo arg_prt_file_version_info, bool arg_is_child) : 
    m_prt_file_version_info(arg_prt_file_version_info),
    m_is_child(arg_is_child)
{
}


void CustomBlockClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    if (m_prt_file_version_info.particle_version == dynamic_particle
        &&
        m_prt_file_version_info.dynamic_particle_version != not_old_dynamic_particle
        &&
        arg_prt_file_version_info.particle_version == dynamic_particle
        &&
        arg_prt_file_version_info.dynamic_particle_version != not_old_dynamic_particle
        )
    {
        CustomBlockUtility::SetCustomBlockVersion(m_custom_block_data, 
                                                  m_prt_file_version_info.dynamic_particle_version,
                                                  arg_prt_file_version_info.dynamic_particle_version,
                                                  m_is_child);
    }


    m_prt_file_version_info = arg_prt_file_version_info;
}


void CustomBlockClass::GetFromKsFile(uint8_t* buff, size_t& curr_offset)
{
    
    for (int i = 0; i < sizeof(uint32_t); ++i)
        m_custom_block_data.format[i] = BinReader::GetBinVal<uint8_t>(buff, curr_offset);

    for (size_t i = 0; i < m_prt_file_version_info.custom_data_block_size; ++i)
    {
        m_custom_block_data.data.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));
    }

}


void CustomBlockClass::GetFromPgAndTwAndE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_custom_block_data.format[0] = 'M';
    m_custom_block_data.format[1] = 'E';
    m_custom_block_data.format[2] = 'S';
    m_custom_block_data.format[3] = 'H';

    m_custom_block_data.data = { 1, 2, 0xFFFFFFFF };

    constexpr size_t number_of_zeros = 0x264 / 0x4;
    for (size_t i = 0; i < number_of_zeros; ++i)
        m_custom_block_data.data.push_back(0);


    constexpr size_t number_of_mini_blocks = 77;

    constexpr uint64_t dot_euro_dot_euro_ascii = 0x80008000;
    constexpr uint64_t dot_euro_dot_dot_ascii = 0x00008000;

    constexpr size_t number_of_zeros_in_mini_block = 0x24 / 0x4;

    for (size_t j = 0; j < number_of_mini_blocks; ++j)
    {
        m_custom_block_data.data.push_back(dot_euro_dot_euro_ascii);
        m_custom_block_data.data.push_back(dot_euro_dot_dot_ascii);

        for (size_t i = 0; i < number_of_zeros_in_mini_block; ++i)
            m_custom_block_data.data.push_back(0);
    
    }

    constexpr size_t number_of_end_ints = 2;
    for (size_t i = 0; i < number_of_end_ints; ++i)
        m_custom_block_data.data.push_back(0);

}


void CustomBlockClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case dynamic_particle:
    case ks_particles_emiter:
        GetFromKsFile(buff, curr_offset);
    break;

    case e2160_particle:
    case particle_gen_particle:
    case two_worlds_particle:
        GetFromPgAndTwAndE2160File(buff, curr_offset);
    break;

    default: break;
    }

}


void CustomBlockClass::WriteAsKsFormat(stringstream& output)
{

    output << "struct DataBlock" << endl << "{" << endl;

    for (int i = 0; i < m_custom_block_data.c_format_size; ++i)
    {
        output << "\tuint8_t format" << i << " = " << static_cast<uint32_t>(m_custom_block_data.format[i]) <<
            "; // " << m_custom_block_data.format[i] << endl;
    }

    for (size_t i = 0; i < m_custom_block_data.data.size(); ++i)
        output << "\tuint32_t data_var" << format("{:04}", i) << " = "
        << m_custom_block_data.data[i] << ";" << endl;

    output << "};" << endl << endl;

}

void CustomBlockClass::WriteToFile(stringstream& output)
{
    switch (m_prt_file_version_info.particle_version)
    {

    case dynamic_particle:
    case ks_particles_emiter:
        WriteAsKsFormat(output);
    break;


    case e2160_particle:
    case particle_gen_particle:
    case two_worlds_particle:
    break;

    default: break;
    }

}

