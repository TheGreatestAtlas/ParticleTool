#include "KS_Particle_To_Aod.hpp"

string BinReader::GetGuid(uint8_t* buff, size_t& curr_offset)
{
    string my_GUID = string();

    constexpr const int static number_of_uint16_fields = 2;

    my_GUID += "{" + format("{:08X}", GetBinVal<uint32_t>(buff, curr_offset));

    for (int i = 0; i < number_of_uint16_fields; ++i)
        my_GUID += "-" + format("{:04X}", GetBinVal<uint16_t>(buff, curr_offset));

    my_GUID += "-" + format("{:04X}", _byteswap_ushort(GetBinVal<uint16_t>(buff, curr_offset))) + "-";
    my_GUID += format("{:08X}", _byteswap_ulong(GetBinVal<uint32_t>(buff, curr_offset)));
    my_GUID += format("{:04X}", _byteswap_ushort(GetBinVal<uint16_t>(buff, curr_offset))) + "}";

    return my_GUID;
}

string BinReader::TakeString(uint8_t* buff, size_t& curr_offset)
{
    string taked_string = string();
    uint32_t lenght_of_string = GetBinVal<uint32_t>(buff, curr_offset);

    for (uint32_t i = 0; i < lenght_of_string; ++i)
    {
        taked_string += buff[curr_offset];
        ++curr_offset;
    }

    if (taked_string.length() > 0 && taked_string.back() == '\0')
        taked_string.pop_back();

    return taked_string;
}


void BinReader::GetCurves(size_t& number_of_iel,
                          AllCurvesGroups& curves_groups,
                          uint8_t* buff,
                          size_t& curr_offset,
                          bool e2160_switch
)
{

    uint64_t curve_vector_size = 0;
    SingleCurvePoint single_curve_point = {};
    vector<SingleCurvePoint> single_curve_vector = {};
    float curve_scalar = 0.0;
    float bonus_value = 0.0;

    for (size_t i = 0; i < number_of_iel; ++i)
    {
        curve_vector_size = BinReader::GetBinVal<uint64_t>(buff, curr_offset);
        
        //DEBUG_PRINT("CURVE_VECTOR_SIZE = ");
        //DEBUG_PRINT(std::to_string(curve_vector_size).c_str());
        //DEBUG_PRINT("\n");


        for (uint64_t j = 0; j < curve_vector_size; ++j)
        {
            single_curve_point.x = BinReader::GetBinVal<float>(buff, curr_offset);
            single_curve_point.y = BinReader::GetBinVal<float>(buff, curr_offset);
            BinReader::GetBinTab<float>(single_curve_point.bonus_data, single_curve_point.bonus_data_size, buff, curr_offset);

            single_curve_vector.push_back(single_curve_point);
        }

        curves_groups.curves.push_back(std::move(single_curve_vector));
        single_curve_vector.clear();

        curve_scalar = BinReader::GetBinVal<float>(buff, curr_offset);
        curves_groups.curve_scalars.push_back(curve_scalar);

        if (e2160_switch == true)
        {
            bonus_value = BinReader::GetBinVal<float>(buff, curr_offset);
            curves_groups.curve_bonus_values.push_back(bonus_value);
        }

    }

}
