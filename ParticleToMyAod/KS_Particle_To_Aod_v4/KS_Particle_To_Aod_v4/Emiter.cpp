#include "KS_Particle_To_Aod.hpp"

vector<string> PgEmiterStrings = 
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


vector<string> TwEmiterStrings = 
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


vector<string> E2160EmiterStrings =
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


vector<string> KsEmiterStrings = 
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

uint32_t EmiterClass::emiter_number = 0;

//-------------------------
// Metody PgEmiterData
//-------------------------

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

EmiterClass::EmiterClass(ParticleFileVersionInfo arg_prt_file_version_info, uint8_t arg_fourth_byte_of_header)
    : m_prt_file_version_info(arg_prt_file_version_info), m_fourth_byte_of_header(arg_fourth_byte_of_header)
{
}


void EmiterClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    if (arg_prt_file_version_info.particle_version == two_worlds_particle)
    {
        m_fourth_byte_of_header = tw_new_format_fourth_byte_of_header;
    }

    EmiterUtility::ConvertTeselatesAndCurvesWholeFormat(m_emiter_data, 
                                                        m_prt_file_version_info.particle_version, 
                                                        arg_prt_file_version_info.particle_version);

    m_prt_file_version_info = arg_prt_file_version_info;
}

void EmiterClass::GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset)
{
    m_emiter_data.emiter_name = BinReader::TakeString(buff, curr_offset);

    m_emiter_data.start_time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    uint64_t size_of_slot_strings = BinReader::GetBinVal<uint64_t>(buff, curr_offset);
    for (uint64_t i = 0; i < size_of_slot_strings; ++i)
    {
        m_emiter_data.slot_strings.push_back(BinReader::TakeString(buff, curr_offset));
    }

    m_emiter_data.surface[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.sphere, m_emiter_data.sphere_size, buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.cubic.a, m_emiter_data.cubic_a_size, buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_emiter_data.cubic.b, m_emiter_data.cubic_b_size, buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.plane.a, m_emiter_data.plane_a_size, buff, curr_offset);

    m_emiter_data.texture = BinReader::TakeString(buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_emiter_data.plane.b, m_emiter_data.plane_b_size, buff, curr_offset);

    m_emiter_data.use_color_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_emiter_data.surface, { 1, 2}, buff, curr_offset);

    m_emiter_data.sun_ray_direction_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.create_on_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.trajectory_z_from_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    BinReader::GetBinTabViaIndexes<uint32_t>(m_emiter_data.rotation, { 2, 0, 1}, buff, curr_offset);

    m_emiter_data.enable_light = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    if( (m_prt_file_version_info.particle_version == two_worlds_particle) && 
        (m_fourth_byte_of_header == tw_new_format_fourth_byte_of_header) )
    {
        m_emiter_data.cast_shadows = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    }

    m_emiter_data.umbra_distance_mul = BinReader::GetBinVal<float>(buff, curr_offset);


    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 0, 2, buff, curr_offset);

    m_emiter_data.scales.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.scales.a[12] = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 3, 5, buff, curr_offset);

    m_emiter_data.scales.b[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 6, 10, buff, curr_offset);

    m_emiter_data.move_end_of_laser_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.attach_to_end_of_laser_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_emiter_data.random_seed, m_emiter_data.random_seed_size, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_emiter_data.scales.a, { 11, 13 }, buff, curr_offset);

    m_emiter_data.unknown_value_1 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.unknown_value_2 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.unknown_value_3 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.unknown_value_4 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.unknown_value_5 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
   
    BinReader::GetCurves(m_prt_file_version_info.number_of_iel_in_emiter,
                         m_emiter_data.curves_groups,
                         buff,
                         curr_offset);

    m_emiter_data.curves_groups.curve_bonus_values.resize(m_prt_file_version_info.number_of_iel_in_emiter);


    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_emiter,
                                          m_emiter_data.teselates_groups,
                                          buff,
                                          curr_offset,
                                          true
                                          );

    }
    else
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_emiter,
                                          m_emiter_data.teselates_groups,
                                          buff,
                                          curr_offset
                                          );

        for (size_t i = 0; i < m_prt_file_version_info.number_of_iel_in_emiter; ++i)
            m_emiter_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));
    }

    
    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_emiter; ++i)
        m_emiter_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_emiter_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    
}

void EmiterClass::GetFromE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_emiter_data.emiter_name = BinReader::TakeString(buff, curr_offset);

    m_emiter_data.start_time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.surface[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    BinReader::GetBinTab<float>(m_emiter_data.sphere, m_emiter_data.sphere_size, buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.cubic.a, m_emiter_data.cubic_a_size, buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_emiter_data.cubic.b, m_emiter_data.cubic_b_size, buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.plane.a, m_emiter_data.plane_a_size, buff, curr_offset);

    m_emiter_data.texture = BinReader::TakeString(buff, curr_offset);

    m_emiter_data.plane.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_emiter_data.surface, { 1, 2 }, buff, curr_offset);

    m_emiter_data.enable_light = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.umbra_distance_mul = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 0, 2, buff, curr_offset);

    m_emiter_data.scales.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.scales.a[12] = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 3, 5, buff, curr_offset);

    m_emiter_data.scales.b[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 6, 10, buff, curr_offset);

    m_emiter_data.move_end_of_laser_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.attach_to_end_of_laser_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_emiter_data.random_seed, m_emiter_data.random_seed_size, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_emiter_data.scales.a, { 11, 13 }, buff, curr_offset);

    BinReader::GetCurves(m_prt_file_version_info.number_of_iel_in_emiter,
                         m_emiter_data.curves_groups,
                         buff,
                         curr_offset,
                         true
                         );


    BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_emiter,
                                      m_emiter_data.teselates_groups,
                                      buff,
                                      curr_offset
                                      );

    for (int i = 0; i < m_prt_file_version_info.number_of_iel_in_emiter; ++i)
        m_emiter_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_emiter; ++i)
        m_emiter_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_emiter_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

}


void EmiterClass::GetFromKsFile(uint8_t* buff, size_t& curr_offset)
{
    ++emiter_number;

    m_emiter_data.emiter_name = "E" + to_string(emiter_number);

    m_emiter_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.budget_type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.surface[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.sphere, m_emiter_data.sphere_size, buff, curr_offset);

    BinReader::GetBinTab<float>(m_emiter_data.cubic.a, m_emiter_data.cubic_a_size, buff, curr_offset);
    BinReader::GetBinTab<uint32_t>(m_emiter_data.cubic.b, m_emiter_data.cubic_b_size, buff, curr_offset);


    m_emiter_data.plane.a[0] = BinReader::GetBinVal<float>(buff, curr_offset);
    m_emiter_data.plane.a[1] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_emiter_data.texture = BinReader::TakeString(buff, curr_offset);

    m_emiter_data.plane.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.surface[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.surface[2] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_emiter_data.enable_light = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 0, 2, buff, curr_offset);

    m_emiter_data.scales.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_emiter_data.scales.a[12] = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 3, 5, buff, curr_offset);

    m_emiter_data.scales.b[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabFromTo<float>(m_emiter_data.scales.a, 6, 11, buff, curr_offset);

    BinReader::GetTeselates<uint32_t>(m_prt_file_version_info.number_of_iel_in_emiter,
                                      m_emiter_data.teselates_groups,
                                      buff,
                                      curr_offset
                                      );

    for (int i = 0; i < m_prt_file_version_info.number_of_iel_in_emiter; ++i)
        m_emiter_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_emiter; ++i)
        m_emiter_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_emiter_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    //Interpolacja:
    Utility::MakeCurves(m_emiter_data.teselates_groups, m_emiter_data.curves_groups, e2160_example_curve_point);

}


void EmiterClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case dynamic_particle:
    break;

    case ks_particles_emiter:
        GetFromKsFile(buff, curr_offset);
    break;

    case particle_gen_particle:
    case two_worlds_particle:
        GetFromParticleGenAndTwoWorldsFile(buff, curr_offset);
    break;

    case e2160_particle:
        GetFromE2160File(buff, curr_offset);
    break;

    }

}

void EmiterClass::WriteAsParticleGenAndTwoWorldsFormat(stringstream& output)
{
    AW::WrtVal("Emiter", m_emiter_data.emiter_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("startTime", m_emiter_data.start_time, output);

    AW::WrtVal("time", m_emiter_data.time, output);

    AW::WrtVal("coordinates", m_emiter_data.coordinates, output);

    AW::WriteVector<string>("slotStrings", m_emiter_data.slot_strings, output);

    AW::WriteArrayToAod<uint32_t>("surface",
                                 m_emiter_data.surface,
                                 m_emiter_data.surface_size,
                                 output);

    AW::WrtVal("sunRayDirection", m_emiter_data.sun_ray_direction_switch, output);

    AW::WriteArrayToAod<float>("sphere",
                               m_emiter_data.sphere,
                               m_emiter_data.sphere_size,
                               output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.cubic_a_size, uint32_t, m_emiter_data.cubic_b_size>
                              ("cubic",
                              m_emiter_data.cubic,
                              output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.plane_a_size, uint32_t, m_emiter_data.plane_b_size>
                              ("plane",
                              m_emiter_data.plane,
                              output);

    AW::WrtVal("useColor", m_emiter_data.use_color_switch, output);

    AW::WrtVal("texture", m_emiter_data.texture, output);

    AW::WrtVal("enableLight", m_emiter_data.enable_light, output);

    if(m_prt_file_version_info.particle_version == two_worlds_particle) 
    {
        if (m_fourth_byte_of_header == tw_old_format_fourth_byte_of_header)
        {
            AW::WrtVal("writeCastShadowsToBin", false, output);
        }

        if (m_fourth_byte_of_header == tw_new_format_fourth_byte_of_header)
        {
            AW::WrtVal("writeCastShadowsToBin", true, output);
            AW::WrtVal("castShadows", m_emiter_data.cast_shadows, output);
        }
    }

    AW::WrtVal("umbraDistanceMul", m_emiter_data.umbra_distance_mul, output);

    AW::WriteTwoTypeArrayToAodViaScheme<float, m_emiter_data.scales_a_size, uint32_t, m_emiter_data.scales_b_size>
                                       ("scales", m_emiter_data.scales, "a,a,a,b,a,a,a,b,a,a,a,a,a,a,a,a", output);

    AW::WrtVal("moveEndOfLaser", m_emiter_data.move_end_of_laser_switch, output);

    AW::WrtVal("attachToEndOfLaser", m_emiter_data.attach_to_end_of_laser_switch, output);

    AW::WriteArrayToAod<uint32_t>("randomSeed",
                                  m_emiter_data.random_seed,
                                  m_emiter_data.random_seed_size,
                                  output);

    AW::WrtVal("createOnGround", m_emiter_data.create_on_ground, output);

    AW::WrtVal("trajectoryZFromGround", m_emiter_data.trajectory_z_from_ground, output);

    AW::WriteArrayToAod<uint32_t>("rotation",
                                  m_emiter_data.rotation,
                                  m_emiter_data.rotation_size,
                                  output);

    AW::WrtVal("unknownValue1", m_emiter_data.unknown_value_1, output);

    AW::WrtVal("unknownValue2", m_emiter_data.unknown_value_2, output);

    AW::WrtVal("unknownValue3", m_emiter_data.unknown_value_3, output);

    AW::WrtVal("unknownValue4", m_emiter_data.unknown_value_4, output);

    AW::WrtVal("unknownValue5", m_emiter_data.unknown_value_5, output);

    AW::WriteVector<uint32_t>("linkedMinMax", m_emiter_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_emiter_data.all_teselate_zero_mask, output);

   
    if (PgEmiterStrings.size() < m_prt_file_version_info.number_of_iel_in_emiter)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_emiter - PgEmiterStrings.size();

        for (size_t j = 0; j < delta; ++j)
            PgEmiterStrings.push_back("ExtraField" + to_string(j));
    }

    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        AW::WriteCurvesAndTeselates(m_emiter_data.curves_groups,
                                    m_emiter_data.teselates_groups,
                                    TwEmiterStrings,
                                    output);
    }
    else
    {
        AW::WriteCurvesAndTeselates(m_emiter_data.curves_groups,
                                    m_emiter_data.teselates_groups,
                                    PgEmiterStrings,
                                    output);
    }

    AW::WrtSinStr("}", output);

}


void EmiterClass::WriteAsE2160Format(stringstream& output)
{
    AW::WrtVal("Emiter", m_emiter_data.emiter_name, output, one_quad);
    
    AW::WrtSinStr("{", output);

    AW::WrtVal("startTime", m_emiter_data.start_time, output);

    AW::WrtVal("time", m_emiter_data.time, output);

    AW::WrtVal("coordinates", m_emiter_data.coordinates, output);

    AW::WriteArrayToAod<uint32_t>("surface",
                                  m_emiter_data.surface,
                                  m_emiter_data.surface_size,
                                  output);

    AW::WriteArrayToAod<float>("sphere",
                               m_emiter_data.sphere,
                               m_emiter_data.sphere_size,
                               output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.cubic_a_size, uint32_t, m_emiter_data.cubic_b_size>
                              ("cubic",
                              m_emiter_data.cubic,
                              output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.plane_a_size, uint32_t, m_emiter_data.plane_b_size>
                              ("plane",
                              m_emiter_data.plane,
                              output);

    AW::WrtVal("texture", m_emiter_data.texture, output);

    AW::WrtVal("enableLight", m_emiter_data.enable_light, output);

    AW::WrtVal("umbraDistanceMul", m_emiter_data.umbra_distance_mul, output);

    AW::WriteTwoTypeArrayToAodViaScheme<float, m_emiter_data.scales_a_size, uint32_t, m_emiter_data.scales_b_size>
                                       ("scales", m_emiter_data.scales, "a,a,a,b,a,a,a,b,a,a,a,a,a,a,a,a", output);

    AW::WrtVal("moveEndOfLaser", m_emiter_data.move_end_of_laser_switch, output);

    AW::WrtVal("attachToEndOfLaser", m_emiter_data.attach_to_end_of_laser_switch, output);

    AW::WriteArrayToAod<uint32_t>("randomSeed",
                                 m_emiter_data.random_seed,
                                 m_emiter_data.random_seed_size,
                                 output);

    AW::WriteVector<uint32_t>("linkedMinMax", m_emiter_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_emiter_data.all_teselate_zero_mask, output);

    if (E2160EmiterStrings.size() < m_prt_file_version_info.number_of_iel_in_emiter)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_emiter - E2160EmiterStrings.size();

        for (size_t j = 0; j < delta; ++j)
            E2160EmiterStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_emiter_data.curves_groups,
                               m_emiter_data.teselates_groups,
                               E2160EmiterStrings,
                               output);

    AW::WrtSinStr("}", output);
}


void EmiterClass::WriteAsKsFormat(stringstream& output)
{
    AW::WrtVal("Emiter", m_emiter_data.emiter_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("time", m_emiter_data.time, output);

    AW::WrtVal("coordinates", m_emiter_data.coordinates, output);

    AW::WrtVal("budgetType", m_emiter_data.budget_type, output);

    AW::WriteArrayToAod<uint32_t>("surface",
                                  m_emiter_data.surface,
                                  m_emiter_data.surface_size,
                                  output);

    AW::WriteArrayToAod<float>("sphere",
                              m_emiter_data.sphere,
                              m_emiter_data.sphere_size,
                              output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.cubic_a_size, uint32_t, m_emiter_data.cubic_b_size>
                              ("cubic",
                              m_emiter_data.cubic,
                              output);

    AW::WriteTwoTypeArrayToAod<float, m_emiter_data.plane_a_size, uint32_t, m_emiter_data.plane_b_size>
                              ("plane",
                              m_emiter_data.plane,
                              output);

    AW::WrtVal("enableLight", m_emiter_data.enable_light, output);

    AW::WriteTwoTypeArrayToAodViaScheme<float, m_emiter_data.scales_a_size, uint32_t, m_emiter_data.scales_b_size>
                                       ("scales", m_emiter_data.scales, "a,a,a,b,a,a,a,b,a,a,a,a,a,a,a", output);

    AW::WrtVal("texture", m_emiter_data.texture, output);

    AW::WriteVector<uint32_t>("linkedMinMax", m_emiter_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_emiter_data.all_teselate_zero_mask, output);

    if (KsEmiterStrings.size() < m_prt_file_version_info.number_of_iel_in_emiter)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_emiter - KsEmiterStrings.size();

        for (size_t j = 0; j < delta; ++j)
            KsEmiterStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_emiter_data.curves_groups,
                               m_emiter_data.teselates_groups,
                               KsEmiterStrings,
                               output);
    
    AW::WrtSinStr("}", output);
}


void EmiterClass::WriteToFile(stringstream& output)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case dynamic_particle:
    break;

    case ks_particles_emiter:
        WriteAsKsFormat(output);
    break;

    case particle_gen_particle:
    case two_worlds_particle:
        WriteAsParticleGenAndTwoWorldsFormat(output);
    break;

    case e2160_particle:
        WriteAsE2160Format(output);
    break;

  
    default: break;
    }

}
