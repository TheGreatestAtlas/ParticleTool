#include "KS_Particle_To_Aod.hpp"
#include "MshCustomBlockVectors.hpp"


void CustomBlockUtility::SetCustomBlockVersion(CustomBlockData& custom_block_data,
                                               old_dynamic_particle_type_value& src,
                                               old_dynamic_particle_type_value& dst,
                                               bool& is_child)
{
    if (src == dst)
    {
        return;
    }
    
    custom_block_data.data.clear();

    if (is_child == false)
    {
        SetCustomBlockVersionForNotChild(custom_block_data, dst);
        return;
    }


    if (is_child == true)
    {
        SetCustomBlockVersionForChild(custom_block_data, dst);
        return;
    }
    
}


void CustomBlockUtility::SetCustomBlockVersionForNotChild(CustomBlockData& custom_block_data,
                                                          old_dynamic_particle_type_value& dst)
{
    switch (dst)
    {
        case e2150_old_dynamic_particle:
            custom_block_data.data = e2150_custom_block_data;
        break;

        case ww3_of_hh_old_dynamic_particle:
            custom_block_data.data = ww3_or_hh_custom_block_data;
        break;

        case ww2_or_fa_or_pc2_old_dynamic_particle:
            custom_block_data.data = ww2_or_fa_or_pc2_custom_block_data;
        break;

        case ks_old_dynamic_particle:
            custom_block_data.data = ks_custom_block_data;
        break;
    }
}


void CustomBlockUtility::SetCustomBlockVersionForChild(CustomBlockData& custom_block_data,
                                                       old_dynamic_particle_type_value& dst)
{
    switch (dst)
    {
    case e2150_old_dynamic_particle:
        custom_block_data.data = e2150_custom_block_data;
    break;

    case ww3_of_hh_old_dynamic_particle:
        custom_block_data.data = ww3_or_hh_custom_block_data_for_child;
    break;

    case ww2_or_fa_or_pc2_old_dynamic_particle:
        custom_block_data.data = ww2_or_fa_or_pc2_custom_block_data_for_child;
    break;

    case ks_old_dynamic_particle:
        custom_block_data.data = ks_custom_block_data_for_child;
    break;

    }

}
