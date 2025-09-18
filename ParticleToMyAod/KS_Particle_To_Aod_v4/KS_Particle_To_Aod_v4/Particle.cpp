#include "KS_Particle_To_Aod.hpp"

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

"rotationZMin",     //ok
"rotationZMax",     //ok

"rotationXMin",     //ok
"rotationXMax",     //ok

"rotationYMin",     //ok
"rotationYMax",     //ok

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


uint32_t ParticleClass::particle_number = 0;

//---------------
// Particle Data
//---------------

ParticleData::ParticleData() :

particle_name(string()),

start_time(0.0),
time(0.0),

coordinates(0),
type(0),

mesh(string()),

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

{}

ParticleClass::ParticleClass(ParticleFileVersionInfo arg_prt_file_version_info)
    : m_prt_file_version_info(arg_prt_file_version_info)
{
}

void ParticleClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    m_particle_data.linked_min_max.resize(arg_prt_file_version_info.number_of_linked_min_max_in_particle, locked_linked_min_max);

    ParticleUtility::ConvertTeselatesAndCurvesWholeFormat(m_particle_data,
                                                          m_prt_file_version_info.particle_version,
                                                          arg_prt_file_version_info.particle_version);

    m_prt_file_version_info = arg_prt_file_version_info;
}



void ParticleClass::GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset)
{
    m_particle_data.particle_name = BinReader::TakeString(buff, curr_offset);

    m_particle_data.start_time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.mesh = BinReader::TakeString(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_particle_data.face.b, { 3, 4 }, buff, curr_offset);

    m_particle_data.screen_depth_offset = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_particle_data.face.b, { 1, 2 }, buff, curr_offset);

    m_particle_data.multiply_dxy_by_prt_scale_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trail.a[0] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.y_texture_mul = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.trail.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    uint64_t size_of_textures_vec = BinReader::GetBinVal<uint64_t>(buff, curr_offset);
    for (uint64_t i = 0; i < size_of_textures_vec; ++i)
    {
        m_particle_data.textures.push_back(BinReader::TakeString(buff, curr_offset));
    }


    uint64_t size_of_bump_textures_vec = BinReader::GetBinVal<uint64_t>(buff, curr_offset);
    for (uint64_t i = 0; i < size_of_bump_textures_vec; ++i)
    {
        m_particle_data.bump_textures.push_back(BinReader::TakeString(buff, curr_offset));
    }

    m_particle_data.trajectory[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_particle_data.tex_anim.a, m_particle_data.tex_anim_a_size - 4, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_particle_data.tex_anim.a, { 8, 9 }, buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_data.coordinates_ltrb, m_particle_data.coordinates_ltrb_size, buff, curr_offset);

    m_particle_data.tex_anim.a[6] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.face.b[0] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.distortion_particle_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.tex_anim.a[7] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.tex_anim.b[0] = BinReader::GetBinVal<float>(buff, curr_offset);


    m_particle_data.x_mirrored = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.y_mirrored = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.color_rand_type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.unknown_value_1 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.unknown_value_2 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.unknown_value_3 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trajectory[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_data.gravity.a, m_particle_data.gravity_a_size, buff, curr_offset);

    m_particle_data.gravity_rot_from_trajectory_z_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.gravity_rot_from_trajectory_x_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.gravity_rot_from_trajectory_y_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.dissappears_on_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.create_on_ground_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.trajectory_z_from_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    for (int i = 0; i < m_particle_data.scales_size; ++i)
    {
        BinReader::GetBinTab<float>(m_particle_data.scales[i].a, m_particle_data.scale_a_size, buff, curr_offset);
        m_particle_data.scales[i].b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    }

    BinReader::GetCurves(m_prt_file_version_info.number_of_iel_in_particle,
                         m_particle_data.curves_groups,
                         buff,
                         curr_offset);

    m_particle_data.curves_groups.curve_bonus_values.resize(m_prt_file_version_info.number_of_iel_in_emiter);


    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_particle,
                                          m_particle_data.teselates_groups,
                                          buff,
                                          curr_offset,
                                          true
                                          );

    }
    else
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_particle,
                                          m_particle_data.teselates_groups,
                                          buff,
                                          curr_offset
                                          );

        for (size_t i = 0; i < m_prt_file_version_info.number_of_iel_in_particle; ++i)
            m_particle_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));
    }


    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_particle; ++i)
        m_particle_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_particle_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.layer = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
}

void ParticleClass::GetFromE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_particle_data.particle_name = BinReader::TakeString(buff, curr_offset);

    m_particle_data.start_time = BinReader::GetBinVal<float>(buff, curr_offset);
    m_particle_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.mesh = BinReader::TakeString(buff, curr_offset);

    m_particle_data.face.a[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.triangle_a = BinReader::GetBinVal<float>(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_particle_data.face.b, { 3, 4, 0, 1, 2 }, buff, curr_offset);

    m_particle_data.multiply_dxy_by_prt_scale_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trail.a[0] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.y_texture_mul = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.trail.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.textures.push_back( BinReader::TakeString(buff, curr_offset) );

    m_particle_data.bump_textures.push_back( BinReader::TakeString(buff, curr_offset) );

    m_particle_data.trajectory[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<uint32_t>(m_particle_data.tex_anim.a, m_particle_data.tex_anim_a_size - 4, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_particle_data.tex_anim.a, { 8, 9 }, buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_data.coordinates_ltrb, m_particle_data.coordinates_ltrb_size, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_particle_data.tex_anim.a, { 6, 7 }, buff, curr_offset);

    m_particle_data.tex_anim.b[0] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.x_mirrored = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_particle_data.y_mirrored = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.color_rand_type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trajectory[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_data.gravity.a, m_particle_data.gravity_a_size, buff, curr_offset);
    m_particle_data.gravity.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.dissappears_on_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    for (int i = 0; i < m_particle_data.scales_size; ++i)
    {
        BinReader::GetBinTab<float>(m_particle_data.scales[i].a, m_particle_data.scale_a_size, buff, curr_offset);
        m_particle_data.scales[i].b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    }

    BinReader::GetCurves(m_prt_file_version_info.number_of_iel_in_particle,
                         m_particle_data.curves_groups,
                         buff,
                         curr_offset,
                         true
                         );


    BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_particle,
                                      m_particle_data.teselates_groups,
                                      buff,
                                      curr_offset
                                      );


    for (int i = 0; i < m_prt_file_version_info.number_of_iel_in_particle; ++i)
        m_particle_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_particle; ++i)
        m_particle_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    m_particle_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.layer = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


}


void ParticleClass::GetFromKsFile(uint8_t* buff, size_t& curr_offset)
{
    ++particle_number;

    m_particle_data.particle_name = "P" + to_string(particle_number);


    m_particle_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.coordinates = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.mesh = BinReader::TakeString(buff, curr_offset);

    m_particle_data.face.a[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trajectory[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTabViaIndexes<float>(m_particle_data.face.b, { 3, 4, 0, 1, 2 }, buff, curr_offset);

    m_particle_data.trail.a[0] = BinReader::GetBinVal<float>(buff, curr_offset);

    m_particle_data.trail.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.textures.push_back( BinReader::TakeString(buff, curr_offset) );

    BinReader::GetBinTab<uint32_t>(m_particle_data.tex_anim.a, m_particle_data.tex_anim_a_size - 4, buff, curr_offset);

    BinReader::GetBinTabViaIndexes<uint32_t>(m_particle_data.tex_anim.a, { 8, 9, 6, 7 }, buff, curr_offset);

    m_particle_data.tex_anim.b[0] = static_cast<float>(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_particle_data.color_rand_type = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.trajectory[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetBinTab<float>(m_particle_data.gravity.a, m_particle_data.gravity_a_size, buff, curr_offset);
    m_particle_data.gravity.b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    m_particle_data.enable_light = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.dissappears_on_ground = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    for (int i = 0; i < m_particle_data.scales_size; ++i)
    {
        BinReader::GetBinTab<float>(m_particle_data.scales[i].a, m_particle_data.scale_a_size, buff, curr_offset);
        m_particle_data.scales[i].b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    }

    m_particle_data.scales_end = BinReader::GetBinVal<float>(buff, curr_offset);


    BinReader::GetTeselates<uint32_t>(m_prt_file_version_info.number_of_iel_in_particle,
                                      m_particle_data.teselates_groups,
                                      buff,
                                      curr_offset
                                      );

    for (int i = 0; i < m_prt_file_version_info.number_of_iel_in_particle; ++i)
        m_particle_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_particle; ++i)
        m_particle_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));


    m_particle_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_particle_data.layer = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    //Interpolacja:
    Utility::MakeCurves(m_particle_data.teselates_groups, m_particle_data.curves_groups, e2160_example_curve_point);

}


void ParticleClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
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


void ParticleClass::WriteAsParticleGenAndTwoWorldsFormat(stringstream& output)
{
    AW::WrtVal("Particle", m_particle_data.particle_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("startTime", m_particle_data.start_time, output);

    AW::WrtVal("time", m_particle_data.time, output);

    AW::WrtVal("coordinates", m_particle_data.coordinates, output);

    AW::WrtVal("type", m_particle_data.type, output);

    AW::WrtVal("mesh", m_particle_data.mesh, output);

    AW::WriteTwoTypeArrayToAod<uint32_t, m_particle_data.face_a_size, float, m_particle_data.face_b_size>
                              ("face",
                              m_particle_data.face,
                              output);

    AW::WrtVal("screenDepthOffset", m_particle_data.screen_depth_offset, output);

    AW::WrtVal("multiplyDxyByPrtScale", m_particle_data.multiply_dxy_by_prt_scale_switch, output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.trail_a_size, uint32_t, m_particle_data.trail_b_size>
                              ("trail",
                              m_particle_data.trail,
                              output);

    AW::WrtVal("yTextureMul", m_particle_data.y_texture_mul, output);

    AW::WriteVector<string>("texture", m_particle_data.textures, output);

    AW::WriteVector<string>("bumpTexture", m_particle_data.bump_textures, output);

    AW::WriteTwoTypeArrayToAodViaScheme<uint32_t, m_particle_data.tex_anim_a_size, float, m_particle_data.tex_anim_b_size>
        ("texAnim", m_particle_data.tex_anim, "a,a,a,a,a,a,a,a,a,b,a", output);


    AW::WriteArrayToAod<float>("coordinatesLTRB",
                              m_particle_data.coordinates_ltrb,
                              m_particle_data.coordinates_ltrb_size,
                              output);

    AW::WrtVal("distortionParticleSwitch", m_particle_data.distortion_particle_switch, output);

    AW::WrtVal("xMirrored", m_particle_data.x_mirrored, output);

    AW::WrtVal("yMirrored", m_particle_data.y_mirrored, output);

    AW::WrtVal("colorRandType", m_particle_data.color_rand_type, output);

    AW::WrtVal("unknownValue1", m_particle_data.unknown_value_1, output);

    AW::WrtVal("unknownValue2", m_particle_data.unknown_value_2, output);

    AW::WrtVal("unknownValue3", m_particle_data.unknown_value_3, output);

    AW::WriteArrayToAod<uint32_t>("trajectory",
                                 m_particle_data.trajectory,
                                 m_particle_data.trajectory_size,
                                 output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.gravity_a_size, uint32_t, m_particle_data.gravity_b_size>
                              ("gravity",
                              m_particle_data.gravity,
                              output);

    AW::WrtVal("gravityRotFromTrajectoryX", m_particle_data.gravity_rot_from_trajectory_x_switch, output);
    
    AW::WrtVal("gravityRotFromTrajectoryY", m_particle_data.gravity_rot_from_trajectory_y_switch, output);
    
    AW::WrtVal("gravityRotFromTrajectoryZ", m_particle_data.gravity_rot_from_trajectory_z_switch, output);

    AW::WrtVal("disappearsOnGround", m_particle_data.dissappears_on_ground, output);

    AW::WrtVal("createOnGround", m_particle_data.create_on_ground_switch, output);

    AW::WrtVal("trajectoryZFromGround", m_particle_data.trajectory_z_from_ground, output);

    AW::WriteArrayOfTwoTypeArrayToAod<float, m_particle_data.scale_a_size, uint32_t, m_particle_data.scale_b_size>
                                     ("scales",
                                     m_particle_data.scales,
                                     m_particle_data.scales_size,
                                     two_quads,
                                     output,
                                     true);

    AW::WriteVector<uint32_t>("linkedMinMax", m_particle_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_particle_data.all_teselate_zero_mask, output);

    AW::WrtVal("layer", m_particle_data.layer, output);

    if (PgAndTwParticleStrings.size() < m_prt_file_version_info.number_of_iel_in_particle)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_particle - PgAndTwParticleStrings.size();

        for (size_t j = 0; j < delta; ++j)
            PgAndTwParticleStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_particle_data.curves_groups,
                               m_particle_data.teselates_groups,
                               PgAndTwParticleStrings,
                               output);

    AW::WrtSinStr("}", output);
}




void ParticleClass::WriteAsE2160Format(stringstream& output)
{
    AW::WrtVal("Particle", m_particle_data.particle_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("startTime", m_particle_data.start_time, output);

    AW::WrtVal("time", m_particle_data.time, output);

    AW::WrtVal("coordinates", m_particle_data.coordinates, output);

    AW::WrtVal("type", m_particle_data.type, output);

    AW::WrtVal("mesh", m_particle_data.mesh, output);

    AW::WriteTwoTypeArrayToAod<uint32_t, m_particle_data.face_a_size, float, m_particle_data.face_b_size>
                              ("face",
                              m_particle_data.face,
                              output);

    AW::WrtVal("triangleA", m_particle_data.triangle_a, output);

    AW::WrtVal("multiplyDxyByPrtScale", m_particle_data.multiply_dxy_by_prt_scale_switch, output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.trail_a_size, uint32_t, m_particle_data.trail_b_size>
                              ("trail",
                              m_particle_data.trail,
                              output);

    AW::WrtVal("yTextureMul", m_particle_data.y_texture_mul, output);

    AW::WriteVector<string>("texture", m_particle_data.textures, output);

    AW::WriteVector<string>("bumpTexture", m_particle_data.bump_textures, output);

    AW::WriteTwoTypeArrayToAodViaScheme<uint32_t, m_particle_data.tex_anim_a_size, float, m_particle_data.tex_anim_b_size>
        ("texAnim", m_particle_data.tex_anim, "a,a,a,a,a,a,a,a,a,b,a", output);


    AW::WriteArrayToAod<float>("coordinatesLTRB",
                              m_particle_data.coordinates_ltrb,
                              m_particle_data.coordinates_ltrb_size,
                              output);

    AW::WrtVal("xMirrored", m_particle_data.x_mirrored, output);

    AW::WrtVal("yMirrored", m_particle_data.y_mirrored, output);

    AW::WrtVal("colorRandType", m_particle_data.color_rand_type, output);

    AW::WriteArrayToAod<uint32_t>("trajectory",
                                 m_particle_data.trajectory,
                                 m_particle_data.trajectory_size,
                                 output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.gravity_a_size, uint32_t, m_particle_data.gravity_b_size>
                              ("gravity",
                              m_particle_data.gravity,
                              output);

    AW::WrtVal("disappearsOnGround", m_particle_data.dissappears_on_ground, output);

    AW::WriteArrayOfTwoTypeArrayToAod<float, m_particle_data.scale_a_size, uint32_t, m_particle_data.scale_b_size>
                                     ("scales",
                                     m_particle_data.scales,
                                     m_particle_data.scales_size,
                                     two_quads,
                                     output,
                                     true);

    AW::WriteVector<uint32_t>("linkedMinMax", m_particle_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_particle_data.all_teselate_zero_mask, output);

    AW::WrtVal("layer", m_particle_data.layer, output);

    if (E2160ParticleStrings.size() < m_prt_file_version_info.number_of_iel_in_particle)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_particle - E2160ParticleStrings.size();

        for (size_t j = 0; j < delta; ++j)
            E2160ParticleStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_particle_data.curves_groups,
                               m_particle_data.teselates_groups,
                               E2160ParticleStrings,
                               output);

    AW::WrtSinStr("}", output);
}


void ParticleClass::WriteAsKsFormat(stringstream& output)
{
    AW::WrtVal("Particle", m_particle_data.particle_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("time", m_particle_data.time, output);

    AW::WrtVal("coordinates", m_particle_data.coordinates, output);

    AW::WrtVal("type", m_particle_data.type, output);

    AW::WrtVal("mesh", m_particle_data.mesh, output);

    AW::WriteTwoTypeArrayToAod<uint32_t, m_particle_data.face_a_size, float, m_particle_data.face_b_size>
                              ("face",
                              m_particle_data.face,
                              output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.trail_a_size, uint32_t, m_particle_data.trail_b_size>
                              ("trail",
                              m_particle_data.trail,
                              output);

    AW::WriteVector<string>("texture", m_particle_data.textures, output);

    AW::WriteTwoTypeArrayToAodViaScheme<uint32_t, m_particle_data.tex_anim_a_size, float, m_particle_data.tex_anim_b_size>
        ("texAnim", m_particle_data.tex_anim, "a,a,a,a,a,a,a,a,a,b,a", output);

    AW::WriteArrayToAod<uint32_t>("trajectory",
                                 m_particle_data.trajectory,
                                 m_particle_data.trajectory_size,
                                 output);

    AW::WriteTwoTypeArrayToAod<float, m_particle_data.gravity_a_size, uint32_t, m_particle_data.gravity_b_size>
                              ("gravity",
                              m_particle_data.gravity,
                              output);

    AW::WrtVal("enableLight", m_particle_data.enable_light, output);

    AW::WrtVal("disappearsOnGround", m_particle_data.dissappears_on_ground, output);

    AW::WrtVal("colorRandType", m_particle_data.color_rand_type, output);

    AW::WriteArrayOfTwoTypeArrayToAod<float, m_particle_data.scale_a_size, uint32_t, m_particle_data.scale_b_size>
                                     ("scales",
                                     m_particle_data.scales,
                                     m_particle_data.scales_size,
                                     two_quads,
                                     output,
                                     false);

    output << m_particle_data.scales_end << endl;

    AW::WriteVector<uint32_t>("linkedMinMax", m_particle_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_particle_data.all_teselate_zero_mask, output);

    AW::WrtVal("layer", m_particle_data.layer, output);

    if (KsParticleStrings.size() < m_prt_file_version_info.number_of_iel_in_particle)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_particle - KsParticleStrings.size();

        for (size_t j = 0; j < delta; ++j)
            KsParticleStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_particle_data.curves_groups,
                               m_particle_data.teselates_groups,
                               KsParticleStrings,
                               output);

    AW::WrtSinStr("}", output);
}


void ParticleClass::WriteToFile(stringstream& output)
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
