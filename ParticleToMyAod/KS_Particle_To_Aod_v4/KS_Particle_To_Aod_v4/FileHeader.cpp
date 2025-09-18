#include "KS_Particle_To_Aod.hpp"

//const string example_guid = "{41D62F91-7401-4841-8E86-0072D58038D6}";

FileHeaderData::FileHeaderData() : particle_file_type(not_particle), guid(Utility::GenerateGuidString())
{}

FileHeaderClass::FileHeaderClass() : m_app_particle_file_type(not_particle)
{}

particle_type_value FileHeaderClass::GetParticleFileType() const { return m_app_particle_file_type; }

uint8_t FileHeaderClass::GetFourthByteOfHeader() const { return m_file_header_data.header[3]; }

void FileHeaderClass::SetPrtVersion(particle_type_value arg_particle_type_value)
{
     switch (arg_particle_type_value)
     {
        case two_worlds_particle:
             m_file_header_data.header[0] = 'P';
             m_file_header_data.header[1] = 'R';
             m_file_header_data.header[2] = '\0';
             m_file_header_data.header[3] = '\2';
             m_file_header_data.particle_file_type = not_particle;
        break;

        case particle_gen_particle:
             m_file_header_data.header[0] = 'P';
             m_file_header_data.header[1] = 'R';
             m_file_header_data.header[2] = '\0';
             m_file_header_data.header[3] = '\0';
             m_file_header_data.particle_file_type = not_particle;
        break;

        case e2160_particle:
        break;

        case dynamic_particle:
            m_file_header_data.header[0] = 0xFF;
            m_file_header_data.header[1] = 0xA1;
            m_file_header_data.header[2] = 0xD0;
            m_file_header_data.header[3] = 0x30;
            m_file_header_data.particle_file_type = dynamic_particle;
        break;

        case ks_particles_emiter:
            m_file_header_data.header[0] = 0xFF;
            m_file_header_data.header[1] = 0xA1;
            m_file_header_data.header[2] = 0xD0;
            m_file_header_data.header[3] = 0x30;
            m_file_header_data.particle_file_type = ks_particles_emiter;
        break;

        default: break;
    }

    m_app_particle_file_type = arg_particle_type_value;
}


void FileHeaderClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{

    for (int i = 0; i < m_file_header_data.c_header_size; ++i)
        m_file_header_data.header[i] = BinReader::GetBinVal<uint8_t>(buff, curr_offset);

    if(m_file_header_data.header[0] == 'P'  && 
       m_file_header_data.header[1] == 'R'  && 
       m_file_header_data.header[2] == '\0' &&
        m_file_header_data.header[3] == '\0'
        )
    {
        m_app_particle_file_type = particle_gen_particle;
    }

    if (m_file_header_data.header[0] == 'P' &&
        m_file_header_data.header[1] == 'R' &&
        m_file_header_data.header[2] == '\0' &&
        m_file_header_data.header[3] == '\1'
        )
    {
        m_app_particle_file_type = two_worlds_particle;
    }


    if (m_file_header_data.header[0] == 'P' &&
        m_file_header_data.header[1] == 'R' &&
        m_file_header_data.header[2] == '\0' &&
        m_file_header_data.header[3] == '\2'
        )
    {
        m_app_particle_file_type = two_worlds_particle;
    }

    if (//m_file_header_data.header[0] == '\0' &&
        m_file_header_data.header[1] == '\0' &&
        m_file_header_data.header[2] == '\0' &&
        m_file_header_data.header[3] == '\0'
        )
    {
        m_app_particle_file_type = e2160_particle;
        curr_offset -= sizeof(uint32_t);
        return;
    }


    if (m_file_header_data.header[0] == 0xFF && 
        m_file_header_data.header[1] == 0xA1 &&
        m_file_header_data.header[2] == 0xD0 &&
        m_file_header_data.header[3] == 0x30)
    {
        m_file_header_data.particle_file_type = BinReader::GetBinVal<particle_type_value>(buff, curr_offset);
        m_app_particle_file_type = m_file_header_data.particle_file_type;

        m_file_header_data.guid = BinReader::GetGuid(buff, curr_offset);
    }
}



void FileHeaderClass::WriteAsKsFormat(stringstream& output) const
{
    output << "struct Header" << endl
        << "{" << endl;


    for (int i = 0; i < m_file_header_data.c_header_size; ++i)
    {
        output << "\tuint8_t header_main_flag_" << i << " = "
               << static_cast<int>(m_file_header_data.header[i]) << ";";

        if (isalpha(m_file_header_data.header[i]))
           output << " // " << m_file_header_data.header[i];

        output << endl;

    }

    output << "\tint32_t particle_file_type" << " = " << m_file_header_data.particle_file_type << ";" << endl;

    output << "\tGUID header_guid = " << m_file_header_data.guid << ";" << endl;

    output << "};" << endl << endl;

}

void FileHeaderClass::WriteAsPgAndTwFormat(stringstream& output) const
{
    output << "struct Header" << endl
           << "{" << endl;

    for (int i = 0; i < m_file_header_data.c_header_size; ++i)
    {
        output << "\tuint8_t header_main_flag_" << i << " = "
               << static_cast<int>(m_file_header_data.header[i]) << ";";

        if (isalpha(m_file_header_data.header[i]))
           output << " // " << m_file_header_data.header[i];

        output << endl;

    }
    
    output << "};" << endl << endl;
}

void FileHeaderClass::WriteAsE2160Format(stringstream& output) const
{
    output << "struct Header" << endl
           << "{"             << endl 
           << "};"            << endl << endl;
}


void FileHeaderClass::WriteToFile(stringstream& output) 
{
    switch (m_app_particle_file_type)
    {

    case particle_gen_particle:
    case two_worlds_particle:
        WriteAsPgAndTwFormat(output);
    break;

    case e2160_particle:
        WriteAsE2160Format(output);
    break;

    case dynamic_particle:
    case ks_particles_emiter:
        WriteAsKsFormat(output);
    break;

    default: break;
    }

}