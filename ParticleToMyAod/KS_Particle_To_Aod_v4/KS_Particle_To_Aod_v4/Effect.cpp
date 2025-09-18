#include "KS_Particle_To_Aod.hpp"

vector<string> PgEffectStrings = {
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
"rotationYMax"     // ok                // ok

};


EffectData::EffectData() :
particle_2D_switch(0),
width(0),
height(0),
start_time(0.0),
time(0.0),
loops(0),
all_teselate_zero_mask(0),

annotation(string()),

unknown_value_1(0),
unknown_value_2(0),

//e2160:
e2160_effect_scale(0)
{}

EffectClass::EffectClass(ParticleFileVersionInfo arg_prt_file_version_info)
    : m_prt_file_version_info(arg_prt_file_version_info)
{
}

void EffectClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{

    EffectUtility::ConvertTeselatesAndCurvesWholeFormat(m_effect_data,
                                                        m_prt_file_version_info.particle_version,
                                                        arg_prt_file_version_info.particle_version);

    m_prt_file_version_info = arg_prt_file_version_info;
}

void EffectClass::GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset)
{
    m_effect_data.annotation = BinReader::TakeString(buff, curr_offset);

    m_effect_data.particle_2D_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_effect_data.width = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_effect_data.height = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

   
    m_effect_data.start_time = BinReader::GetBinVal<float>(buff, curr_offset);
    m_effect_data.time = BinReader::GetBinVal<float>(buff, curr_offset);
    m_effect_data.loops = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

   
    for (int i = 0; i < m_effect_data.scales_size; ++i)
    {
        BinReader::GetBinTab<float>(m_effect_data.scales[i].a, m_effect_data.scale_a_size, buff, curr_offset);
        m_effect_data.scales[i].b[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    }


    float avg_of_scales = 0.0;
    for (int i = 0; i < m_effect_data.scales_size; ++i)
    {
        for (int j = 0; j < m_effect_data.scale_a_size; ++j)
        {
            avg_of_scales += m_effect_data.scales[i].a[j];
        }
    }
    avg_of_scales /= (m_effect_data.scales_size * m_effect_data.scale_a_size);
    m_effect_data.e2160_effect_scale = static_cast<uint32_t>(avg_of_scales);

    m_effect_data.unknown_value_1 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_effect_data.unknown_value_2 = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    BinReader::GetCurves(m_prt_file_version_info.number_of_iel_in_effect,
                         m_effect_data.curves_groups,
                         buff,
                         curr_offset);


    if (m_prt_file_version_info.particle_version == two_worlds_particle)
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_effect,
                                          m_effect_data.teselates_groups,
                                          buff,
                                          curr_offset,
                                          true);

    }
    else
    {
        BinReader::GetTeselates<uint64_t>(m_prt_file_version_info.number_of_iel_in_effect,
                                          m_effect_data.teselates_groups,
                                          buff,
                                          curr_offset
                                          );

        for (size_t i = 0; i < m_prt_file_version_info.number_of_iel_in_effect; ++i)
           m_effect_data.teselates_groups.teselate_flags.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));
    }

    for (uint32_t i = 0; i < m_prt_file_version_info.number_of_linked_min_max_in_effect; ++i)
       m_effect_data.linked_min_max.push_back(BinReader::GetBinVal<uint32_t>(buff, curr_offset));

    m_effect_data.all_teselate_zero_mask = BinReader::GetBinVal<uint32_t>(buff, curr_offset); 
}

void EffectClass::GetFromE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_effect_data.particle_2D_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_effect_data.width = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_effect_data.height = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_effect_data.e2160_effect_scale = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    for (int i = 0; i < m_effect_data.scales_size; ++i)
    {
        for (int j = 0; j < m_effect_data.scale_a_size; ++j)
        {
            m_effect_data.scales[i].a[j] = static_cast<float>(m_effect_data.e2160_effect_scale);
        }
    }

    m_effect_data.annotation = BinReader::TakeString(buff, curr_offset);
}


void EffectClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{
    switch (m_prt_file_version_info.particle_version)
    {
    
    case particle_gen_particle:
    case two_worlds_particle:
        GetFromParticleGenAndTwoWorldsFile(buff, curr_offset);
    break;

    case e2160_particle:
        GetFromE2160File(buff, curr_offset);
    break;

    case dynamic_particle:
    case ks_particles_emiter:
    break;

    default: break;
    }

}


void EffectClass::WriteAsParticleGenAndTwoWorldsFormat(stringstream& output)
{
    AW::WrtVal("Effect", "EF", output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("particle2D", m_effect_data.particle_2D_switch, output);

    AW::WrtVal("width", m_effect_data.width, output);

    AW::WrtVal("height", m_effect_data.height, output);

    AW::WrtVal("annotation", m_effect_data.annotation, output);

    AW::WrtVal("startTime", m_effect_data.start_time, output);

    AW::WrtVal("time", m_effect_data.time, output);

    AW::WrtVal("loops", m_effect_data.loops, output);

    AW::WriteArrayOfTwoTypeArrayToAod<float, m_effect_data.scale_a_size, uint32_t, m_effect_data.scale_b_size>
                                     ("scales",
                                     m_effect_data.scales,
                                     m_effect_data.scales_size,
                                     two_quads,
                                     output,
                                     true);

    AW::WrtVal("unknownValue1", m_effect_data.unknown_value_1, output);

    AW::WrtVal("unknownValue2", m_effect_data.unknown_value_2, output);

    AW::WriteVector<uint32_t>("linkedMinMax", m_effect_data.linked_min_max, output);

    AW::WrtVal("allTeselateZeroMask", m_effect_data.all_teselate_zero_mask, output);

    if (PgEffectStrings.size() < m_prt_file_version_info.number_of_iel_in_effect)
    {
        size_t delta = m_prt_file_version_info.number_of_iel_in_effect - PgEffectStrings.size();

        for (size_t j = 0; j < delta; ++j)
            PgEffectStrings.push_back("ExtraField" + to_string(j));
    }

    AW::WriteCurvesAndTeselates(m_effect_data.curves_groups,
                                m_effect_data.teselates_groups,
                                PgEffectStrings,
                                output);
    
    AW::WrtSinStr("}", output);
}


void EffectClass::WriteAsE2160Format(stringstream& output) const
{
    AW::WrtVal("Effect", "EF", output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("particle2D", m_effect_data.particle_2D_switch, output);

    AW::WrtVal("width", m_effect_data.width, output);

    AW::WrtVal("height", m_effect_data.height, output);

    AW::WrtVal("scale", m_effect_data.e2160_effect_scale, output);
    
    AW::WrtVal("annotation", m_effect_data.annotation, output);

    AW::WrtSinStr("}", output);
}

void EffectClass::WriteToFile(stringstream& output)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case particle_gen_particle:
    case two_worlds_particle:
        WriteAsParticleGenAndTwoWorldsFormat(output);
    break;

    case e2160_particle:
        WriteAsE2160Format(output);
    break;

    case dynamic_particle:
    case ks_particles_emiter:
    break;

    default: break;
    }

}
