#include "my_aod_to_particle.hpp"


std::vector<std::string> PgEmiterStrings =
{
    // particle gen:    
"trajectoryXMin",       // ok                
"trajectoryXMax",       // ok                

"trajectoryYMin",       // ok                
"trajectoryYMax",       // ok                

"trajectoryZMin",       // ok                
"trajectoryZMax",       // ok                


"rotationZMin",         // ok                
"rotationZMax",         // ok                


"rotationXMin",         // ok                
"rotationXMax",         // ok                

"rotationYMin",         // ok                
"rotationYMax",         // ok                

"countMin",             // ok                
"countMax",             // ok                

"speedMulMin",          // ok                
"speedMulMax",          // ok                

"sizeMulMin",           // ok                
"sizeMulMax",           // ok                

"timeMulMin",           // ok               
"timeMulMax",           // ok                

"lightRangeMin",        // ok                
"lightRangeMax",        // ok                

"lightColorRMin",       // ok               
"lightColorRMax",       // ok                

"lightColorGMin",       // ok                
"lightColorGMax",       // ok                

"lightColorBMin",       // ok                
"lightColorBMax",       // ok                

"surfaceScaleXMin",     // ok                
"surfaceScaleXMax",     // ok                

"surfaceScaleYMin",     // ok                
"surfaceScaleYMax",     // ok                

"surfaceScaleZMin",     // ok                
"surfaceScaleZMax",     // ok                

"sphereScaleOMin_Min",  // ok                
"sphereScaleOMin_Max",  // ok                

"sphereScaleOMax_Min",  // ok                
"sphereScaleOMax_Max",  // ok                

"sphereScaleWMin_Min",  // ok                
"sphereScaleWMin_Max",  // ok                

"sphereScaleWMax_Min",  // ok                
"sphereScaleWMax_Max"  // ok                

};


std::vector<std::string> TwEmiterStrings =
{
    // two worlds :
"trajectoryXMin",       // ok
"trajectoryXMax",       // ok

"trajectoryYMin",       // ok
"trajectoryYMax",       // ok

"trajectoryZMin",       // ok
"trajectoryZMax",       // ok


"rotationZMin",         // ok
"rotationZMax",         // ok


"rotationXMin",         // ok
"rotationXMax",         // ok

"rotationYMin",         // ok
"rotationYMax",         // ok

"countMin",             // ok
"countMax",             // ok

"speedMulMin",          // ok
"speedMulMax",          // ok

"sizeMulMin",           // ok
"sizeMulMax",           // ok

"timeMulMin",           // ok
"timeMulMax",           // ok


"lightRangeMin",        // ok
"lightRangeMax",        // ok

"lightColorRMin",       // ok
"lightColorRMax",       // ok

"lightColorGMin",       // ok
"lightColorGMax",       // ok

"lightColorBMin",       // ok
"lightColorBMax",       // ok

"lightIntensityMin",    // ok
"lightIntensityMax",    // ok

"surfaceScaleXMin",     // ok
"surfaceScaleXMax",     // ok

"surfaceScaleYMin",     // ok
"surfaceScaleYMax",     // ok

"surfaceScaleZMin",     // ok
"surfaceScaleZMax",     // ok

"sphereScaleOMin_Min",  // ok
"sphereScaleOMin_Max",  // ok

"sphereScaleOMax_Min",  // ok
"sphereScaleOMax_Max",  // ok

"sphereScaleWMin_Min",  // ok
"sphereScaleWMin_Max",  // ok

"sphereScaleWMax_Min",  // ok
"sphereScaleWMax_Max"   // ok

};


std::vector<std::string> E2160EmiterStrings =
{
    // e2160 :
"trajectoryXMin",       // ok
"trajectoryXMax",       // ok

"trajectoryYMin",       // ok
"trajectoryYMax",       // ok

"trajectoryZMin",       // ok
"trajectoryZMax",       // ok


"rotationZMin",         // ok
"rotationZMax",         // ok


"rotationXMin",         // ok
"rotationXMax",         // ok

"rotationYMin",         // ok
"rotationYMax",         // ok

"countMin",             // ok
"countMax",             // ok

"speedMulMin",          // ok 
"speedMulMax",          // ok

"sizeMulMin",           // ok
"sizeMulMax",           // ok

"timeMulMin",           // ok
"timeMulMax",           // ok

"lightRange",           //?

"lightColorR",          // ok     

"lightColorG",          // ok   

"lightColorB",          // ok        

"surfaceScaleX",       // ok
"surfaceScaleY",       // ok
"surfaceScaleZ",       // ok

"sphereScaleOMin",     // ok

"sphereScaleOMax",     // ok

"sphereScaleWMin",     // ok

"sphereScaleWMax"     // ok

};


std::vector<std::string> KsEmiterStrings =
{
"trajectoryX",
"trajectoryY",
"trajectoryZ",

"rotationAlpha",
"rotationBeta",
"rotationPhi",

"countMin",
"countMax",
"speedMulMin",
"speedMulMax",
"sizeMulMin",
"sizeMulMax",
"timeMulMin",
"timeMulMax",
"budget",
"lightRange",
"lightIntensity",
"lightColorR",
"lightColorG",
"lightColorB",
"surfaceScaleX",
"surfaceScaleY",
"surfaceScaleZ",
"sphereScaleOMin",
"sphereScaleOMax",
"sphereScaleWMin",
"sphereScaleWMax"
};



//-------------------
// Metody EmiterData
//-------------------

EmiterData::EmiterData() :

    emiter_name(string()),
    start_time(0.0),
    time(0.0),

    coordinates(0),
    budget_type(0),

    sun_ray_direction_switch(0),

    use_color_switch(0),

    texture(string()),

    create_on_ground(0),
    trajectory_z_from_ground(0),

    enable_light(0),
    umbra_distance_mul(0.0),

    cast_shadows(0),

    move_end_of_laser_switch(0),
    attach_to_end_of_laser_switch(0),

    unknown_value_1(0),
    unknown_value_2(0),
    unknown_value_3(0),
    unknown_value_4(0),
    unknown_value_5(0),

    all_teselate_zero_mask(0)
{
}

void EmiterClass::InitializeHashMap()
{
    switch (m_prt_file_version_info.particle_version)
    {
    case particle_gen_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, PgEmiterStrings);
    break;

    case two_worlds_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, TwEmiterStrings);
    break;

    case e2160_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, E2160EmiterStrings);
    break;

    case ks_particles_emiter:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, KsEmiterStrings);
    break;

    case dynamic_particle:
    break;

    default: break;
    }

}

EmiterClass::EmiterClass(ParticleFileVersionInfo arg_prt_file_version_info, string arg_emiter_name)
    : m_prt_file_version_info(arg_prt_file_version_info),

    m_curve_iel_index(-1),
    m_number_of_single_curve_points_checksum(0),

    m_teselate_iel_index(-1),
    m_number_of_single_teselate_points_checksum(0),

    m_mode(null),

    m_write_cast_shadows_to_bin(static_cast<uint32_t>(false))
{
    m_emiter_data.emiter_name = arg_emiter_name;

    InitializeHashMap();

    m_emiter_data.linked_min_max.resize(m_prt_file_version_info.number_of_linked_min_max_in_emiter, locked_linked_min_max);

    m_emiter_data.curves_groups.resize(m_prt_file_version_info.number_of_iel_in_emiter);
    m_emiter_data.teselates_groups.resize(m_prt_file_version_info.number_of_iel_in_emiter);
}

void EmiterClass::GetFromFile(string& arg_line)
{
    AU::GetOneValue<float>(arg_line, "startTime", "%f", m_emiter_data.start_time);

    AU::GetOneValue<float>(arg_line, "time", "%f", m_emiter_data.time);

    AU::GetOneValue<uint32_t>(arg_line, "coordinates", "%d", m_emiter_data.coordinates);

    AU::GetOneValue<uint32_t>(arg_line, "budgetType", "%d", m_emiter_data.budget_type);

    AU::GetOneValue<uint32_t>(arg_line, "sunRayDirection", "%d", m_emiter_data.sun_ray_direction_switch);

    AU::GetOneValue<uint32_t>(arg_line, "useColor", "%d", m_emiter_data.use_color_switch);

    AU::GetOneValue<uint32_t>(arg_line, "createOnGround", "%d", m_emiter_data.create_on_ground);

    AU::GetOneValue<uint32_t>(arg_line, "trajectoryZFromGround", "%d", m_emiter_data.trajectory_z_from_ground);

    AU::GetOneValue<uint32_t>(arg_line, "enableLight", "%d", m_emiter_data.enable_light);

    AU::GetOneValue<float>(arg_line, "umbraDistanceMul", "%f", m_emiter_data.umbra_distance_mul);

    //------------------------------------------------- special -------------------------------------
    AU::GetOneValue<uint32_t>(arg_line, "writeCastShadowsToBin", "%d", m_write_cast_shadows_to_bin);
    //------------------------------------------------- special -------------------------------------

    AU::GetOneValue<uint32_t>(arg_line, "castShadows", "%d", m_emiter_data.cast_shadows);
    //-----------------------------------------------------------------------------------------------

    AU::GetOneValue<uint32_t>(arg_line, "moveEndOfLaser", "%d", m_emiter_data.move_end_of_laser_switch);

    AU::GetOneValue<uint32_t>(arg_line, "attachToEndOfLaser", "%d", m_emiter_data.attach_to_end_of_laser_switch);

    AU::GetOneValue<uint32_t>(arg_line, "unknownValue1", "%d", m_emiter_data.unknown_value_1);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue2", "%d", m_emiter_data.unknown_value_2);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue3", "%d", m_emiter_data.unknown_value_3);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue4", "%d", m_emiter_data.unknown_value_4);
    AU::GetOneValue<uint32_t>(arg_line, "unknownValue5", "%d", m_emiter_data.unknown_value_5);

    AU::GetOneValue<uint32_t>(arg_line, "allTeselateZeroMask", "%d", m_emiter_data.all_teselate_zero_mask);

    AU::GetTabValues<uint32_t>(arg_line, "surface", "%d", m_emiter_data.surface, m_emiter_data.surface_size);

    AU::GetTabValues<float>(arg_line, "sphere", "%f", m_emiter_data.sphere, m_emiter_data.sphere_size);

    AU::GetTabValues<uint32_t>(arg_line, "rotation", "%d", m_emiter_data.rotation, m_emiter_data.rotation_size);

    AU::GetTabValues<uint32_t>(arg_line, "randomSeed", "%d", m_emiter_data.random_seed, m_emiter_data.random_seed_size);

    AU::GetTabValues<uint32_t>(arg_line, 
                               "linkedMinMax", 
                               "%d", 
                               &m_emiter_data.linked_min_max[0], 
                               static_cast<int>(m_emiter_data.linked_min_max.size()));

    AU::GetSingleStringFromQueue(arg_line, "texture", m_emiter_data.texture, GlobalQueues::m_texture_q);

    AU::GetTwoTypeTabValues<float, m_emiter_data.cubic_a_size, uint32_t, m_emiter_data.cubic_b_size>
        (arg_line, "cubic", "%f", "%d", m_emiter_data.cubic);

    AU::GetTwoTypeTabValues<float, m_emiter_data.plane_a_size, uint32_t, m_emiter_data.plane_b_size>
        (arg_line, "plane", "%f", "%d", m_emiter_data.plane);

    AU::GetVectorOfStringsFromQueue(arg_line, "slotStrings", m_emiter_data.slot_strings, GlobalQueues::m_slot_strings_q);

    AU::GetTwoTypeTabValuesViaScheme<float, m_emiter_data.scales_a_size, uint32_t, m_emiter_data.scales_b_size>
        (arg_line, "scales", "%f", "%d", "a,a,a,b,a,a,a,b,a,a,a,a,a,a,a,a", m_emiter_data.scales);

    AU::GetCurveHeader(arg_line,
        m_emiter_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetCurves(arg_line,
        m_emiter_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_mode);


    AU::GetTeselateHeader(arg_line,
        m_emiter_data.teselates_groups,
        m_teselate_iel_index,
        m_number_of_single_teselate_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetTeselates(arg_line,
        m_emiter_data.teselates_groups,
        m_teselate_iel_index,
        m_number_of_single_teselate_points_checksum,
        m_mode);

}

void EmiterClass::WriteToFile(vector<char>& output_file_buff)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case dynamic_particle:
    break;

    case ks_particles_emiter:
        WriteToKsFormat(output_file_buff);
    break;

    case particle_gen_particle:
    case two_worlds_particle:
        WriteToParticleGenAndTwoWorldsFormat(output_file_buff);
    break;

    case e2160_particle:
        WriteToE2160Format(output_file_buff);
    break;

    }

}

void EmiterClass::WriteToKsFormat(vector<char>& output_file_buff)
{

    BW::WriteValue(output_file_buff, m_emiter_data.time);

    BW::WriteValue(output_file_buff, m_emiter_data.coordinates);

    BW::WriteValue(output_file_buff, m_emiter_data.budget_type);

    BW::WriteValue(output_file_buff, m_emiter_data.surface[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.sphere);

    BW::WriteValue(output_file_buff, m_emiter_data.cubic);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.a);

    BW::WriteString(output_file_buff, m_emiter_data.texture, true);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.b);

    BW::WriteValue(output_file_buff, m_emiter_data.surface[1]);

    BW::WriteValue(output_file_buff, m_emiter_data.surface[2]);

    BW::WriteValue(output_file_buff, m_emiter_data.enable_light);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 0, 2);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.a[12]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 3, 5);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[1]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 6, 11);

    BW::WriteTeselates<uint32_t>(output_file_buff, m_emiter_data.teselates_groups);
    
    BW::WriteVector(output_file_buff, m_emiter_data.teselates_groups.teselate_flags);

    BW::WriteVector(output_file_buff, m_emiter_data.linked_min_max);

    BW::WriteValue(output_file_buff, m_emiter_data.all_teselate_zero_mask);

}

void EmiterClass::WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_emiter_data.emiter_name);

    BW::WriteValue(output_file_buff, m_emiter_data.start_time);

    BW::WriteValue(output_file_buff, m_emiter_data.time);

    BW::WriteValue(output_file_buff, m_emiter_data.coordinates);

    BW::WriteVectorOfStrings(output_file_buff, m_emiter_data.slot_strings);

    BW::WriteValue(output_file_buff, m_emiter_data.surface[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.sphere);

    BW::WriteValue(output_file_buff, m_emiter_data.cubic);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.a);

    BW::WriteString(output_file_buff, m_emiter_data.texture);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.b);

    BW::WriteValue(output_file_buff, m_emiter_data.use_color_switch);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.surface, 1, 2);

    BW::WriteValue(output_file_buff, m_emiter_data.sun_ray_direction_switch);

    BW::WriteValue(output_file_buff, m_emiter_data.create_on_ground);

    BW::WriteValue(output_file_buff, m_emiter_data.trajectory_z_from_ground);

    BW::WriteTabViaIndexes(output_file_buff, m_emiter_data.rotation, { 2, 0, 1 });

    BW::WriteValue(output_file_buff, m_emiter_data.enable_light);

    if( (m_prt_file_version_info.particle_version == two_worlds_particle) && (static_cast<bool>(m_write_cast_shadows_to_bin) == true) )
    {
        BW::WriteValue(output_file_buff, m_emiter_data.cast_shadows);
    }

    BW::WriteValue(output_file_buff, m_emiter_data.umbra_distance_mul);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 0, 2);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.a[12]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 3, 5);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[1]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 6, 10);

    BW::WriteValue(output_file_buff, m_emiter_data.move_end_of_laser_switch);

    BW::WriteValue(output_file_buff, m_emiter_data.attach_to_end_of_laser_switch);

    BW::WriteValue(output_file_buff, m_emiter_data.random_seed);

    BW::WriteTabViaIndexes(output_file_buff, m_emiter_data.scales.a, { 11, 13 });

    BW::WriteValue(output_file_buff, m_emiter_data.unknown_value_1);
    BW::WriteValue(output_file_buff, m_emiter_data.unknown_value_2);
    BW::WriteValue(output_file_buff, m_emiter_data.unknown_value_3);
    BW::WriteValue(output_file_buff, m_emiter_data.unknown_value_4);
    BW::WriteValue(output_file_buff, m_emiter_data.unknown_value_5);

    BW::WriteCurves(output_file_buff, m_emiter_data.curves_groups);

    
    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BW::WriteTeselates<uint64_t>(output_file_buff,
                                    m_emiter_data.teselates_groups,
                                    true);
    }
    else
    {
        BW::WriteTeselates<uint64_t>(output_file_buff, m_emiter_data.teselates_groups);

        BW::WriteVector(output_file_buff, m_emiter_data.teselates_groups.teselate_flags);
    }

    BW::WriteVector(output_file_buff, m_emiter_data.linked_min_max);
    BW::WriteValue(output_file_buff, m_emiter_data.all_teselate_zero_mask);

}


void EmiterClass::WriteToE2160Format(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_emiter_data.emiter_name);

    BW::WriteValue(output_file_buff, m_emiter_data.start_time);

    BW::WriteValue(output_file_buff, m_emiter_data.time);

    BW::WriteValue(output_file_buff, m_emiter_data.coordinates);

    BW::WriteValue(output_file_buff, m_emiter_data.surface[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.sphere);

    BW::WriteValue(output_file_buff, m_emiter_data.cubic);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.a);

    BW::WriteString(output_file_buff, m_emiter_data.texture);

    BW::WriteValue(output_file_buff, m_emiter_data.plane.b);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.surface, 1, 2);

    BW::WriteValue(output_file_buff, m_emiter_data.enable_light);

    BW::WriteValue(output_file_buff, m_emiter_data.umbra_distance_mul);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 0, 2);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[0]);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.a[12]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 3, 5);

    BW::WriteValue(output_file_buff, m_emiter_data.scales.b[1]);

    BW::WriteTabFromTo(output_file_buff, m_emiter_data.scales.a, 6, 10);

    BW::WriteValue(output_file_buff, m_emiter_data.move_end_of_laser_switch);

    BW::WriteValue(output_file_buff, m_emiter_data.attach_to_end_of_laser_switch);

    BW::WriteValue(output_file_buff, m_emiter_data.random_seed);

    BW::WriteTabViaIndexes(output_file_buff, m_emiter_data.scales.a, { 11, 13 } );

    BW::WriteCurves(output_file_buff, m_emiter_data.curves_groups, true);

    BW::WriteTeselates<uint64_t>(output_file_buff, m_emiter_data.teselates_groups);

    BW::WriteVector(output_file_buff, m_emiter_data.teselates_groups.teselate_flags);

    BW::WriteVector(output_file_buff, m_emiter_data.linked_min_max);
    BW::WriteValue(output_file_buff, m_emiter_data.all_teselate_zero_mask);
}
