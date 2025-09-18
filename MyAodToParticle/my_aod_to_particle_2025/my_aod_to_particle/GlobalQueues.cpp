#include "my_aod_to_particle.hpp"

queue<string> GlobalQueues::m_effect_names_q = {};
queue<string> GlobalQueues::m_emiter_names_q = {};
queue<string> GlobalQueues::m_particle_names_q = {};
queue<string> GlobalQueues::m_pair_particle_emiter_names_q = {};

queue<string> GlobalQueues::m_annotations_q = {};

queue<string> GlobalQueues::m_slot_strings_q = {};
queue<string> GlobalQueues::m_texture_q = {};
queue<string> GlobalQueues::m_mesh_q = {};
queue<string> GlobalQueues::m_bump_texture_q = {};
queue<string> GlobalQueues::m_emits_groups_q = {};

//Only For Dynamic Particle:
queue<string> GlobalQueues::m_dynamic_particle_Texture_q = {};
queue<string> GlobalQueues::m_dynamic_particle_Mesh_q = {};