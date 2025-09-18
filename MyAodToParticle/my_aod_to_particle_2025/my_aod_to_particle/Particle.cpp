#include "my_aod_to_particle.hpp"

vector<string> PgAndTwParticleStrings =
{
                    // pg particle:   // two worlds:
"trajectoryXMin",   // ok             // ok
"trajectoryXMax",   // ok             // ok

"trajectoryYMin",   // ok             // ok
"trajectoryYMax",   // ok             // ok

"trajectoryZMin",   // ok             // ok
"trajectoryZMax",   // ok             // ok

"scaleXMin",        // ok             // ok
"scaleXMax",        // ok             // ok

"scaleYMin",        // ok             // ok
"scaleYMax",        // ok             // ok

"scaleZMin",        // ok             // ok
"scaleZMax",        // ok             // ok

"colorRMin",        // ok             // ok
"colorRMax",        // ok             // ok

"colorGMin",        // ok             // ok
"colorGMax",        // ok             // ok

"colorBMin",        // ok             // ok
"colorBMax",        // ok             // ok


"alphaMin",         // ok             // ok
"alphaMax",         // ok             // ok

"rotationZMin",     // ok             // ok
"rotationZMax",     // ok             // ok

"rotationXMin",     // ok             // ok
"rotationXMax",     // ok             // ok

"rotationYMin",     // ok             // ok
"rotationYMax",     // ok             // ok

"speedMulMin",      // ok             // ok
"speedMulMax"       // ok             // ok
};



vector<string> E2160ParticleStrings =
{

"trajectoryXMin",   // ok
"trajectoryXMax",   // ok

"trajectoryYMin",   // ok
"trajectoryYMax",   // ok

"trajectoryZMin",   // ok
"trajectoryZMax",   // ok

"scaleX",           // ok
"scaleY",           // ok
"scaleZ",           // ok

"colorRMin",        // ok
"colorRMax",        // ok

"colorGMin",        // ok
"colorGMax",        // ok

"colorBMin",        // ok
"colorBMax",        // ok

"alphaMin",         // ok 
"alphaMax",         // ok

"rotationZMin",     // ok
"rotationZMax",     // ok

"rotationXMin",     // ok
"rotationXMax",     // ok

"rotationYMin",     // ok
"rotationYMax",     // ok

"speedMulMin",      // ok             
"speedMulMax"       // ok             

};


vector<string> KsParticleStrings =
{
"trajectoryXMin",
"trajectoryXMax",
"trajectoryYMin",
"trajectoryYMax",
"trajectoryZMin",
"trajectoryZMax",
"scaleX",
"scaleY",
"scaleZ",
"colorRMin",
"colorRMax",
"colorGMin",
"colorGMax",
"colorBMin",
"colorBMax",
"colorAMin",
"colorAMax",
"rotationAlpha",
"rotationBeta",
"rotationPhi",
"speedMulMin",
"speedMulMax",
"lightRange",
"lightIntensity",
"lightColorR",
"lightColorG",
"lightColorB"
};


//---------------
// Particle Data
//---------------

ParticleData::ParticleData() :

    particle_name(std::string()),

    start_time(0.0),
    time(0.0),

    coordinates(0),
    type(0),

    mesh(std::string()),

    triangle_a(0.0),
    screen_depth_offset(0.0),

    multiply_dxy_by_prt_scale_switch(0),

    y_texture_mul(0.0),

    distortion_particle_switch(0),

    x_mirrored(0),
    y_mirrored(0),

    color_rand_type(0),

    unknown_value_1(0),
    unknown_value_2(0),
    unknown_value_3(0),

    enable_light(0),

    gravity_rot_from_trajectory_z_switch(0),
    gravity_rot_from_trajectory_x_switch(0),
    gravity_rot_from_trajectory_y_switch(0),

    dissappears_on_ground(0),

    create_on_ground_switch(0),
    trajectory_z_from_ground(0),

    scales_end(0.0),

    all_teselate_zero_mask(0),

    layer(0)

{
}

void ParticleClass::InitializeHashMap()
{
    switch (m_prt_file_version_info.particle_version)
    {
    case particle_gen_particle:
    case two_worlds_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, PgAndTwParticleStrings);
    break;

    case e2160_particle:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, E2160ParticleStrings);
    break;

    case ks_particles_emiter:
        Utility::InitializeHashMapViaLabelsVector(m_labels_map, KsParticleStrings);
    break;

    case dynamic_particle:
    break;

    default: break;
    }
}

ParticleClass::ParticleClass(ParticleFileVersionInfo arg_prt_file_version_info, string arg_particle_name)
    : m_prt_file_version_info(arg_prt_file_version_info),

    m_curve_iel_index(-1),
    m_number_of_single_curve_points_checksum(0),

    m_teselate_iel_index(-1),
    m_number_of_single_teselate_points_checksum(0),

    m_mode(null)
{
    m_particle_data.particle_name = arg_particle_name;

    InitializeHashMap();

    m_particle_data.linked_min_max.resize(m_prt_file_version_info.number_of_linked_min_max_in_particle, locked_linked_min_max);

    m_particle_data.curves_groups.resize(m_prt_file_version_info.number_of_iel_in_particle);

    m_particle_data.teselates_groups.resize(m_prt_file_version_info.number_of_iel_in_particle);

}

void ParticleClass::GetFromFile(string& arg_line)
{
    AU::GetOneValue<float>(arg_line, "startTime", "%f", m_particle_data.start_time);

    AU::GetOneValue<float>(arg_line, "time", "%f", m_particle_data.time);

    AU::GetOneValue<uint32_t>(arg_line, "coordinates", "%d", m_particle_data.coordinates);

    AU::GetOneValue<uint32_t>(arg_line, "type", "%d", m_particle_data.type);

    AU::GetSingleStringFromQueue(arg_line, "mesh", m_particle_data.mesh, GlobalQueues::m_mesh_q);

    AU::GetTwoTypeTabValues<uint32_t, m_particle_data.face_a_size, float, m_particle_data.face_b_size>
        (arg_line, "face", "%d", "%f", m_particle_data.face);

    AU::GetOneValue<float>(arg_line, "triangleA", "%f", m_particle_data.triangle_a);

    AU::GetOneValue<float>(arg_line, "screenDepthOffset", "%f", m_particle_data.screen_depth_offset);

    AU::GetOneValue<uint32_t>(arg_line, "multiplyDxyByPrtScale", "%d", m_particle_data.multiply_dxy_by_prt_scale_switch);

    AU::GetTwoTypeTabValues<float, m_particle_data.trail_a_size, uint32_t, m_particle_data.trail_b_size>
        (arg_line, "trail", "%f", "%d", m_particle_data.trail);

    AU::GetOneValue<float>(arg_line, "yTextureMul", "%f", m_particle_data.y_texture_mul);

    AU::GetVectorOfStringsFromQueue(arg_line, "texture", m_particle_data.textures, GlobalQueues::m_texture_q);

    AU::GetVectorOfStringsFromQueue(arg_line, "bumpTexture", m_particle_data.bump_textures, GlobalQueues::m_bump_texture_q);


    AU::GetTwoTypeTabValuesViaScheme<uint32_t, m_particle_data.tex_anim_a_size, float, m_particle_data.tex_anim_b_size>
        (arg_line, "texAnim", "%d", "%f", "a,a,a,a,a,a,a,a,a,b,a", m_particle_data.tex_anim);


    AU::GetTabValues<float>(arg_line, "coordinatesLTRB", "%f", m_particle_data.coordinates_ltrb, m_particle_data.coordinates_ltrb_size);

    AU::GetOneValue<uint32_t>(arg_line, "distortionParticleSwitch", "%d", m_particle_data.distortion_particle_switch);

    AU::GetOneValue<uint32_t>(arg_line, "xMirrored", "%d", m_particle_data.x_mirrored);

    AU::GetOneValue<uint32_t>(arg_line, "yMirrored", "%d", m_particle_data.y_mirrored);

    AU::GetOneValue<uint32_t>(arg_line, "colorRandType", "%d", m_particle_data.color_rand_type);

    AU::GetOneValue<uint32_t>(arg_line, "unknownValue1", "%d", m_particle_data.unknown_value_1);

    AU::GetOneValue<uint32_t>(arg_line, "unknownValue2", "%d", m_particle_data.unknown_value_2);

    AU::GetOneValue<uint32_t>(arg_line, "unknownValue3", "%d", m_particle_data.unknown_value_3);

    AU::GetTabValues<uint32_t>(arg_line, "trajectory", "%d", m_particle_data.trajectory, m_particle_data.trajectory_size);

    AU::GetTwoTypeTabValues<float, m_particle_data.gravity_a_size, uint32_t, m_particle_data.gravity_b_size>
        (arg_line, "gravity", "%f", "%d", m_particle_data.gravity);

    AU::GetOneValue<uint32_t>(arg_line, "enableLight", "%d", m_particle_data.enable_light);

    AU::GetOneValue<uint32_t>(arg_line, "gravityRotFromTrajectoryX", "%d", m_particle_data.gravity_rot_from_trajectory_x_switch);

    AU::GetOneValue<uint32_t>(arg_line, "gravityRotFromTrajectoryY", "%d", m_particle_data.gravity_rot_from_trajectory_y_switch);

    AU::GetOneValue<uint32_t>(arg_line, "gravityRotFromTrajectoryZ", "%d", m_particle_data.gravity_rot_from_trajectory_z_switch);

    AU::GetOneValue<uint32_t>(arg_line, "disappearsOnGround", "%d", m_particle_data.dissappears_on_ground);

    AU::GetOneValue<uint32_t>(arg_line, "createOnGround", "%d", m_particle_data.create_on_ground_switch);

    AU::GetOneValue<uint32_t>(arg_line, "trajectoryZFromGround", "%d", m_particle_data.trajectory_z_from_ground);


    AU::GetArrayOfTwoTypeTabValues_PlusBonus<float, m_particle_data.scale_a_size, uint32_t, m_particle_data.scale_b_size>
                                            (arg_line, 
                                            "scales", 
                                            "%f", 
                                            "%d", 
                                            m_particle_data.scales, 
                                            m_particle_data.scales_size, 
                                            m_particle_data.scales_end);


    AU::GetOneValue<uint32_t>(arg_line, "allTeselateZeroMask", "%d", m_particle_data.all_teselate_zero_mask);

    AU::GetOneValue<uint32_t>(arg_line, "layer", "%d", m_particle_data.layer);

    AU::GetTabValues<uint32_t>(arg_line, 
                               "linkedMinMax", 
                               "%d", 
                               &m_particle_data.linked_min_max[0], 
                               static_cast<int>(m_particle_data.linked_min_max.size()) );

    AU::GetCurveHeader(arg_line,
        m_particle_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetCurves(arg_line,
        m_particle_data.curves_groups,
        m_curve_iel_index,
        m_number_of_single_curve_points_checksum,
        m_mode);


    AU::GetTeselateHeader(arg_line,
        m_particle_data.teselates_groups,
        m_teselate_iel_index,
        m_number_of_single_teselate_points_checksum,
        m_labels_map,
        m_mode);


    AU::GetTeselates(arg_line,
        m_particle_data.teselates_groups,
        m_teselate_iel_index,
        m_number_of_single_teselate_points_checksum,
        m_mode);


}


void ParticleClass::WriteToFile(vector<char>& output_file_buff)
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


void ParticleClass::WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_particle_data.particle_name);

    BW::WriteValue(output_file_buff, m_particle_data.start_time);

    BW::WriteValue(output_file_buff, m_particle_data.time);

    BW::WriteValue(output_file_buff, m_particle_data.coordinates);

    BW::WriteValue(output_file_buff, m_particle_data.type);

    BW::WriteString(output_file_buff, m_particle_data.mesh);

    BW::WriteTabFromTo(output_file_buff, m_particle_data.face.b, 3, 4);

    BW::WriteValue(output_file_buff, m_particle_data.screen_depth_offset);

    BW::WriteTabFromTo(output_file_buff, m_particle_data.face.b, 1, 2);

    BW::WriteValue(output_file_buff, m_particle_data.multiply_dxy_by_prt_scale_switch);

    BW::WriteValue(output_file_buff, m_particle_data.trail.a);

    BW::WriteValue(output_file_buff, m_particle_data.y_texture_mul);

    BW::WriteValue(output_file_buff, m_particle_data.trail.b);

    BW::WriteVectorOfStrings(output_file_buff, m_particle_data.textures);

    BW::WriteVectorOfStrings(output_file_buff, m_particle_data.bump_textures);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[1]);

    BW::WriteTabFromTo(output_file_buff, m_particle_data.tex_anim.a, 0, m_particle_data.tex_anim_a_size - 5);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.tex_anim.a, { 8, 9 } );

    BW::WriteValue(output_file_buff, m_particle_data.coordinates_ltrb);

    BW::WriteValue(output_file_buff, m_particle_data.tex_anim.a[6]);

    BW::WriteValue(output_file_buff, m_particle_data.face.b[0]);

    BW::WriteValue(output_file_buff, m_particle_data.distortion_particle_switch);

    BW::WriteValue(output_file_buff, m_particle_data.tex_anim.a[7]);

    BW::WriteValue(output_file_buff, m_particle_data.tex_anim.b[0]);

    BW::WriteValue(output_file_buff, m_particle_data.x_mirrored);

    BW::WriteValue(output_file_buff, m_particle_data.y_mirrored);

    BW::WriteValue(output_file_buff, m_particle_data.color_rand_type);

    BW::WriteValue(output_file_buff, m_particle_data.unknown_value_1);

    BW::WriteValue(output_file_buff, m_particle_data.unknown_value_2);

    BW::WriteValue(output_file_buff, m_particle_data.unknown_value_3);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[0]);

    BW::WriteValue(output_file_buff, m_particle_data.gravity.a);

    BW::WriteValue(output_file_buff, m_particle_data.gravity_rot_from_trajectory_z_switch);

    BW::WriteValue(output_file_buff, m_particle_data.gravity_rot_from_trajectory_x_switch);

    BW::WriteValue(output_file_buff, m_particle_data.gravity_rot_from_trajectory_y_switch);

    BW::WriteValue(output_file_buff, m_particle_data.dissappears_on_ground);

    BW::WriteValue(output_file_buff, m_particle_data.create_on_ground_switch);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory_z_from_ground);

    BW::WriteValue(output_file_buff, m_particle_data.scales);

    BW::WriteCurves(output_file_buff, m_particle_data.curves_groups);


    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BW::WriteTeselates<uint64_t>(output_file_buff,
                                     m_particle_data.teselates_groups,
                                     true);
    }
    else
    {
        BW::WriteTeselates<uint64_t>(output_file_buff, m_particle_data.teselates_groups);

        BW::WriteVector(output_file_buff, m_particle_data.teselates_groups.teselate_flags);
    }

    BW::WriteVector(output_file_buff, m_particle_data.linked_min_max);

    BW::WriteValue(output_file_buff, m_particle_data.all_teselate_zero_mask);

    BW::WriteValue(output_file_buff, m_particle_data.layer);

}

void ParticleClass::WriteToE2160Format(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_particle_data.particle_name);

    BW::WriteValue(output_file_buff, m_particle_data.start_time);

    BW::WriteValue(output_file_buff, m_particle_data.time);

    BW::WriteValue(output_file_buff, m_particle_data.coordinates);

    BW::WriteValue(output_file_buff, m_particle_data.type);

    BW::WriteString(output_file_buff, m_particle_data.mesh);

    BW::WriteValue(output_file_buff, m_particle_data.face.a[0]);

    BW::WriteValue(output_file_buff, m_particle_data.triangle_a);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.face.b, { 3, 4, 0, 1, 2 });

    BW::WriteValue(output_file_buff, m_particle_data.multiply_dxy_by_prt_scale_switch);

    BW::WriteValue(output_file_buff, m_particle_data.trail.a);

    BW::WriteValue(output_file_buff, m_particle_data.y_texture_mul);

    BW::WriteValue(output_file_buff, m_particle_data.trail.b);

    BW::WriteFirstStringFromVector(output_file_buff, m_particle_data.textures);

    BW::WriteFirstStringFromVector(output_file_buff, m_particle_data.bump_textures);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[1]);

    BW::WriteTabFromTo(output_file_buff, m_particle_data.tex_anim.a, 0, m_particle_data.tex_anim_a_size - 5);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.tex_anim.a, { 8, 9 });

    BW::WriteValue(output_file_buff, m_particle_data.coordinates_ltrb);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.tex_anim.a, { 6, 7 });

    BW::WriteValue(output_file_buff, m_particle_data.tex_anim.b[0]);

    BW::WriteValue(output_file_buff, m_particle_data.x_mirrored);

    BW::WriteValue(output_file_buff, m_particle_data.y_mirrored);

    BW::WriteValue(output_file_buff, m_particle_data.color_rand_type);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[0]);

    BW::WriteValue(output_file_buff, m_particle_data.gravity);

    BW::WriteValue(output_file_buff, m_particle_data.dissappears_on_ground);

    BW::WriteValue(output_file_buff, m_particle_data.scales);

    BW::WriteCurves(output_file_buff, m_particle_data.curves_groups, true);

    BW::WriteTeselates<uint64_t>(output_file_buff, m_particle_data.teselates_groups);

    BW::WriteVector(output_file_buff, m_particle_data.teselates_groups.teselate_flags);

    BW::WriteVector(output_file_buff, m_particle_data.linked_min_max);

    BW::WriteValue(output_file_buff, m_particle_data.all_teselate_zero_mask);

    BW::WriteValue(output_file_buff, m_particle_data.layer);
}


void ParticleClass::WriteToKsFormat(vector<char>& output_file_buff)
{
    BW::WriteValue(output_file_buff, m_particle_data.time);

    BW::WriteValue(output_file_buff, m_particle_data.coordinates);

    BW::WriteValue(output_file_buff, m_particle_data.type);

    BW::WriteString(output_file_buff, m_particle_data.mesh, true);

    BW::WriteValue(output_file_buff, m_particle_data.face.a[0]);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[1]);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.face.b, { 3, 4, 0, 1, 2 });

    BW::WriteValue(output_file_buff, m_particle_data.trail);

    BW::WriteFirstStringFromVector(output_file_buff, m_particle_data.textures, true);

    BW::WriteTabFromTo(output_file_buff, m_particle_data.tex_anim.a, 0, m_particle_data.tex_anim_a_size - 5);

    BW::WriteTabViaIndexes(output_file_buff, m_particle_data.tex_anim.a, { 8, 9, 6, 7 });

    BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(m_particle_data.tex_anim.b[0]) );

    BW::WriteValue(output_file_buff, m_particle_data.color_rand_type);

    BW::WriteValue(output_file_buff, m_particle_data.trajectory[0]);

    BW::WriteValue(output_file_buff, m_particle_data.gravity);

    BW::WriteValue(output_file_buff, m_particle_data.enable_light);

    BW::WriteValue(output_file_buff, m_particle_data.dissappears_on_ground);

    BW::WriteValue(output_file_buff, m_particle_data.scales);

    BW::WriteValue(output_file_buff, m_particle_data.scales_end);

    BW::WriteTeselates<uint32_t>(output_file_buff, m_particle_data.teselates_groups);

    BW::WriteVector(output_file_buff, m_particle_data.teselates_groups.teselate_flags);

    BW::WriteVector(output_file_buff, m_particle_data.linked_min_max);

    BW::WriteValue(output_file_buff, m_particle_data.all_teselate_zero_mask);

    BW::WriteValue(output_file_buff, m_particle_data.layer);

}
