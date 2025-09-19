#include "KS_Particle_To_Aod.hpp"


//--------------------------
// Klasa Particle Decompiler
//--------------------------

void ParticleDecompiler::ShowIntroScreen()
{
  m_o_stream << "**************************************" << endl
             << "|PARTICLE TO MYAOD BY ATLAS AND IMBEE|" << endl
             << "*****************2025*****************" << endl;
}

void ParticleDecompiler::StandardProgramExecution()
{
    //MAIN MENU
    ShowIntroScreen();
    m_o_stream << "Specify the name of the input file:" << endl;
    m_in_stream >> m_input_name;
}

void ParticleDecompiler::ComputeAdditionalInfo()
{
    this->m_input_path = this->m_input_name;

    //dodatkowe informacje:
    //m_obj_name = m_input_name.substr(0, m_input_name.length() - 4);
    this->m_obj_name = this->m_input_path.stem().string();

    this->m_particle_directory_path = this->m_input_path.parent_path() / m_obj_name;

    //m_output_name = m_obj_name + "\\" + m_obj_name + my_aod_format;
    this->m_output_file_path = (this->m_particle_directory_path / (this->m_obj_name + ::my_aod_format)).string();

    //m_bonus_file_name = m_obj_name + "\\" + m_obj_name + bonus_file_rest_name;
    this->m_bonus_file_path = (this->m_particle_directory_path / (this->m_obj_name + ::bonus_file_rest_name)).string();

    //------------------------------

}


void ParticleDecompiler::CheckParticleStatus(particle_type_value& my_particle_file_type)
{
    if (my_particle_file_type == not_particle)
    {
        m_o_stream << "The file being opened is not a particle file..." << endl;
        exit(EXIT_FAILURE);
    }
}

void ParticleDecompiler::WriteIntroToFile(particle_type_value& my_particle_file_type)
{
    switch (my_particle_file_type)
    {
    case dynamic_particle:
        m_output_stream << "Dynamic" << endl
            << "{" << endl
            << "}" << endl;
        break;

    case ks_particles_emiter:
    case particle_gen_particle:
    case two_worlds_particle:
    case e2160_particle:

        m_output_stream << "ParticlesEmiter" << endl
            << "{" << endl
            << '\t' << "version " << my_particle_file_type - 1 << endl
            << "}" << endl;
        break;

    }
}


void ParticleDecompiler::InitializeFormatHashMaps()
{
    m_format_names[not_particle]            = "NULL (NOT PARTICLE)";
    m_format_names[dynamic_particle]        = "Dynamic"; 
    m_format_names[ks_particles_emiter]     = "Ks ParticlesEmiter";
    m_format_names[e2160_particle]          = "E2160 Particle Edit"; 
    m_format_names[two_worlds_particle]     = "Two Worlds Particle Edit";
    m_format_names[particle_gen_particle]   = "Particle Gen";

    m_format_containers[ks_format_str] = ks_particle_file_type;
    m_format_containers[earth2160_format_str] = e2160_particle_edit_file_type;
    m_format_containers[tw_format_str] = two_worlds_particle_edit_file_type;
    m_format_containers[pg_format_str] = particle_gen_file_type;
}

void ParticleDecompiler::ShowInfoAboutParticle(particle_type_value& arg_particle_file_type)
{
    m_o_stream << "I detected particle type: "
               << m_format_names[arg_particle_file_type] << " (" << arg_particle_file_type << ")" << endl
               << "version: " << arg_particle_file_type - 1 << endl << endl;
}

void ParticleDecompiler::GetInformationAboutEndFormat()
{
    m_o_stream << "Force a specific output format? [y/n]" << endl;

    string answer = string();
    string format = string();

    const vector<string> possible_answers = { "y","n","Y","N" };
    const vector<string> yes_answers = { "y","Y" };

    do
    {
        m_in_stream >> answer;

        if (!(Misc::CheckAnswerWithEveryElementInVec(answer, possible_answers)))
        {
            m_o_stream << "Give the answer in y or n form..." << endl;
        }

    } 
    while (!(Misc::CheckAnswerWithEveryElementInVec(answer, possible_answers)));

    if (Misc::CheckAnswerWithEveryElementInVec(answer, yes_answers))
    {
        m_force_end_format = true;

        m_o_stream << "Select the format: [" 
                   << ks_format_str << "/" 
                   << earth2160_format_str << "/" 
                   << tw_format_str << "/" 
                   << pg_format_str 
                   << "]" << endl;

        do
        {
            m_in_stream >> format;

            if (!(Misc::CheckAnswerWithEveryElementInVec(format, correct_formats)))
            {
                m_o_stream << "Choose from the given formats..." << endl;
            }

        } 
        while(!(Misc::CheckAnswerWithEveryElementInVec(format, correct_formats)));

        m_forced_format = m_format_containers[format];
    }
    else
    {
        m_force_end_format = false;
    }

}


ParticleDecompiler::ParticleDecompiler(istream& argin, ostream& argo, int my_argc, char* my_argv[]) :
    m_in_stream(argin), 
    m_o_stream(argo),

    m_input_name(string()), 
    m_output_file_path(string()), 
    m_bonus_file_path(string()), 
    m_number_of_emiters(0),
    m_number_of_particles(0), 
    m_number_of_pairs(0), 
    m_obj_name(string()), 
    m_offset_in_file(0), 
    m_force_end_format(false), 
    m_my_particle_file_type(not_particle), 
    m_fourth_byte_of_header(0),

    m_particle_directory_path(fs::path()),
    m_input_path(fs::path()),
    m_program_parent_path(fs::path())
{
    this->m_program_parent_path = fs::current_path();

    InitializeFormatHashMaps();

    bool standard_program_exec = false;

    if (my_argc != c_correct_number_of_args && my_argc != c_number_of_args_with_force_format)
    {
        standard_program_exec = true;
    }

    if (my_argc == c_correct_number_of_args)
    {

        m_input_name = my_argv[1];
        m_o_stream << "File = " << m_input_name << endl;
    }

    if (my_argc == c_number_of_args_with_force_format)
    {

        m_input_name = my_argv[1];
        m_o_stream << "File = " << m_input_name << endl;

        string force_command = my_argv[2];
        string format = my_argv[3];
        m_o_stream << "Forced format = " << format << endl;

        if (force_command == "--force" || force_command == "-force")
        {
            if (Misc::CheckAnswerWithEveryElementInVec(format, correct_formats))
            {
                m_force_end_format = true;
                m_forced_format = m_format_containers[format];
            }
            else
            {
                standard_program_exec = true;
            }
        }
        else
        {
            standard_program_exec = true;
        }
    }

    if (standard_program_exec == true)
    {
        StandardProgramExecution();
    }

    //Okreœl dodatkowe informacje
    ComputeAdditionalInfo();

    //Dwa proste komunikaty:
    m_o_stream << "I open the file: " << m_input_name << endl
               << "Object found: "    << m_obj_name   << endl;

    // Zaci¹gam plik input do pamiêci
    InputBinFile input_file(const_cast<char*>(m_input_name.c_str()));
    input_file.Informations(m_o_stream);
    input_file.ExitWhenFileDontExist(m_o_stream);

    FileHeaderClass file_header;
    file_header.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
    m_my_particle_file_type = file_header.GetParticleFileType();
    m_fourth_byte_of_header = file_header.GetFourthByteOfHeader();

    CheckParticleStatus(m_my_particle_file_type);

    filesystem::create_directory(this->m_particle_directory_path); // tworze folder na output
    
    ShowInfoAboutParticle(m_my_particle_file_type);

    if (standard_program_exec == true && m_my_particle_file_type != dynamic_particle)
    {
        GetInformationAboutEndFormat();
    }

    if (m_force_end_format == true && m_my_particle_file_type != dynamic_particle)
    {
        file_header.SetPrtVersion(m_forced_format.particle_version);
    }
    file_header.WriteToFile(m_bonus_file_stream);

   
    switch (m_my_particle_file_type)
    {
       
        case dynamic_particle: 
            DecompileDynamicParticleToMyAod(input_file);
        break;

        case ks_particles_emiter:
            DecompileParticleEditFileToMyAod(ks_particle_file_type, input_file);
        break;

        case particle_gen_particle:
            DecompileParticleEditFileToMyAod(particle_gen_file_type, input_file);
        break;

        case two_worlds_particle:
            DecompileParticleEditFileToMyAod(two_worlds_particle_edit_file_type, input_file);
        break;

        case e2160_particle:
            DecompileParticleEditFileToMyAod(e2160_particle_edit_file_type, input_file);
        break;

        default: break;
    }


    AW::WriteStreamToFile(m_bonus_file_stream, m_bonus_file_path);

    m_o_stream << "The file named " << m_input_name << " has been decompiled!" << endl;
}


void ParticleDecompiler::DecompileDynamicParticleToMyAod(InputBinFile& input_file)
{
    string dynamic_particle_cfg_file_path = (this->m_program_parent_path / dynamic_particle_cfg_file_name).string();

    CfgCompilator dynamic_particle_cfg_file(dynamic_particle_cfg_file_path.c_str(), m_o_stream);

    WriteIntroToFile(m_my_particle_file_type); //Wypisujê informacjê wstêpn¹ do pliku wyjœciowego...

    CfgFileData temp_cfg_file_data = dynamic_particle_cfg_file.GetCfgFileData();


    CustomBlockClass custom_block( temp_cfg_file_data.input_dynamic_particle_file_version_info );
    custom_block.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
    if (temp_cfg_file_data.force_dynamic_particle_export_format_switch)
    {
        custom_block.SetPrtVersion(temp_cfg_file_data.forced_dynamic_particle_file_version_info_for_export);
    }
    custom_block.WriteToFile(m_bonus_file_stream);

    m_output_stream << fixed << setprecision(::aod_float_precision);

    DynamicParticleClass my_dynamic_particle(temp_cfg_file_data);

    //my_dynamic_particle.m_particle_directory_name = m_obj_name + "\\";

    my_dynamic_particle.SetParticleDirectoryPath(this->m_particle_directory_path);

    my_dynamic_particle.GetAndSetAndWriteDynamicParticleToAod(input_file.m_file_buffer, m_offset_in_file, m_output_stream, m_obj_name);

    AW::WriteStreamToFile(m_output_stream, m_output_file_path);


}

void ParticleDecompiler::DecompileParticleEditFileToMyAod(ParticleFileVersionInfo arg_particle_file_type, InputBinFile& input_file)
{
    if (m_force_end_format)
    {
        WriteIntroToFile(m_forced_format.particle_version);
    }
    else
    {
        WriteIntroToFile(arg_particle_file_type.particle_version);
    }

    CustomBlockClass custom_block(arg_particle_file_type);
    custom_block.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
    if (m_force_end_format)
    {
        custom_block.SetPrtVersion(m_forced_format);
    }
    custom_block.WriteToFile(m_bonus_file_stream);

    m_output_stream << fixed << setprecision(::aod_float_precision); 

    m_output_stream << "ParticlesEmiter" << " " << m_obj_name << endl << "{" << endl;

    // g³ówna pêtla efektow...
    constexpr uint32_t number_of_effects = 1;
    for (uint32_t h = 0; h < number_of_effects; ++h)
    {
        EffectClass my_effect(arg_particle_file_type);
        my_effect.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
        if (m_force_end_format)
        {
            my_effect.SetPrtVersion(m_forced_format);
        }
        my_effect.WriteToFile(m_output_stream);
    }

    //zaci¹gam liczbê emiterów z pamiêci.
    m_number_of_emiters = BinReader::GetBinVal<uint32_t>(input_file.m_file_buffer, m_offset_in_file);
    m_o_stream << "Number of emiters: " << m_number_of_emiters << endl; //Wypisujê komunikat o iloœci emiterów.

    // g³ówna pêtla emiterów...
    for (uint32_t h = 0; h < m_number_of_emiters; ++h)
    {
        EmiterClass my_emiter(arg_particle_file_type, m_fourth_byte_of_header);
        my_emiter.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
        if (m_force_end_format)
        {
            my_emiter.SetPrtVersion(m_forced_format);
        }
        my_emiter.WriteToFile(m_output_stream);
    }

    
    //zaci¹gam liczbê particlesów z pamiêci.
    m_number_of_particles = BinReader::GetBinVal<uint32_t>(input_file.m_file_buffer, m_offset_in_file);

    m_o_stream << "Number of particles: " << m_number_of_particles << endl;

    //g³ówna pêtla particlesów
    for (uint32_t h = 0; h < m_number_of_particles; ++h)
    {
        ParticleClass my_particle(arg_particle_file_type);
        my_particle.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
        if (m_force_end_format)
        {
            my_particle.SetPrtVersion(m_forced_format);
        }
        my_particle.WriteToFile(m_output_stream);
    }

  
    m_number_of_pairs = BinReader::GetBinVal<uint32_t>(input_file.m_file_buffer, m_offset_in_file);
    m_o_stream << "Number of particle-emiter pairs: " << m_number_of_pairs << endl;

    //g³ówna pêtla PairParticleEmiter
    for (uint32_t h = 0; h < m_number_of_pairs; ++h)
    {
        PairParticleEmiterClass my_pair_particle_emiter(arg_particle_file_type);
        my_pair_particle_emiter.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
        if (m_force_end_format)
        {
            my_pair_particle_emiter.SetPrtVersion(m_forced_format);
        }
        my_pair_particle_emiter.WriteToFile(m_output_stream);
    }


    ParticleEndClass my_particle_end(arg_particle_file_type);
    my_particle_end.GetFromFile(input_file.m_file_buffer, m_offset_in_file);
    if (m_force_end_format)
    {
        my_particle_end.SetPrtVersion(m_forced_format);
    }
    my_particle_end.WriteToFile(m_output_stream);

    m_output_stream << "}";

    AW::WriteStreamToFile(m_output_stream, m_output_file_path);

    
}