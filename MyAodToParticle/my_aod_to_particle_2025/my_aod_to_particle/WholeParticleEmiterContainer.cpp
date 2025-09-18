#include "my_aod_to_particle.hpp"

void WholeParticleEmiterContainer::WriteToFileBuffer(vector<char>& output_file_buff)
{

	for (auto& single_effect : m_effects)
		single_effect.WriteToFile(output_file_buff);


	BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(m_emiters.size()));

	for (auto& single_emiter : m_emiters)
		single_emiter.WriteToFile(output_file_buff);


	BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(m_particles.size()));

	for (auto& single_particle : m_particles)
		single_particle.WriteToFile(output_file_buff);


	BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(m_pair_particles_emiters.size()));

	for (auto& single_pair_particle_emiter : m_pair_particles_emiters)
		single_pair_particle_emiter.WriteToFile(output_file_buff);

	m_single_particle_end.WriteToFile(output_file_buff);

}
