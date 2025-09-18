#include "my_aod_to_particle.hpp"

//-------------------------------
// Metody PairParticleEmiterData
//-------------------------------

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
{
}

PairParticleEmiterClass::PairParticleEmiterClass(ParticleFileVersionInfo arg_prt_file_version_info, 
                                                 string arg_pair_particle_emiter_name) :
     m_prt_file_version_info(arg_prt_file_version_info)
{
    m_pair_particle_emiter_data.group_name = arg_pair_particle_emiter_name;
}


void PairParticleEmiterClass::GetFromFile(string& arg_line)
{
    AU::GetSingleString(arg_line, "Emiter", m_pair_particle_emiter_data.emiter_name);

    AU::GetSingleStringFromQueue(arg_line, "annotation", m_pair_particle_emiter_data.annotation, GlobalQueues::m_annotations_q);

    AU::GetSingleString(arg_line, "Particle", m_pair_particle_emiter_data.particle_name);

    AU::GetOneValue<uint32_t>(arg_line, "EmiterIndex", "%d", m_pair_particle_emiter_data.emiter_index);

    AU::GetOneValue<uint32_t>(arg_line, "ParticleIndex", "%d", m_pair_particle_emiter_data.particle_index);

    AU::GetOneValue<float>(arg_line, "time", "%f", m_pair_particle_emiter_data.time);

    AU::GetOneValue<uint32_t>(arg_line, "simpleOneParticleSwitch", "%d", m_pair_particle_emiter_data.simple_one_particle_switch);

    AU::GetOneValue<uint32_t>(arg_line, "2Dmask", "%d", m_pair_particle_emiter_data.mask_2d_switch);

    AU::GetOneValue<uint32_t>(arg_line, "hardwareCursor", "%d", m_pair_particle_emiter_data.hardware_cursor_switch);

    AU::GetOneValue<uint32_t>(arg_line, "stopInPartialPause", "%d", m_pair_particle_emiter_data.stop_in_partial_pause_switch);

    AU::GetOneValue<uint32_t>(arg_line, "finishMissile", "%d", m_pair_particle_emiter_data.finish_missile_switch);

    AU::GetOneValue<uint32_t>(arg_line, "emitsGroupsSwitch", "%d", m_pair_particle_emiter_data.emits_groups_switch);

    AU::GetOneValue<uint32_t>(arg_line, 
                              "onlyEmitedByOtherEmiterSwitch", 
                              "%d", 
                              m_pair_particle_emiter_data.only_emited_by_other_emiter_switch);

    AU::GetOneValue<uint32_t>(arg_line, "drawParticleEmiter", "%d", m_pair_particle_emiter_data.draw_particle_emiter);

    AU::GetVectorOfStringsFromQueue(arg_line, "emitsGroups", m_pair_particle_emiter_data.emits_groups, GlobalQueues::m_emits_groups_q);


    AU::GetTwoTypeTabValues<uint32_t, 
                            m_pair_particle_emiter_data.looped_emission_a_size, 
                            float, 
                            m_pair_particle_emiter_data.looped_emission_b_size>
                            (arg_line, 
                            "loopedEmission", 
                            "%d", 
                            "%f", 
                            m_pair_particle_emiter_data.looped_emission);

}


void PairParticleEmiterClass::WriteToFile(vector<char>& output_file_buff)
{
    switch (m_prt_file_version_info.particle_version)
    {
    case ks_particles_emiter:
        WriteToKsFormat(output_file_buff);
    break;

    case dynamic_particle:
    break;

    case e2160_particle:
    case particle_gen_particle:
    case two_worlds_particle:
        WriteToPgAndTwAndE2160Format(output_file_buff);
    break;

    default: break;
    }
}

void PairParticleEmiterClass::WriteToPgAndTwAndE2160Format(vector<char>& output_file_buff)
{
    BW::WriteString(output_file_buff, m_pair_particle_emiter_data.group_name);

    BW::WriteString(output_file_buff, m_pair_particle_emiter_data.annotation);

    BW::WriteString(output_file_buff, m_pair_particle_emiter_data.emiter_name);

    BW::WriteString(output_file_buff, m_pair_particle_emiter_data.particle_name);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.emiter_index);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.particle_index);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.time);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[0]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[1]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.b[0]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[2]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.simple_one_particle_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.mask_2d_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.hardware_cursor_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.stop_in_partial_pause_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.finish_missile_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.emits_groups_switch);

    BW::WriteVectorOfStrings(output_file_buff, m_pair_particle_emiter_data.emits_groups);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.only_emited_by_other_emiter_switch);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.draw_particle_emiter);

}



void PairParticleEmiterClass::WriteToKsFormat(vector<char>& output_file_buff)
{

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.emiter_index);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.particle_index);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.time);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[0]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[1]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.b[0]);

    BW::WriteValue(output_file_buff, m_pair_particle_emiter_data.looped_emission.a[2]);

}