#include "KS_Particle_To_Aod.hpp"


void EffectUtility::ConvertTeselatesAndCurves(EffectData& effect_data, particle_type_value& src, particle_type_value& dst)
{
    if (src == dst)
    {
        return;
    }

    if (src == two_worlds_particle
        &&
        dst == particle_gen_particle)
    {
        return;
    }

    if (src == particle_gen_particle
        &&
        dst == two_worlds_particle)
    {
        return;
    }

    if (src == two_worlds_particle
        &&
        dst == e2160_particle)
    {
        effect_data.linked_min_max.resize(e2160_particle_edit_file_type.number_of_linked_min_max_in_effect, locked_linked_min_max);

        effect_data.curves_groups.curves.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);
        effect_data.curves_groups.curve_scalars.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);
        effect_data.curves_groups.curve_bonus_values.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);

        effect_data.teselates_groups.teselates.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);
        effect_data.teselates_groups.iel_scalars.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);
        effect_data.teselates_groups.teselate_flags.resize(e2160_particle_edit_file_type.number_of_iel_in_effect);

        return;
    }

    if (src == e2160_particle
        &&
        dst == two_worlds_particle)
    {
        for (size_t i = 0; i < two_worlds_particle_edit_file_type.number_of_iel_in_effect; ++i)
            Utility::InsertTeselateAndCurveGroup(effect_data.curves_groups,
                                                 effect_data.teselates_groups,
                                                 i,
                                                 e2160_standard_teselate_and_curve_pair);

        effect_data.linked_min_max.resize(two_worlds_particle_edit_file_type.number_of_linked_min_max_in_effect, locked_linked_min_max);

        return;
    }


    if (src == e2160_particle
        &&
        dst == ks_particles_emiter)
    {
        return;
    }


    if (src == ks_particles_emiter
        &&
        dst == e2160_particle)
    {
        return;
    }

}

void EffectUtility::ConvertTeselatesAndCurvesWholeFormat(EffectData& effect_data, particle_type_value& src, particle_type_value& dst)
{
    particle_type_value copy_of_src = src;
    particle_type_value copy_of_dst;

    if (src > dst)
    {
        while (copy_of_src > dst)
        {
            copy_of_dst = static_cast<particle_type_value>(static_cast<int>(copy_of_src) - 1);

            EffectUtility::ConvertTeselatesAndCurves(effect_data, copy_of_src, copy_of_dst);

            copy_of_src = copy_of_dst;
        }

        return;
    }

    if (src == dst)
    {
        return;
    }

    if (src < dst)
    {
        while (copy_of_src < dst)
        {
            copy_of_dst = static_cast<particle_type_value>(static_cast<int>(copy_of_src) + 1);

            EffectUtility::ConvertTeselatesAndCurves(effect_data, copy_of_src, copy_of_dst);

            copy_of_src = copy_of_dst;
        }

        return;
    }


}