#include "KS_Particle_To_Aod.hpp"

CfgFileData CfgCompilator::GetCfgFileData() const { return m_cfg_file_data; }

constexpr size_t number_of_value_types = 3;
const char* value_types[] =
{
"dynamic_particle_input_format",
"force_specific_export_format",
"forced_export_format"
};

const vector<string> dynamic_particle_correct_formats = { e2150_format_str, 
                                                                    ks_format_str, 
                                                                    ww3_or_hh_format_str, 
                                                                    ww2_or_fa_or_pc2_format_str };

const vector<string> boolean_values = 
{ 
"YES","yes","Yes","TRUE","true","True","1",
"NO","no","No","FALSE","false","False","0"
};

CfgCompilator::CfgCompilator(const char arg_cfg_file_name[], ostream& arg_o) :
    m_o(arg_o),
    m_instruction(string()),
    m_value_type(string()),
    m_value(string())
{
    bool m_its_comment = false;
    bool m_its_string = false;


    if (!filesystem::exists(arg_cfg_file_name))
    {
        fstream new_cfg_file;

        m_o << dynamic_particle_cfg_file_name << " file does not exist so I created a new one...." << endl;

        new_cfg_file.open(arg_cfg_file_name, ios::out); 

        new_cfg_file << "// Dynamic Particle Config:" << endl
                     << value_types[0] << " = " << ks_format_str << ";" << endl
                     << value_types[1] << " = " << "false;" << endl 
                     << value_types[2] << " = " << ks_format_str << ";" << endl
                     << "// Available formats:" << endl
                     << "// " << e2150_format_str << endl
                     << "// " << ww3_or_hh_format_str << endl
                     << "// " << ww2_or_fa_or_pc2_format_str << endl
                     << "// " << ks_format_str << endl << endl;

        new_cfg_file.close();
    }

    InputBinFile cfg_file(const_cast<char*>(arg_cfg_file_name));
    cfg_file.Informations(m_o);
    cfg_file.ExitWhenFileDontExist(m_o);


    if (cfg_file.m_file_buffer != nullptr && cfg_file.GetFileLength() != 0)
    {
        for (uint64_t i = 0; i < cfg_file.GetFileLength() - 1; ++i)
        {
            if ((cfg_file.m_file_buffer[i] == '/') && (cfg_file.m_file_buffer[i + 1] == '/'))
            {
                m_its_comment = !(m_its_comment);
            }

            if ((cfg_file.m_file_buffer[i] == 0x0D) && (cfg_file.m_file_buffer[i + 1] == 0x0A) && (m_its_comment == true))
            {
                m_its_comment = !(m_its_comment);
            }

            if (m_its_comment == true)
            {
                cfg_file.m_file_buffer[i] = ' ';
                continue;
            }
        }


        for (uint64_t i = 0; i < cfg_file.GetFileLength() - 1; ++i)
        {
            if (cfg_file.m_file_buffer[i] == '\"')
            {
                m_its_string = !m_its_string;
            }

            if ((cfg_file.m_file_buffer[i] <= 32) && (m_its_string == false))
            {
                continue;
            }

            m_cfg_clean_file.push_back(cfg_file.m_file_buffer[i]);
        }
    }

    GetInfoFromCfg();
}


void CfgCompilator::GetInfoFromCfg()
{
    InitializeDynamicParticleHashMap();

    m_o << "DynamicParticle.cfg:" << endl
        << "####################" << endl;
        

    for (uint64_t i = 0; i < m_cfg_clean_file.size(); ++i)
    {
        m_instruction += m_cfg_clean_file[i];

        if (m_cfg_clean_file[i] == ';')
        {
            DEBUG_PRINT("----\n");
            DEBUG_PRINT(m_instruction.c_str());
            DEBUG_PRINT("\n");

            GetValuesFromString();
            InitializeSwitches(m_value_type, m_value);

            DEBUG_PRINT(m_value_type.c_str()); DEBUG_PRINT("\n");
            DEBUG_PRINT(m_value.c_str()); DEBUG_PRINT("\n");
            DEBUG_PRINT("----\n");

            m_instruction = string();
            m_value_type = string();
            m_value = string();
        }
    }

    m_o << "-----------------" << endl;
}


void CfgCompilator::GetValuesFromString()
{
    while (1)
    {
        bool nopswitch = true;

        //Sprawdzenie czy jest prawidlowy typ
        for (uint32_t i = 0; i < number_of_value_types; ++i)
        {
            if (m_instruction.substr(0, string(value_types[i]).length()) == value_types[i])
            {
                m_value_type = value_types[i];
                nopswitch = false;
                break;
            }
        }

        // Jesli typ byl nieprawidlowy czyli nopswitch == true to wychodze
        if (nopswitch == true)
        {
            m_value_type = "nop";
            m_value = string();
            return;
        }

        //Wyodrebnienie wartosci od instrukcji
        uint64_t valueindex = m_instruction.find("=") + 1;
        m_value = m_instruction.substr(valueindex, m_instruction.length() - valueindex - 1);


        if (m_value == "YES" || m_value == "yes" || m_value == "Yes" ||
            m_value == "TRUE" || m_value == "true" || m_value == "True")
        {
            m_value = "1";
            return;
        }

        if (m_value == "NO" || m_value == "no" || m_value == "No" ||
            m_value == "FALSE" || m_value == "false" || m_value == "False")
        {
            m_value = "0";
            return;
        }


        break;
    }

}

void CfgCompilator::InitializeSwitches(string& value_type, string& value)
{
    // dynamic_particle_input_format
    if (value_type == value_types[0])
    {
        if (!(Misc::CheckAnswerWithEveryElementInVec(value, dynamic_particle_correct_formats)))
        {
            m_o << "Error: The configuration file contains an incorrect dynamic particle INPUT format." << endl 
                << "       Please correct the configuration file by specifying the available dynamic particle format." << endl 
                << "       At this point, the program will assume that it is ks format." << endl;

            value = ks_format_str;
        }

        m_cfg_file_data.input_dynamic_particle_file_version_info = m_dynamic_particle_file_version_map[value];

        m_o << value_type << " = " << value << endl;

        return;
    }

    // force_specific_export_format
    if (value_type == value_types[1])
    {
        if (!(Misc::CheckAnswerWithEveryElementInVec(value, boolean_values)))
        {
            m_o << "Error: The configuration file contains an incorrect switch value." << endl
                << "       Please correct the configuration file." << endl
                << "       At this point, the program will assume that it is \"false\" value." << endl;

            value = "0";
        }

        m_cfg_file_data.force_dynamic_particle_export_format_switch = stoul(value, nullptr, 10);

        m_o << value_type << " = " << m_cfg_file_data.force_dynamic_particle_export_format_switch << endl;

        return;
    }

    // forced_export_format
    if (value_type == value_types[2])
    {
        if (!(Misc::CheckAnswerWithEveryElementInVec(value, dynamic_particle_correct_formats)))
        {
            m_o << "Error: The configuration file contains an incorrect dynamic particle EXPORT format." << endl
                << "       Please correct the configuration file by specifying the available dynamic particle format." << endl
                << "       At this point, the program will assume that it is ks format." << endl;

            value = ks_format_str;
        }

        m_cfg_file_data.forced_dynamic_particle_file_version_info_for_export = m_dynamic_particle_file_version_map[value];

        m_o << value_type << " = " << value << endl;

        return;
    }


}


void CfgCompilator::InitializeDynamicParticleHashMap()
{
    m_dynamic_particle_file_version_map[""] = ParticleFileVersionInfo();
    m_dynamic_particle_file_version_map[e2150_format_str] = earth2150_dynamic_file_type;
    m_dynamic_particle_file_version_map[ww3_or_hh_format_str] = ww3_or_heli_heroes_dynamic_file_type;
    m_dynamic_particle_file_version_map[ww2_or_fa_or_pc2_format_str] = ww2_or_fa_or_pc2_dynamic_file_type;
    m_dynamic_particle_file_version_map[ks_format_str] = ks_dynamic_file_type;
}