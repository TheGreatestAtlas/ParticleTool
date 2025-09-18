#include "my_aod_to_particle.hpp"

void WholeDynamicParticleContainer::WriteToFileBuffer(vector<char>& output_file_buff)
{
	for (auto& single_dynamic_particle : m_dynamic_particles)
		single_dynamic_particle.WriteToFile(output_file_buff);
}

