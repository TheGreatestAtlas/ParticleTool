#include "my_aod_to_particle.hpp"
#include "MshCustomBlockVectors.hpp"


void CustomBlockUtility::SetCustomBlockVersionForNotChild(vector<char>& output_file_buff,
                                                            vector<uint32_t>& custom_block_data,
                                                            old_dynamic_particle_type_value& dst)
{
    switch (dst)
    {
    case e2150_old_dynamic_particle:
        custom_block_data = e2150_custom_block_data;
    break;

    case ww3_of_hh_old_dynamic_particle:
        custom_block_data = ww3_or_hh_custom_block_data;
    break;

    case ww2_or_fa_or_pc2_old_dynamic_particle:
        custom_block_data = ww2_or_fa_or_pc2_custom_block_data;
    break;

    case ks_old_dynamic_particle:
        custom_block_data = ks_custom_block_data;
    break;
    }

}


void CustomBlockUtility::SetCustomBlockVersionForChild(vector<char>& output_file_buff,
                                                         vector<uint32_t>& custom_block_data,
                                                         old_dynamic_particle_type_value& dst)
{
    switch (dst)
    {
    case e2150_old_dynamic_particle:
        custom_block_data = e2150_custom_block_data;
    break;

    case ww3_of_hh_old_dynamic_particle:
        custom_block_data = ww3_or_hh_custom_block_data_for_child;
    break;

    case ww2_or_fa_or_pc2_old_dynamic_particle:
        custom_block_data = ww2_or_fa_or_pc2_custom_block_data_for_child;
    break;

    case ks_old_dynamic_particle:
        custom_block_data = ks_custom_block_data_for_child;
    break;

    }

}
