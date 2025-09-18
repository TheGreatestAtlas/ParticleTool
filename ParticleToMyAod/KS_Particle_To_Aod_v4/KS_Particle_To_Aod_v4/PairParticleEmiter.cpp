#include "KS_Particle_To_Aod.hpp"

//-------------------------
// Metody PgPairParticleEmiterData
//-------------------------

PairParticleEmiterData::PairParticleEmiterData() :

    group_name(string()),

    annotation(string()),

    emiter_name(string()),
    particle_name(string()),

    emiter_index(0),
    particle_index(0),

    time(0.0),

    simple_one_particle_switch(0),
    mask_2d_switch(0),
    hardware_cursor_switch(0),
    stop_in_partial_pause_switch(0),
    finish_missile_switch(0),

    emits_groups_switch(0),

    only_emited_by_other_emiter_switch(0),
    draw_particle_emiter(0)
{}

uint32_t PairParticleEmiterClass::group_number = 0;


PairParticleEmiterClass::PairParticleEmiterClass(ParticleFileVersionInfo arg_prt_file_version_info) 
    : m_prt_file_version_info(arg_prt_file_version_info)
{
}

void PairParticleEmiterClass::SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info)
{
    m_prt_file_version_info = arg_prt_file_version_info;
}


void PairParticleEmiterClass::GetFromKsFile(uint8_t* buff, size_t& curr_offset)
{
    ++group_number;

    m_pair_particle_emiter_data.group_name = "G" + to_string(group_number);

    m_pair_particle_emiter_data.emiter_name = "E" + to_string(group_number);

    m_pair_particle_emiter_data.particle_name = "P" + to_string(group_number);


    m_pair_particle_emiter_data.emiter_index = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.particle_index = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_pair_particle_emiter_data.looped_emission.a[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.a[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.b[0] = BinReader::GetBinVal<float>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.a[2] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.draw_particle_emiter = 1;
}


void PairParticleEmiterClass::GetFromPgAndTwAndE2160File(uint8_t* buff, size_t& curr_offset)
{
    m_pair_particle_emiter_data.group_name = BinReader::TakeString(buff, curr_offset);

    m_pair_particle_emiter_data.annotation = BinReader::TakeString(buff, curr_offset);

    m_pair_particle_emiter_data.emiter_name = BinReader::TakeString(buff, curr_offset);
    m_pair_particle_emiter_data.particle_name = BinReader::TakeString(buff, curr_offset);

    m_pair_particle_emiter_data.emiter_index = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.particle_index = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.time = BinReader::GetBinVal<float>(buff, curr_offset);

    m_pair_particle_emiter_data.looped_emission.a[0] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.a[1] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.b[0] = BinReader::GetBinVal<float>(buff, curr_offset);
    m_pair_particle_emiter_data.looped_emission.a[2] = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.simple_one_particle_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.mask_2d_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.hardware_cursor_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.stop_in_partial_pause_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);
    m_pair_particle_emiter_data.finish_missile_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.emits_groups_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);


    uint64_t size_of_emits_groups_data = BinReader::GetBinVal<uint64_t>(buff, curr_offset);

    for (uint64_t i = 0; i < size_of_emits_groups_data; ++i)
    {
        m_pair_particle_emiter_data.emits_groups.push_back(  BinReader::TakeString(buff, curr_offset)   );
    }

    m_pair_particle_emiter_data.only_emited_by_other_emiter_switch = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

    m_pair_particle_emiter_data.draw_particle_emiter = BinReader::GetBinVal<uint32_t>(buff, curr_offset);

}

void PairParticleEmiterClass::GetFromFile(uint8_t* buff, size_t& curr_offset)
{
    switch(m_prt_file_version_info.particle_version)
    {
        case ks_particles_emiter:
            GetFromKsFile(buff, curr_offset);
        break;

        case dynamic_particle:
        break;


        case e2160_particle:
        case particle_gen_particle:
        case two_worlds_particle:
            GetFromPgAndTwAndE2160File(buff, curr_offset);
        break;

        default: break;
    }

}


void PairParticleEmiterClass::WriteAsKsFormat(stringstream& output)
{
    AW::WrtVal("PairParticleEmiter", m_pair_particle_emiter_data.group_name, output, one_quad);

    AW::WrtSinStr("{", output);

    AW::WrtVal("Emiter", m_pair_particle_emiter_data.emiter_name, output);

    AW::WrtVal("EmiterIndex", m_pair_particle_emiter_data.emiter_index, output);

    AW::WrtVal("Particle", m_pair_particle_emiter_data.particle_name, output);

    AW::WrtVal("ParticleIndex", m_pair_particle_emiter_data.particle_index, output);

    AW::WrtVal("time", m_pair_particle_emiter_data.time, output);

    AW::WriteTwoTypeArrayToAod<uint32_t, m_pair_particle_emiter_data.looped_emission_a_size,
                              float, m_pair_particle_emiter_data.looped_emission_b_size>
                              ("loopedEmission",
                              m_pair_particle_emiter_data.looped_emission,
                              output);

    AW::WrtVal("drawParticleEmiter", m_pair_particle_emiter_data.draw_particle_emiter, output);

    AW::WrtSinStr("}", output);
}



void PairParticleEmiterClass::WriteAsPgAndTwAndE2160Format(stringstream& output) 
{
    AW::WrtVal("PairParticleEmiter", m_pair_particle_emiter_data.group_name, output, one_quad);

    AW::WrtSinStr("{", output);
    
    AW::WrtVal("Emiter", m_pair_particle_emiter_data.emiter_name, output);

    AW::WrtVal("EmiterIndex", m_pair_particle_emiter_data.emiter_index, output);

    AW::WrtVal("Particle", m_pair_particle_emiter_data.particle_name, output);

    AW::WrtVal("ParticleIndex", m_pair_particle_emiter_data.particle_index, output);

    AW::WrtVal("annotation", m_pair_particle_emiter_data.annotation, output);

    AW::WrtVal("time", m_pair_particle_emiter_data.time, output);

    AW::WriteTwoTypeArrayToAod<uint32_t, m_pair_particle_emiter_data.looped_emission_a_size, 
                              float, m_pair_particle_emiter_data.looped_emission_b_size>
                              ("loopedEmission",
                              m_pair_particle_emiter_data.looped_emission,
                              output);

    AW::WrtVal("drawParticleEmiter", m_pair_particle_emiter_data.draw_particle_emiter, output);

    AW::WrtVal("simpleOneParticleSwitch", m_pair_particle_emiter_data.simple_one_particle_switch, output);

    AW::WrtVal("2Dmask", m_pair_particle_emiter_data.mask_2d_switch, output);

    AW::WrtVal("hardwareCursor", m_pair_particle_emiter_data.hardware_cursor_switch, output);

    AW::WrtVal("stopInPartialPause", m_pair_particle_emiter_data.stop_in_partial_pause_switch, output);

    AW::WrtVal("finishMissile", m_pair_particle_emiter_data.finish_missile_switch, output);

    AW::WrtVal("emitsGroupsSwitch", m_pair_particle_emiter_data.emits_groups_switch, output);

    AW::WriteVector<string>("emitsGroups", m_pair_particle_emiter_data.emits_groups, output);

    AW::WrtVal("onlyEmitedByOtherEmiterSwitch", m_pair_particle_emiter_data.only_emited_by_other_emiter_switch, output);

    AW::WrtSinStr("}", output);
}

void PairParticleEmiterClass::WriteToFile(stringstream& output)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case ks_particles_emiter:
        WriteAsKsFormat(output);
    break;

    case dynamic_particle:
    break;


    case e2160_particle:
    case particle_gen_particle:
    case two_worlds_particle:
        WriteAsPgAndTwAndE2160Format(output);
    break;

    default: break;
    }

}
