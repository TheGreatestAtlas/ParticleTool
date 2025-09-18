#include "KS_Particle_To_Aod.hpp"

void Utility::InsertLinkedMinMax(vector<uint32_t>& linked_min_max, 
                                 size_t index, 
                                 uint32_t value)
{
    linked_min_max.insert(linked_min_max.begin() + index, value);
}

void Utility::EraseLinkedMinMax(vector<uint32_t>& linked_min_max, size_t index)
{
    linked_min_max.erase(linked_min_max.begin() + index);
}

void Utility::CopyCurveToDestination(AllCurvesGroups& curves_groups, 
                                     size_t destination, 
                                     size_t source)
{
    curves_groups.curves[destination] = curves_groups.curves[source];
    curves_groups.curve_bonus_values[destination] = curves_groups.curve_bonus_values[source];
    curves_groups.curve_scalars[destination] = curves_groups.curve_scalars[source];
}


void Utility::CopyTeselateAndCurvePairToBackup(AllCurvesGroups& curves_groups,
                                               AllTeselatesGroups& teselates_groups,
                                               TeselateAndCurvePair& backup,
                                               size_t index)
{
    backup.curve = curves_groups.curves[index];
    backup.curve_bonus_value = curves_groups.curve_bonus_values[index];
    backup.curve_scalar = curves_groups.curve_scalars[index];
    backup.iel_scalar = teselates_groups.iel_scalars[index];
    backup.teselate = teselates_groups.teselates[index];
    backup.teselate_flag = teselates_groups.teselate_flags[index];
}

void Utility::InsertTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
                                          AllTeselatesGroups& teselates_groups,
                                          size_t index,
                                          TeselateAndCurvePair arg_tes_and_curve)
{
    curves_groups.curves.insert(curves_groups.curves.begin() + index, arg_tes_and_curve.curve);
    curves_groups.curve_scalars.insert(curves_groups.curve_scalars.begin() + index, arg_tes_and_curve.curve_scalar);
    curves_groups.curve_bonus_values.insert(curves_groups.curve_bonus_values.begin() + index, arg_tes_and_curve.curve_bonus_value);

    teselates_groups.teselates.insert(teselates_groups.teselates.begin() + index, arg_tes_and_curve.teselate);
    teselates_groups.iel_scalars.insert(teselates_groups.iel_scalars.begin() + index, arg_tes_and_curve.iel_scalar);
    teselates_groups.teselate_flags.insert(teselates_groups.teselate_flags.begin() + index, arg_tes_and_curve.teselate_flag);

}

void Utility::EraseTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
                                         AllTeselatesGroups& teselates_groups,
                                         size_t index)
{
    curves_groups.curves.erase(curves_groups.curves.begin() + index);

    curves_groups.curve_scalars.erase(curves_groups.curve_scalars.begin() + index);

    curves_groups.curve_bonus_values.erase(curves_groups.curve_bonus_values.begin() + index);

    teselates_groups.teselates.erase(teselates_groups.teselates.begin() + index);

    teselates_groups.iel_scalars.erase(teselates_groups.iel_scalars.begin() + index);

    teselates_groups.teselate_flags.erase(teselates_groups.teselate_flags.begin() + index);

}


void Utility::EraseSetOfTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
                                              AllTeselatesGroups& teselates_groups,
                                              size_t index_from, 
                                              size_t index_to)
{
    curves_groups.curves.erase(curves_groups.curves.begin() + index_from,
        curves_groups.curves.begin() + index_to + 1);

    curves_groups.curve_scalars.erase(curves_groups.curve_scalars.begin() + index_from,
        curves_groups.curve_scalars.begin() + index_to + 1);

    curves_groups.curve_bonus_values.erase(curves_groups.curve_bonus_values.begin() + index_from,
        curves_groups.curve_bonus_values.begin() + index_to + 1);


    teselates_groups.teselates.erase(teselates_groups.teselates.begin() + index_from,
        teselates_groups.teselates.begin() + index_to + 1);

    teselates_groups.iel_scalars.erase(teselates_groups.iel_scalars.begin() + index_from,
        teselates_groups.iel_scalars.begin() + index_to + 1);

    teselates_groups.teselate_flags.erase(teselates_groups.teselate_flags.begin() + index_from,
        teselates_groups.teselate_flags.begin() + index_to + 1);
}


void Utility::MakeCurves(AllTeselatesGroups& teselates_groups,
                         AllCurvesGroups& curves_groups, 
                         const SingleCurvePoint& template_curve_point)
{
    constexpr long double domain_begin = 0.0;
    constexpr long double domain_end = 1.0;

    SingleCurvePoint single_curve_point = template_curve_point;

    

    vector<SingleCurvePoint> single_curve_vector = {};

    for (size_t j = 0; j < teselates_groups.teselates.size(); ++j)
    {
        const auto& m_line = teselates_groups.teselates[j];

        if (m_line.empty() == true)
        {
            single_curve_point = template_curve_point;
            single_curve_vector.push_back(single_curve_point);

            single_curve_point.x = domain_end;
            single_curve_vector.push_back(single_curve_point);

            curves_groups.curves.push_back(single_curve_vector);
            curves_groups.curve_scalars.push_back(e2160_standard_curve_scalar);
            curves_groups.curve_bonus_values.push_back(e2160_standard_bonus_value);

            single_curve_vector.clear();
            single_curve_point = template_curve_point;

            continue;
        }

        long double sampleRate = sqrt(m_line.size());

        if ( (static_cast<size_t>(sampleRate) != 0) && (m_line.size() % static_cast<size_t>(sampleRate) == 0) )
        {
            sampleRate = sqrt(m_line.size() + 1.0);
        }

        //ratio:
        long double sampleRatio = m_line.size() / sampleRate;
        long double h = (domain_end - domain_begin) / sampleRate;

        for (long double i = 0; i < sampleRate; ++i)
        {
            auto&& calc = i * sampleRatio;
            auto&& arg_x = i * h;

            if (calc >= m_line.size())
                throw;

            if (arg_x >= domain_end)
                throw;

            if (i == domain_begin)
            {
                single_curve_point.x = domain_begin;
                single_curve_point.y = m_line[0];
            }

            if (sampleRate < i + 1.0)
            {
                single_curve_point.x = domain_end;
                single_curve_point.y = m_line[m_line.size() - 1];
            }

            if ((!(i == domain_begin)) && (!(sampleRate < i + 1.0)))
            {
                single_curve_point.x = static_cast<float>(arg_x);
                single_curve_point.y = m_line[ static_cast<const unsigned __int64>(calc) ];
            }

            single_curve_vector.push_back(single_curve_point);
        }

        curves_groups.curves.push_back(move(single_curve_vector));
        single_curve_vector.clear();

        curves_groups.curve_scalars.push_back(ks_example_curve_scalar);
        
        curves_groups.curve_bonus_values.push_back(e2160_standard_bonus_value);


    }

}


string Utility::GenerateGuidString()
{
    GUID guid;
    string guid_str = string();

    if (CoCreateGuid(&guid) == S_OK) 
    {
        stringstream oss;
        oss << uppercase << hex << setfill('0')
            << setw(8) << guid.Data1 << "-"
            << setw(4) << guid.Data2 << "-"
            << setw(4) << guid.Data3 << "-"
            << setw(2) << static_cast<int>(guid.Data4[0])
            << setw(2) << static_cast<int>(guid.Data4[1]) << "-"
            << setw(2) << static_cast<int>(guid.Data4[2])
            << setw(2) << static_cast<int>(guid.Data4[3])
            << setw(2) << static_cast<int>(guid.Data4[4])
            << setw(2) << static_cast<int>(guid.Data4[5])
            << setw(2) << static_cast<int>(guid.Data4[6])
            << setw(2) << static_cast<int>(guid.Data4[7]);

        guid_str = "{" + oss.str() + "}";
    }
    else
    {
        throw;
    }

    return guid_str;
}