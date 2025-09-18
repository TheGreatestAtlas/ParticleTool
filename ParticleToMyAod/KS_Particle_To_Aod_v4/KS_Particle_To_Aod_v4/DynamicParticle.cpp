#include "KS_Particle_To_Aod.hpp"

//----------------------------
// Metody DynamicParticleData
//----------------------------

DynamicParticleData::DynamicParticleData() :
    type(0), 
    light_type(0), 
    delta_size(0.0), 
    null_space(0), 
    additive(0),
    mesh(string()), 
    texture(string()), 
    number_of_nested_particles(0), 
    layer(0)
{
}


void DynamicParticleClass::InitializeHashMaps()
{

    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
    {
        m_list_of_type_consts[0] = "";
        m_list_of_type_consts[1] = "Explosion";
        m_list_of_type_consts[2] = "Track";
        m_list_of_type_consts[3] = "ScaleableObject";
        m_list_of_type_consts[4] = "MappedExplosion";
        m_list_of_type_consts[5] = "FlatExplosion";
        m_list_of_type_consts[6] = "Laser";
        m_list_of_type_consts[7] = "LaserWall";
        m_list_of_type_consts[8] = "Shockwave";
        m_list_of_type_consts[9] = "Line";
        m_list_of_type_consts[10] = "ElectricalCannon";
        m_list_of_type_consts[11] = "Lighting";
        m_list_of_type_consts[12] = "Smoke";
        m_list_of_type_consts[13] = "Kilwater";
        m_list_of_type_consts[14] = "Blood";

        m_list_of_lighttype_consts[1] = "Const";
        m_list_of_lighttype_consts[2] = "Pyramid";
        m_list_of_lighttype_consts[3] = "Trapezium";
        m_list_of_lighttype_consts[4] = "Random";
    }

    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version == e2150_old_dynamic_particle)
    {
        m_list_of_type_consts[0] = "";
        m_list_of_type_consts[1] = "Explosion";
        m_list_of_type_consts[2] = "Track";
        m_list_of_type_consts[3] = "ScaleableObject";
        m_list_of_type_consts[4] = "MappedExplosion";
        m_list_of_type_consts[5] = "FlatExplosion";
        m_list_of_type_consts[6] = "Laser";
        m_list_of_type_consts[7] = "LaserWall";
        m_list_of_type_consts[8] = "Shockwave";
        m_list_of_type_consts[9] = "Line";
        m_list_of_type_consts[10] = "Sphere";
        m_list_of_type_consts[11] = "ElectricalCannon";
        m_list_of_type_consts[12] = "Lighting";
        m_list_of_type_consts[13] = "Smoke";
        m_list_of_type_consts[14] = "Kilwater";
        m_list_of_type_consts[15] = "Blood";

        m_list_of_lighttype_consts[0] = "Const";
        m_list_of_lighttype_consts[1] = "Pyramid";
        m_list_of_lighttype_consts[2] = "Trapezium";
        m_list_of_lighttype_consts[3] = "Random";

    }

}


DynamicParticleClass::DynamicParticleClass(CfgFileData arg_cfg_file_data) :
    m_dynamic_particle_cfg(arg_cfg_file_data),

    m_particle_directory_name(string())
{
    InitializeHashMaps();
}


void DynamicParticleClass::SetParticleDirectoryName(string arg_name)
{
    m_particle_directory_name = arg_name;
}



string DynamicParticleClass::GetParticleDirectoryName() const { return m_particle_directory_name; };



void DynamicParticleClass::WriteCompilatorFlagsFile() const
{
    unordered_map<old_dynamic_particle_type_value, string> dynamic_particle_formats_hash_map = {};

    dynamic_particle_formats_hash_map[not_old_dynamic_particle] = "not_dynamic_particle";
    dynamic_particle_formats_hash_map[e2150_old_dynamic_particle] = e2150_format_str;
    dynamic_particle_formats_hash_map[ww3_of_hh_old_dynamic_particle] = ww3_or_hh_format_str;
    dynamic_particle_formats_hash_map[ww2_or_fa_or_pc2_old_dynamic_particle] = ww2_or_fa_or_pc2_format_str;
    dynamic_particle_formats_hash_map[ks_old_dynamic_particle] = ks_format_str;

    stringstream compilator_flags_file_stream;
    const char compilator_flags_file_name[] = "__compilator_flags__.cfg";

    compilator_flags_file_stream << "__compiler_var__ dynamic_particle_version = "
                                 << dynamic_particle_formats_hash_map[m_dynamic_particle_cfg .
                                                                      input_dynamic_particle_file_version_info .
                                                                      dynamic_particle_version] << ";" << endl << endl;

    AW::WriteStreamToFile(compilator_flags_file_stream, 
                          m_particle_directory_name + compilator_flags_file_name);

}


void DynamicParticleClass::GetAndSetAndWriteDynamicParticleToAod(uint8_t* buffer,
                                                            size_t& offset, 
                                                            stringstream& output, 
                                                            string& particle_name)
{
    GetData(buffer, offset, particle_name, m_private_dynamic_particle_data);
    if (m_dynamic_particle_cfg.force_dynamic_particle_export_format_switch)
    {
        SetPrtVersion(m_dynamic_particle_cfg.forced_dynamic_particle_file_version_info_for_export);
    }
    WriteToFile(m_private_dynamic_particle_data, output, particle_name, one_quad);

    WriteCompilatorFlagsFile();
}

void DynamicParticleClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    m_dynamic_particle_cfg.input_dynamic_particle_file_version_info = arg_prt_file_version_info;
}

bool DynamicParticleClass::GetData(uint8_t* buffer, size_t& offset, string particle_name,
    DynamicParticleData& dynamic_particle_data)
{
    dynamic_particle_data.type = BinReader::GetBinVal<uint32_t>(buffer, offset);

    dynamic_particle_data.light_type = BinReader::GetBinVal<uint32_t>(buffer, offset);

    dynamic_particle_data.frames[0] = BinReader::GetBinVal<int32_t>(buffer, offset);

    dynamic_particle_data.frames[1] = BinReader::GetBinVal<int32_t>(buffer, offset) 
                                        + dynamic_particle_data.frames[0] - 1;
    //frames2 = mshvalue + frames1 - 1

    dynamic_particle_data.frames[3] = BinReader::GetBinVal<int32_t>(buffer, offset);
    dynamic_particle_data.frames[4] = BinReader::GetBinVal<int32_t>(buffer, offset);
    dynamic_particle_data.frames[2] = BinReader::GetBinVal<int32_t>(buffer, offset);

    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
    {
        int64_t help_time = static_cast<int64_t>(BinReader::GetBinVal<int32_t>(buffer, offset));

        dynamic_particle_data.time[0] = static_cast<float>(static_cast<double>(help_time) / 1000.0);

        help_time = static_cast<int64_t>(BinReader::GetBinVal<int32_t>(buffer, offset));

        dynamic_particle_data.time[1] = static_cast<float>(1.0 - (static_cast<double>(help_time) / 1000.0));
    }

    BinReader::GetBinTab<float>(dynamic_particle_data.frames_ratio, 
        dynamic_particle_data.frames_ratio_size, buffer, offset);


    BinReader::GetBinTab<float>(dynamic_particle_data.size,
        dynamic_particle_data.size_size - 1, buffer, offset);


    BinReader::GetBinTab<float>(dynamic_particle_data.size_2,
        dynamic_particle_data.size2_size, buffer, offset);


    dynamic_particle_data.size[4] = BinReader::GetBinVal<float>(buffer, offset);

    dynamic_particle_data.delta_size = BinReader::GetBinVal<float>(buffer, offset);

    dynamic_particle_data.null_space = BinReader::GetBinVal<uint32_t>(buffer, offset);

    dynamic_particle_data.additive = BinReader::GetBinVal<uint32_t>(buffer, offset);

    for (int i = 0; i < dynamic_particle_data.light_size - 1; ++i)
        dynamic_particle_data.light[i] = BinReader::GetBinVal<float>(buffer, offset) / dynamic_particle_data.light[3];
    // dziele przez l4
    // i zakladam ze l4 = 1 zeby bylo prosciej


    BinReader::GetBinTab<float>(dynamic_particle_data.color,
        dynamic_particle_data.color_size, buffer, offset);


    dynamic_particle_data.alpha.b[0] = BinReader::GetBinVal<uint32_t>(buffer, offset);

    BinReader::GetBinTabReverse<float>(dynamic_particle_data.alpha.a, dynamic_particle_data.alpha_a_size, buffer, offset);

    BinReader::GetBinTabReverse<float>(dynamic_particle_data.scale,
        dynamic_particle_data.scale_size, buffer, offset);


    BinReader::GetBinTab<float>(dynamic_particle_data.position,
        dynamic_particle_data.position_size, buffer, offset);


    dynamic_particle_data.position[1] /= -1.0;

    BinReader::GetBinTab<float>(dynamic_particle_data.position_2,
        dynamic_particle_data.position_2_size, buffer, offset);


    dynamic_particle_data.position_2[1] /= -1.0;

    dynamic_particle_data.mesh = BinReader::TakeString(buffer, offset);

    dynamic_particle_data.texture = BinReader::TakeString(buffer, offset);

    dynamic_particle_data.number_of_nested_particles = BinReader::GetBinVal<uint32_t>(buffer, offset);

    int no = 1;
    string child_name;

    // Rekurencyjne wczytywanie dzieci
    for (uint32_t i = 0; i < dynamic_particle_data.number_of_nested_particles; ++i)
    {
        child_name = particle_name + "_" + to_string(no);

        stringstream child_bonus_file_stream;

        CustomBlockClass custom_children_block(m_dynamic_particle_cfg.input_dynamic_particle_file_version_info, true);
        custom_children_block.GetFromFile(buffer, offset);
        if (m_dynamic_particle_cfg.force_dynamic_particle_export_format_switch)
        {
            custom_children_block.SetPrtVersion(m_dynamic_particle_cfg.forced_dynamic_particle_file_version_info_for_export);
        }
        custom_children_block.WriteToFile(child_bonus_file_stream);

        AW::WriteStreamToFile(child_bonus_file_stream, m_particle_directory_name + child_name + bonus_file_rest_name);


        DynamicParticleData child;

        if (!GetData(buffer, offset, child_name, child)) break;

        dynamic_particle_data.children.push_back(move(child));

        //------- naprawa
        ++no;
    }

    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
    {
        dynamic_particle_data.layer = BinReader::GetBinVal<uint32_t>(buffer, offset);
    }

    return true;
}


void DynamicParticleClass::WriteToFile(const DynamicParticleData& dynamic_particle_data,
                                       stringstream& output, string particle_name, int count_of_tabulation)
{
    AW::WrtVal("Object", particle_name, output, count_of_tabulation - 1);

    AW::WrtSinStr("{", output, count_of_tabulation - 1);

    AW::WriteConstValueToAod<uint32_t>("Type",
                                      dynamic_particle_data.type, 
                                      m_list_of_type_consts, 
                                      output,
                                      count_of_tabulation);

    AW::WrtVal("Additive", dynamic_particle_data.additive, output, count_of_tabulation);

    AW::WriteConstValueToAod<uint32_t>("LightType", 
                                      dynamic_particle_data.light_type,
                                      m_list_of_lighttype_consts, 
                                      output,
                                      count_of_tabulation);

    AW::WriteArrayToAod<int32_t>("Frames", 
                                dynamic_particle_data.frames,
                                dynamic_particle_data.frames_size, 
                                output,
                                count_of_tabulation);

    AW::WriteArrayToAod<float>("Light", 
                              dynamic_particle_data.light, 
                              dynamic_particle_data.light_size,
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Size", 
                              dynamic_particle_data.size,
                              dynamic_particle_data.size_size, 
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Size2", 
                              dynamic_particle_data.size_2, 
                              dynamic_particle_data.size2_size,
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Color", 
                              dynamic_particle_data.color, 
                              dynamic_particle_data.color_size,
                              output,
                              count_of_tabulation);

    TwoTypeArray<float, dynamic_particle_data.alpha_a_size, uint32_t, dynamic_particle_data.alpha_b_size> help_alpha
                                                                                       = dynamic_particle_data.alpha;

    AW::WriteTwoTypeArrayToAod<float, dynamic_particle_data.alpha_a_size, uint32_t, dynamic_particle_data.alpha_b_size>
                              ("Alpha",
                              help_alpha,
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Scale", 
                              dynamic_particle_data.scale,
                              dynamic_particle_data.scale_size, 
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Position", 
                              dynamic_particle_data.position,
                              dynamic_particle_data.position_size, 
                              output,
                              count_of_tabulation);

    AW::WriteArrayToAod<float>("Position2", 
                              dynamic_particle_data.position_2,
                              dynamic_particle_data.position_2_size,
                              output,
                              count_of_tabulation);

    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
    {
        AW::WriteArrayToAod<float>("Time",
                                   dynamic_particle_data.time,
                                   dynamic_particle_data.time_size,
                                   output,
                                   count_of_tabulation);
    }

    AW::WrtVal("Texture", dynamic_particle_data.texture, output, count_of_tabulation);

    AW::WrtVal("Mesh", dynamic_particle_data.mesh, output, count_of_tabulation);

    int no = 1;
    string child_name;
    for (const auto& child : dynamic_particle_data.children)
    {
        child_name = particle_name + "_" + to_string(no);

        WriteToFile(child, output, child_name, count_of_tabulation + 1);

        ++no;
    }


    if (m_dynamic_particle_cfg.input_dynamic_particle_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
    {
        AW::WrtVal("Layer", dynamic_particle_data.layer, output, count_of_tabulation);
    }

    AW::WrtSinStr("}", output, count_of_tabulation - 1);

}

