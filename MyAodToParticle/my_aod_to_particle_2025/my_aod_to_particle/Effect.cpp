#include "my_aod_to_particle.hpp"


std::vector<std::string> PgEffectStrings = 
{
                    // particle gen      // two worlds
"trajectoryXMin",   // ok                // ok
"trajectoryXMax",   // ok                // ok

"trajectoryYMin",   // ok                // ok
"trajectoryYMax",   // ok                // ok

"trajectoryZMin",   // ok                // ok
"trajectoryZMax",   // ok                // ok

"rotationZMin",     // ok                // ok
"rotationZMax",     // ok                // ok

"rotationXMin",     // ok                // ok
"rotationXMax",     // ok                // ok

"rotationYMin",     // ok                // ok
"rotationYMax"      // ok                // ok

};



EffectData::EffectData() :
    particle_2D_switch(0),
    width(0),
    height(0),
    start_time(0.0),
    time(0.0),
    loops(0),
    all_teselate_zero_mask(0),

    unknown_value_1(0),
    unknown_value_2(0),
    
    annotation(string()),

    //e2160:
    e2160_effect_scale(0)
{
}


void EffectClass::InitializeHashMap()
{
    switch (m_prt_file_version_info.particle_version)
    {
    case particle_gen_particle:
    case two_worlds_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, PgEffectStrings);
    break;

    case e2160_particle:
    case dynamic_particle:
    case ks_particles_emiter:
    break;

    default: break;
    }

}

EffectClass::EffectClass(ParticleFileVersionInfo arg_prt_file_version_info, string arg_effect_name)
    : m_prt_file_version_info(arg_prt_file_version_info),

    m_curve_iel_index(-1),
    m_number_of_single_curve_points_checksum(0),

    m_teselate_iel_index(-1),
    m_number_of_single_teselate_points_checksum(0),

    m_mode(null)
{
    InitializeHashMap();

    m_effect_data.linked_min_max.resize(m_prt_file_version_info.number_of_linked_min_max_in_effect, locked_linked_min_max);

    m_effect_data.curves_groups.resize(m_prt_file_version_info.number_of_iel_in_effect);
    m_effect_data.teselates_groups.resize(m_prt_file_version_info.number_of_iel_in_effect);
}


void EffectClass::GetFromFile(string& arg_line)
{
    AU::GetOneValue<uint32_t>(arg_line, "particle2D", "%d", m_effect_data.particle_2D_switch);
    AU::GetOneValue<uint32_t>(arg_line, "width", "%d", m_effect_data.width);
    AU::GetOneValue<uint32_t>(arg_line, "height", "%d", m_effect_data.height);
    AU::GetOneValue<float>(arg_line, "startTime", "%f", m_effect_data.start_time);
    AU::GetOneValue<float>(arg_line, "time", "%f", m_effect_data.time);

    AU::GetOneValue<uint32_t>(arg_line, "loops", "%d", m_effect_data.loops);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue1", "%d", m_effect_data.unknown_value_1);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue2", "%d", m_effect_data.unknown_value_2);

    AU::GetSingleStringFromQueue(arg_line, "annotation", m_effect_data.annotation, GlobalQueues::m_annotations_q);

    AU::GetOneValue<uint32_t>(arg_line, "allTeselateZeroMask", "%d", m_effect_data.all_teselate_zero_mask);

    if (!m_effect_data.linked_min_max.empty())
    {
        AU::GetTabValues<uint32_t>(arg_line,
            "linkedMinMax",
            "%d",
            &m_effect_data.linked_min_max[0],
            static_cast<int>(m_effect_data.linked_min_max.size()));
    }

    AU::GetOneValue<uint32_t>(arg_line, "scale", "%d", m_effect_data.e2160_effect_scale);

    AU::GetArrayOfTwoTypeTabValues<float, m_effect_data.scale_a_size, uint32_t, m_effect_data.scale_b_size>
                                  (arg_line, "scales", "%f", "%d", m_effect_data.scales, m_effect_data.scales_size);

    AU::GetCurveHeader(arg_line,
        m_effect_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetCurves(arg_line,
        m_effect_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_mode);
     

    AU::GetTeselateHeader(arg_line,
        m_effect_data.teselates_groups,
        m_teselate_iel_index,
        m_number_of_single_teselate_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetTeselates(arg_line, 
                     m_effect_data.teselates_groups, 
                     m_teselate_iel_index, 
                     m_number_of_single_teselate_points_checksum, 
                     m_mode);
}


void EffectClass::WriteToFile(vector<char>& output_file_buff)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case particle_gen_particle:
    case two_worlds_particle:
        WriteToParticleGenAndTwoWorldsFormat(output_file_buff);
    break;

    case e2160_particle:
        WriteToE2160Format(output_file_buff);
    break;

    case dynamic_particle:
    case ks_particles_emiter:
    break;

    default: break;
    }

}


void EffectClass::WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_effect_data.annotation);

    BW::WriteValue(output_file_buff, m_effect_data.particle_2D_switch);
    BW::WriteValue(output_file_buff, m_effect_data.width);
    BW::WriteValue(output_file_buff, m_effect_data.height);

    BW::WriteValue(output_file_buff, m_effect_data.start_time);
    BW::WriteValue(output_file_buff, m_effect_data.time);
    BW::WriteValue(output_file_buff, m_effect_data.loops);
   
    BW::WriteValue(output_file_buff, m_effect_data.scales);

    BW::WriteValue(output_file_buff, m_effect_data.unknown_value_1);
    BW::WriteValue(output_file_buff, m_effect_data.unknown_value_2);

    BW::WriteCurves(output_file_buff, m_effect_data.curves_groups);
   
    
    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BW::WriteTeselates<uint64_t>(output_file_buff,
                                    m_effect_data.teselates_groups,
                                    true);
    }
    else
    {
        BW::WriteTeselates<uint64_t>(output_file_buff,
                                     m_effect_data.teselates_groups);

        BW::WriteVector(output_file_buff, m_effect_data.teselates_groups.teselate_flags);
    }

    BW::WriteVector(output_file_buff, m_effect_data.linked_min_max );
    BW::WriteValue(output_file_buff, m_effect_data.all_teselate_zero_mask);
}


void EffectClass::WriteToE2160Format(vector<char>& output_file_buff)
{

    BW::WriteValue(output_file_buff, m_effect_data.particle_2D_switch);
    BW::WriteValue(output_file_buff, m_effect_data.width);
    BW::WriteValue(output_file_buff, m_effect_data.height);

    BW::WriteValue(output_file_buff, m_effect_data.e2160_effect_scale);

    BW::WriteString(output_file_buff, m_effect_data.annotation);
}