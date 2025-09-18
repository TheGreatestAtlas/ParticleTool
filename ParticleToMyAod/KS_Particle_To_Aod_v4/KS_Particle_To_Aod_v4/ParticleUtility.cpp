#include "KS_Particle_To_Aod.hpp"


void ParticleUtility::ConvertTeselatesAndCurves(ParticleData& particle_data, particle_type_value& src, particle_type_value& dst)
{
    if (src == dst)
    {
        return;
    }

    constexpr size_t tw_and_pg_scale_x_min_index = 6;
    constexpr size_t tw_and_pg_scale_z_min_index = 10;

    constexpr size_t e2160_rotation_z_min_index = 17;
    constexpr size_t e2160_rotation_y_min_index = 21;

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
        for(size_t i = 7 ; i <= 9 ; ++i)
            Utility::EraseTeselateAndCurveGroup(particle_data.curves_groups, particle_data.teselates_groups, i);

        return;
    }

    if (src == e2160_particle
        &&
        dst == two_worlds_particle)
    {
        for (size_t i = 9; i >= 7; --i)
            Utility::InsertTeselateAndCurveGroup(particle_data.curves_groups, particle_data.teselates_groups, i);

        for(size_t i = tw_and_pg_scale_x_min_index; i <= tw_and_pg_scale_z_min_index; i += 2)
            Utility::CopyCurveToDestination(particle_data.curves_groups, i+1, i);

        return;
    }


    if (src == e2160_particle
        &&
        dst == ks_particles_emiter)
    {
        for(size_t i = 18; i <= 20 ; ++i)
            Utility::EraseTeselateAndCurveGroup(particle_data.curves_groups,particle_data.teselates_groups, i);

        for (size_t i = 22; i <= ks_particle_file_type.number_of_iel_in_particle - 1 /*26*/; ++i)
            Utility::InsertTeselateAndCurveGroup(particle_data.curves_groups,
                                                 particle_data.teselates_groups, 
                                                 i,
                                                 e2160_standard_teselate_and_curve_pair);

        return;
    }


    if (src == ks_particles_emiter
        &&
        dst == e2160_particle)
    {
        for (size_t i = ks_particle_file_type.number_of_iel_in_particle - 1 /*26*/; i >= 22; --i)
            Utility::EraseTeselateAndCurveGroup(particle_data.curves_groups,particle_data.teselates_groups, i);

        for (size_t i = 20; i >= 18; --i)
            Utility::InsertTeselateAndCurveGroup(particle_data.curves_groups,
                                                 particle_data.teselates_groups, 
                                                 i,
                                                 e2160_standard_teselate_and_curve_pair);

        for (size_t i = e2160_rotation_z_min_index; i <= e2160_rotation_y_min_index; i += 2)
            Utility::CopyCurveToDestination(particle_data.curves_groups, i + 1, i);

        return;
    }

}

void ParticleUtility::ConvertTeselatesAndCurvesWholeFormat(ParticleData& particle_data, particle_type_value& src, particle_type_value& dst)
{
    particle_type_value copy_of_src = src;
    particle_type_value copy_of_dst;

    if (src > dst)
    {
        while (copy_of_src > dst)
        {
            copy_of_dst = static_cast<particle_type_value>(static_cast<int>(copy_of_src) - 1);

            ParticleUtility::ConvertTeselatesAndCurves(particle_data, copy_of_src, copy_of_dst);

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

            ParticleUtility::ConvertTeselatesAndCurves(particle_data, copy_of_src, copy_of_dst);

            copy_of_src = copy_of_dst;
        }

        return;
    }


}