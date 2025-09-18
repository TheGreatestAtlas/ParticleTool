#include "my_aod_to_particle.hpp"

void AodUtility::MoveByLines(stringstream& arg_stream, int count)
{
    string null_str;

    for (int i = 0; i < count; ++i)
        getline(arg_stream, null_str);
}


void AodUtility::GetSingleString(string& arg_line, string instruction_name, string& dst)
{
    instruction_name += " ";

    const size_t intruction_name_len = instruction_name.length();

    if (arg_line.substr(0, intruction_name_len) == instruction_name)
    {
        dst = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);

        DEBUG_PRINT("STR = ");
        DEBUG_PRINT(dst.c_str());
        DEBUG_PRINT("\n");
    }
}

void AodUtility::GetSingleStringAndInterpretConst(string& arg_line, 
                                                  string instruction_name, 
                                                  uint32_t& dst, 
                                                  unordered_map<string, uint32_t>& m_list_of_consts)
{
    instruction_name += " ";

    const size_t intruction_name_len = instruction_name.length();

    if (arg_line.substr(0, intruction_name_len) == instruction_name)
    {
        string m_help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);

        dst = m_list_of_consts[m_help_str];

        DEBUG_PRINT("STR = ");
        DEBUG_PRINT(m_help_str.c_str());
        DEBUG_PRINT("\n");

        DEBUG_PRINT("Value = ");
        DEBUG_PRINT(to_string(dst).c_str());
        DEBUG_PRINT("\n");
    }
}

void AodUtility::GetSingleStringFromQueue(string& arg_line, string instruction_name, string& dst, queue<string>& arg_q)
{
    instruction_name += " ";

    const size_t intruction_name_len = instruction_name.length();

    if (arg_line.substr(0, intruction_name_len) == instruction_name)
    {
        if(!arg_q.empty())
        {
            dst = arg_q.front();
            arg_q.pop(); 
        }

        DEBUG_PRINT("STR = \"");
        DEBUG_PRINT(dst.c_str());
        DEBUG_PRINT("\"\n");
    }
}


void AodUtility::GetVectorOfStrings(string& arg_line,
    string instruction_name,
    vector<string>& dst)
{
    instruction_name += " ";

    const size_t intruction_name_len = instruction_name.length();

    if (arg_line.substr(0, intruction_name_len) == instruction_name)
    {
        string m_help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);

        m_help_str += ",";

        size_t counter = 0;
        size_t single_value_end_index;
        string single_value_str;

        Utility::RemoveCharFromTheString(m_help_str, '\n');

        for (size_t i = 0; i < m_help_str.size(); ++i)
        {
            if (m_help_str[i] == ',')
                ++counter;
        }

        //cout << "\"" << m_help_str << "\"" << endl << endl;

        for (size_t i = 0; i < counter; ++i)
        {
            single_value_end_index = m_help_str.find(",");

            single_value_str = m_help_str.substr(0, single_value_end_index);

            if (!single_value_str.empty())
            {
                DEBUG_PRINT("STR = \"");
                DEBUG_PRINT(single_value_str.c_str());
                DEBUG_PRINT("\"\n");

                dst.push_back(single_value_str);
            }

            if (single_value_end_index < m_help_str.length() - 1)
            {
                m_help_str = m_help_str.substr(single_value_end_index + 2, m_help_str.length() - single_value_end_index);
            }
        }
    }
}


void AodUtility::GetVectorOfStringsFromQueue(string& arg_line,
                                           string instruction_name,
                                           vector<string>& dst,
                                           queue<string>& arg_q)
{
    instruction_name += " ";

    const size_t intruction_name_len = instruction_name.length();

    if (arg_line.substr(0, intruction_name_len) == instruction_name)
    {
        string m_help_str;

        if (!arg_q.empty())
        {
            m_help_str = arg_q.front();
            arg_q.pop();
        }

        m_help_str += ",";

        size_t counter = 0;
        size_t single_value_end_index;
        string single_value_str;

        Utility::RemoveCharFromTheString(m_help_str, '\n');

        for (size_t i = 0; i < m_help_str.size(); ++i)
        {
            if (m_help_str[i] == ',')
                ++counter;
        }

        //cout << "\"" << m_help_str << "\"" << endl << endl;

        for (size_t i = 0; i < counter; ++i)
        {
            single_value_end_index = m_help_str.find(",");

            single_value_str = m_help_str.substr(0, single_value_end_index);

            if (!single_value_str.empty())
            {
                DEBUG_PRINT("STR = \"");
                DEBUG_PRINT(single_value_str.c_str());
                DEBUG_PRINT("\"\n");

                dst.push_back(single_value_str);
            }

            if (single_value_end_index < m_help_str.length() - 1)
            {
                m_help_str = m_help_str.substr(single_value_end_index + 2, m_help_str.length() - single_value_end_index);
            }
        }
    }
}



void AodUtility::GetObjType(string& arg_line, string& inside_obj_type, string& inside_obj_name, string obj_type_name, queue<string>& arg_q)
{

    if (arg_line.substr(0, obj_type_name.length()) == obj_type_name)
    {
        string obj_type_name_no_space = obj_type_name;
        obj_type_name_no_space.pop_back();

        inside_obj_type = arg_line.substr(0, obj_type_name.length() - 1);
        
        AU::GetSingleStringFromQueue(arg_line, obj_type_name_no_space, inside_obj_name, arg_q);

        DEBUG_PRINT("m_inside_obj_type = |");
        DEBUG_PRINT(inside_obj_type.c_str());
        DEBUG_PRINT("|\n");

        DEBUG_PRINT("m_inside_obj_name = |");
        DEBUG_PRINT(inside_obj_name.c_str());
        DEBUG_PRINT("|\n");
    }


}

void AodUtility::GetTeselateHeader(string& arg_line,
                                   AllTeselatesGroups& teselates_groups,
                                   int64_t& label_index,
                                   size_t& number_of_single_teselate_points_checksum,
                                   unordered_map<string, int64_t>& mapped_labels,
                                   curve_or_teselate_mode& mode)
{
    
    if (arg_line.substr(0, teselate_type_name.length()) == teselate_type_name)
    {
        const int size_of_c_help = 30;
        char c_help[size_of_c_help];

        uint32_t m_single_teselate_flag = 0;
        float m_teselate_scalar = 0.0;
        string m_teselate_string = string();

        const size_t teselate_type_name_len = teselate_type_name.length();
        
        string m_help_str = arg_line.substr(teselate_type_name_len, arg_line.length() - teselate_type_name_len);

        sscanf_s(
                 m_help_str.c_str(), 
                 "%zu %d %f %s", 
                 &number_of_single_teselate_points_checksum,
                 &m_single_teselate_flag,
                 &m_teselate_scalar, 
                 c_help, 
                 _countof(c_help)
                 );

        for (int i = 0; i < _countof(c_help); ++i)
        {
            if (c_help[i] == NULL) { break; }
            m_teselate_string += c_help[i];
        }

        label_index = mapped_labels[m_teselate_string];

        if (label_index >= 0)
        {
            teselates_groups.teselate_flags[label_index] = m_single_teselate_flag;
            teselates_groups.iel_scalars[label_index] = m_teselate_scalar;
        }

        mode = teselate;
    }

   


}


void AodUtility::GetTeselates(string& arg_line,
    AllTeselatesGroups& teselates_groups,
    int64_t& label_index,
    size_t& number_of_single_teselate_points_checksum,
    curve_or_teselate_mode& mode)
{
    if (arg_line.substr(0, teselate_type_name.length()) != teselate_type_name &&
        arg_line != "{" && 
        arg_line != "}" &&
        number_of_single_teselate_points_checksum > 0 &&
        mode == teselate)
    {
        if (arg_line.find(".") != string::npos)
        {
            teselates_groups.teselates[label_index].push_back(stof(arg_line));

            --number_of_single_teselate_points_checksum;
        }
    }

    if (mode == teselate && number_of_single_teselate_points_checksum == 0)
    {
        mode = null;
    }

}


void AodUtility::GetCurveHeader(string& arg_line,
    AllCurvesGroups& curves_groups,
    int64_t& label_index,
    size_t& number_of_single_curve_points_checksum,
    unordered_map<string, int64_t>& mapped_labels,
    curve_or_teselate_mode& mode)
{

    if (arg_line.substr(0, curve_type_name.length()) == curve_type_name)
    {

        DEBUG_PRINT("ITS CURVE HEADER!\n");
        const int size_of_c_help = 30;
        char c_help[size_of_c_help];

        float m_curve_scalar = 0.0;
        string m_curve_string = string();

        const size_t curve_type_name_len = curve_type_name.length();

        string m_help_str = arg_line.substr(curve_type_name_len, arg_line.length() - curve_type_name_len);

        sscanf_s(
            m_help_str.c_str(),
            "%zu %f %s",
            &number_of_single_curve_points_checksum,
            &m_curve_scalar,
            c_help,
            _countof(c_help)
        );

        //printf("number_of_single_curve_points_checksum = %zu\n", number_of_single_curve_points_checksum);

        for (int i = 0; i < _countof(c_help); ++i)
        {
            if (c_help[i] == NULL) { break; }
            m_curve_string += c_help[i];
        }

        label_index = mapped_labels[m_curve_string];

        if (label_index >= 0)
        {
            curves_groups.curve_scalars[label_index] = m_curve_scalar;
            curves_groups.curve_bonus_values[label_index] = example_curve_bonus_value;
        }

        mode = curve;
    }





}


void AodUtility::GetCurves(string& arg_line,
    AllCurvesGroups& curves_groups,
    int64_t& label_index,
    size_t& number_of_single_curve_points_checksum,
    curve_or_teselate_mode& mode)
{
   

    if (arg_line.substr(0, curve_type_name.length()) != curve_type_name &&
        arg_line != "{" &&
        arg_line != "}" &&
        number_of_single_curve_points_checksum > 0 &&
        mode == curve)
    {
        if (arg_line.find(".") != string::npos)
        {
            DEBUG_PRINT("AU::GetCurves\n");
            string curve_instruction = "crv " + arg_line;
            

            DEBUG_PRINT(curve_instruction.c_str());
            DEBUG_PRINT("\n");

            const int temporary_array_size = 5;
            float temporary_array[temporary_array_size] = { 0.0 };

            AU::GetTabValues<float>(curve_instruction, "crv", "%f", temporary_array, temporary_array_size);

            SingleCurvePoint temporary_single_curve_point =
            {
            temporary_array[0],
            temporary_array[1],

                {
                    temporary_array[2],
                    temporary_array[3],
                    temporary_array[4]
                }

            };

            curves_groups.curves[label_index].push_back(temporary_single_curve_point);


            --number_of_single_curve_points_checksum;
        }
    }

    if (mode == curve && number_of_single_curve_points_checksum == 0)
    {
        mode = null;
    }

}