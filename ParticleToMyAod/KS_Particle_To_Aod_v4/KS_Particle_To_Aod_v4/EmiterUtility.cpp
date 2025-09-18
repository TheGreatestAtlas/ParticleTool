#include "KS_Particle_To_Aod.hpp"

void EmiterUtility::ConvertTeselatesAndCurves(EmiterData& emiter_data, particle_type_value& src, particle_type_value& dst)
{
    if (src == dst)
    {
        return;
    }

    constexpr size_t e2160_rotation_y_min_index = 10; 

    constexpr size_t tw_light_range_min_index = 20;
    constexpr size_t tw_light_intensity_min_index = 28;
    constexpr size_t tw_light_intensity_max_index = 29;

    constexpr size_t knightshift_light_intensity_index = 16;
    constexpr size_t knightshift_budget_index = 14;

    constexpr size_t pg_linked_min_max_surface_scale_x_index = 14;
    constexpr size_t tw_linked_min_max_light_intensity_index = 14;

    constexpr int64_t e2160_linked_min_max_rotation_y_index = 5;


    if (src == two_worlds_particle
        &&
        dst == particle_gen_particle)
    {
        Utility::EraseSetOfTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, 
                                                 tw_light_intensity_min_index, tw_light_intensity_max_index);

        Utility::EraseLinkedMinMax(emiter_data.linked_min_max, tw_linked_min_max_light_intensity_index);

        return;
    }

    if (src == particle_gen_particle
        &&
        dst == two_worlds_particle)
    {
        Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, tw_light_intensity_min_index);
        Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, tw_light_intensity_max_index);

        Utility::InsertLinkedMinMax(emiter_data.linked_min_max, pg_linked_min_max_surface_scale_x_index);

        return;
    }


    if (src == two_worlds_particle
        &&
        dst == e2160_particle)
    {
        Utility::CopyTeselateAndCurvePairToBackup(emiter_data.curves_groups,
                                                  emiter_data.teselates_groups,
                                                  emiter_data.convert_backup,
                                                  tw_light_intensity_min_index);
                                                                  
        for(size_t i = 21; i <= 24; ++i)
            Utility::EraseTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, i);


        Utility::EraseSetOfTeselateAndCurveGroup(emiter_data.curves_groups,
                                                 emiter_data.teselates_groups,
                                                 tw_light_intensity_min_index - 4,
                                                 tw_light_intensity_max_index - 4);

        for (size_t i = 25; i <= 31; ++i)
            Utility::EraseTeselateAndCurveGroup(emiter_data.curves_groups,
                                                emiter_data.teselates_groups, 
                                                i);

        emiter_data.linked_min_max.resize(e2160_particle_edit_file_type.number_of_linked_min_max_in_emiter);

        return;
    }

    if (src == e2160_particle
        &&
        dst == two_worlds_particle)
    {
        for(size_t i = e2160_particle_edit_file_type.number_of_iel_in_emiter /*31*/ ; i >= 24 ; --i)
            Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, i);

        Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups,
                                             emiter_data.teselates_groups,
                                             tw_light_intensity_min_index - 4,
                                             emiter_data.convert_backup);

        for (size_t i = 24; i >= 21; --i)
            Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, i);

        for(size_t i = tw_light_range_min_index; i < two_worlds_particle_edit_file_type.number_of_iel_in_emiter; i +=2 )
            Utility::CopyCurveToDestination(emiter_data.curves_groups, i+1, i);

        emiter_data.linked_min_max.resize(two_worlds_particle_edit_file_type.number_of_linked_min_max_in_emiter, locked_linked_min_max);

        return;
    }


    if (src == e2160_particle
        &&
        dst == ks_particles_emiter)
    {
        for(size_t i = 1 ; i <= 6; ++i)
            Utility::EraseTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, i);

        

        Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, 
                                             emiter_data.teselates_groups, 
                                             knightshift_budget_index,
                                             e2160_standard_teselate_and_curve_pair);

        Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups, 
                                             emiter_data.teselates_groups, 
                                             knightshift_light_intensity_index, 
                                             emiter_data.convert_backup);

        for(int64_t i = e2160_linked_min_max_rotation_y_index; i >= 0 ; --i)
            Utility::EraseLinkedMinMax(emiter_data.linked_min_max, i);

        return;
    }


    if (src == ks_particles_emiter
        &&
        dst == e2160_particle)
    {
        Utility::CopyTeselateAndCurvePairToBackup(emiter_data.curves_groups,
                                                  emiter_data.teselates_groups,
                                                  emiter_data.convert_backup,
                                                  knightshift_light_intensity_index);

        Utility::EraseTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, knightshift_light_intensity_index);

        Utility::EraseTeselateAndCurveGroup(emiter_data.curves_groups, emiter_data.teselates_groups, knightshift_budget_index);

        for (size_t i = 6; i >= 1; --i)
            Utility::InsertTeselateAndCurveGroup(emiter_data.curves_groups,
                                                 emiter_data.teselates_groups, 
                                                 i,
                                                 e2160_standard_teselate_and_curve_pair);
 
        for (size_t i = 0; i <= e2160_rotation_y_min_index; i += 2)
            Utility::CopyCurveToDestination(emiter_data.curves_groups, i + 1, i);

        for(int64_t i = 0 ; i <= e2160_linked_min_max_rotation_y_index; ++i)
            Utility::InsertLinkedMinMax(emiter_data.linked_min_max, i);

        return;
    }


}

void EmiterUtility::ConvertTeselatesAndCurvesWholeFormat(EmiterData& emiter_data, particle_type_value& src, particle_type_value& dst)
{
    particle_type_value copy_of_src = src;
    particle_type_value copy_of_dst;

    if (src > dst)
    {
        while (copy_of_src > dst)
        {
            copy_of_dst = static_cast<particle_type_value>(static_cast<int>(copy_of_src) - 1);


            EmiterUtility::ConvertTeselatesAndCurves(emiter_data,
                copy_of_src,
                copy_of_dst);

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


            EmiterUtility::ConvertTeselatesAndCurves(emiter_data,
                copy_of_src,
                copy_of_dst);

            copy_of_src = copy_of_dst;
        }

        return;
    }


}