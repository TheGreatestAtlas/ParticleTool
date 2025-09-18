#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <iomanip>
#include <Windows.h>

using namespace std;

#ifdef _DEBUG
    #define DEBUG_PRINT(x) printf_s("%s",x)
#else
    #define DEBUG_PRINT(x)
#endif

const string my_aod_format = ".myaod";

constexpr size_t size_of_e2150_root_data_block = 0x364;                 // 868 (dec)

constexpr size_t size_of_ww3_or_heli_heroes_root_data_block = 0xAAC;    // 2732 (dec)

constexpr size_t size_of_ww2_or_fa_or_pc2_root_data_block = 0xCA4;      // 3236 (dec)

constexpr size_t size_of_ks_root_data_block = 0xFB4;                    // 4020 (dec)

const string effect_type_name = "Effect ";
const string emiter_type_name = "Emiter ";
const string particle_type_name = "Particle ";
const string pair_particle_type_name = "PairParticleEmiter ";

const string teselate_type_name = "teselate ";
const string curve_type_name = "curve ";


constexpr float example_curve_bonus_value = 0.0;

constexpr uint32_t locked_linked_min_max = 1;
constexpr uint32_t unlocked_linked_min_max = 0;

constexpr int64_t initialize_signal_value = -1;
constexpr int64_t push_back_signal_value = 0;

constexpr int first_type_of_two_type_array = 0;
constexpr int second_type_of_two_type_array = 1;

constexpr int dec_base = 10;


const char e2150_format_str[] = "e2150";
const char ww3_or_hh_format_str[] = "ww3_or_hh";
const char ww2_or_fa_or_pc2_format_str[] = "ww2_or_fa_or_pc2";
const char ks_format_str[] = "ks";


struct SingleCurvePoint
{
    float x = 0;
    float y = 0;

    constexpr const unsigned int static bonus_data_size = 3;
    float bonus_data[bonus_data_size] = { 0.0 };
};

struct AllCurvesGroups
{
    vector <vector<SingleCurvePoint> > curves = { };
    vector< float > curve_scalars = {};
    vector< float > curve_bonus_values = {};

    void resize(size_t size = 0)
    {
        curves.resize(size);
        curve_scalars.resize(size);
        curve_bonus_values.resize(size);
    }

};

struct AllTeselatesGroups
{
    vector< vector<float> > teselates = {};
    vector<float> iel_scalars = {};
    vector<uint32_t> teselate_flags = {};

    void resize(size_t size = 0)
    {
        teselates.resize(size);
        iel_scalars.resize(size);
        teselate_flags.resize(size);
    }
};

enum particle_type_value
{
    not_particle,
    dynamic_particle,
    ks_particles_emiter,
    e2160_particle,
    two_worlds_particle,
    particle_gen_particle
};

enum old_dynamic_particle_type_value
{
    not_old_dynamic_particle,
    e2150_old_dynamic_particle,
    ww3_of_hh_old_dynamic_particle,
    ww2_or_fa_or_pc2_old_dynamic_particle,
    ks_old_dynamic_particle
};

enum curve_or_teselate_mode
{
    null,
    curve,
    teselate
};


//---------------------------------
// Struct ParticleFileVersionInfo
//---------------------------------

struct ParticleFileVersionInfo
{
    particle_type_value particle_version = not_particle;
    size_t custom_data_block_size = 0;

    size_t number_of_iel_in_effect = 0;
    size_t number_of_iel_in_emiter = 0;
    size_t number_of_iel_in_particle = 0;

    uint32_t number_of_linked_min_max_in_effect = 0;
    uint32_t number_of_linked_min_max_in_emiter = 0;
    uint32_t number_of_linked_min_max_in_particle = 0;

    int size_of_end_bonus_data = 0;

    old_dynamic_particle_type_value dynamic_particle_version = not_old_dynamic_particle;
};


const ParticleFileVersionInfo earth2150_dynamic_file_type =
{
    dynamic_particle,  //particle_version
    size_of_e2150_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

    0, // number_of_iel_in_effect
    0, // number_of_iel_in_emiter
    0, // number_of_iel_in_particle

    0, // number_of_linked_min_max_in_effect
    0, // number_of_linked_min_max_in_emiter
    0, // number_of_linked_min_max_in_particle

    0, // size_of_end_bonus_data
    e2150_old_dynamic_particle
};


const ParticleFileVersionInfo ww3_or_heli_heroes_dynamic_file_type =
{
    dynamic_particle,  //particle_version
    size_of_ww3_or_heli_heroes_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

    0, // number_of_iel_in_effect
    0, // number_of_iel_in_emiter
    0, // number_of_iel_in_particle

    0, // number_of_linked_min_max_in_effect
    0, // number_of_linked_min_max_in_emiter
    0, // number_of_linked_min_max_in_particle

    0, // size_of_end_bonus_data
    ww3_of_hh_old_dynamic_particle
};

const ParticleFileVersionInfo ww2_or_fa_or_pc2_dynamic_file_type =
{
    dynamic_particle,  //particle_version
    size_of_ww2_or_fa_or_pc2_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

    0, // number_of_iel_in_effect
    0, // number_of_iel_in_emiter
    0, // number_of_iel_in_particle

    0, // number_of_linked_min_max_in_effect
    0, // number_of_linked_min_max_in_emiter
    0, // number_of_linked_min_max_in_particle

    0, // size_of_end_bonus_data
    ww2_or_fa_or_pc2_old_dynamic_particle
};


const ParticleFileVersionInfo ks_dynamic_file_type =
{
    dynamic_particle,  //particle_version
    size_of_ks_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

    0, // number_of_iel_in_effect
    0, // number_of_iel_in_emiter
    0, // number_of_iel_in_particle

    0, // number_of_linked_min_max_in_effect
    0, // number_of_linked_min_max_in_emiter
    0, // number_of_linked_min_max_in_particle

    0, // size_of_end_bonus_data
    ks_old_dynamic_particle
};


const ParticleFileVersionInfo ks_particle_file_type =
{
    ks_particles_emiter,  //particle_version
    size_of_ks_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

    0, // number_of_iel_in_effect
    27, // number_of_iel_in_emiter
    27, // number_of_iel_in_particle

    0, // number_of_linked_min_max_in_effect
    4, // number_of_linked_min_max_in_emiter
    8, // number_of_linked_min_max_in_particle

    0, // size_of_end_bonus_data
    not_old_dynamic_particle
};


const ParticleFileVersionInfo particle_gen_file_type =
{
    particle_gen_particle,
    0,

    12,
    42,
    28,

    12 / 2,
    42 / 2,
    28 / 2,

    16,
    not_old_dynamic_particle
};

const ParticleFileVersionInfo two_worlds_particle_edit_file_type =
{
    two_worlds_particle,
    0,

    12,
    44,
    28,

    12 / 2,
    44 / 2,
    28 / 2,

    16,
    not_old_dynamic_particle
};

const ParticleFileVersionInfo e2160_particle_edit_file_type =
{
    e2160_particle, //particle_version
    0, //custom_data_block_size

    0, //number_of_iel_in_effect
    31, //number_of_iel_in_emiter
    25, //number_of_iel_in_particle

    0, //number_of_linked_min_max_in_effect
    10, //number_of_linked_min_max_in_emiter
    11, //number_of_linked_min_max_in_particle

    18, //size_of_end_bonus_data
    not_old_dynamic_particle
};

template <typename A, int A_SIZE, typename B, int B_SIZE>
struct TwoTypeArray
{
    A a[A_SIZE] = {};
    B b[B_SIZE] = {};
};

namespace Utility
{
    void RemoveCharFromTheString(string& str, char symbol);

    static size_t CountOccurrences(const uint8_t* buffer, size_t& size, const string& word);

    queue<string> FindLineStrings(const char* buffer, size_t bufferSize, const string pattern);

    void InitializeHashMapViaLabelsVector(unordered_map<string, int64_t>& m_labels_map,
        vector<string>& labels);

    void ReplaceStringInFileBuffer(string& file_buffer, const string searchStr, const string replaceStr);


    inline bool CheckAnswerWithEveryElementInVec(string answer, const vector<string> elements)
    {
        bool result = false;

        for (const auto& element : elements)
        {
            result |= (answer == element);
        }

        return result;
    }

}


//Aod Utility:
namespace AodUtility
{

    void MoveByLines(stringstream& arg_stream, int count);

    template<typename T>
    void GetOneValue(string& arg_line, string instruction_name, const char* format, T& dst)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);
            sscanf_s(m_help_str.c_str(), format, &dst);

            DEBUG_PRINT("VALUE = ");
            DEBUG_PRINT(to_string(dst).c_str());
            DEBUG_PRINT("\n");
        }
    }

    void GetSingleString(string& arg_line, string instruction_name, string& dst);

    void GetSingleStringAndInterpretConst(string& arg_line,
        string instruction_name,
        uint32_t& dst,
        unordered_map<string, uint32_t>& m_list_of_consts);

    void GetSingleStringFromQueue(string& arg_line,
        string instruction_name,
        string& dst,
        queue<string>& arg_q);
    

    void GetObjType(string& arg_line, string& inside_obj_type, string& inside_obj_name, string obj_type_name, queue<string>& arg_q);

    void GetVectorOfStrings(string& arg_line,
        string instruction_name,
        vector<string>& dst);

    void GetVectorOfStringsFromQueue(string& arg_line,
        string instruction_name,
        vector<string>& dst,
        queue<string>& arg_q);

    template<typename T>
    void GetTabValues(string& arg_line,
        string instruction_name,
        string format,
        T dst[],
        int size_of_dst)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len,
                arg_line.length() - intruction_name_len);

            m_help_str += ",";

            size_t single_value_end_index;
            string single_value_str;

            Utility::RemoveCharFromTheString(m_help_str, ' ');
            Utility::RemoveCharFromTheString(m_help_str, '\n');

            //cout << "\"" << m_help_str << "\"" << endl;

            for (size_t i = 0; i < size_of_dst; ++i)
            {
                single_value_end_index = m_help_str.find(",");

                single_value_str = m_help_str.substr(0, single_value_end_index);

                if (!single_value_str.empty())
                {
                    if (format == "%d")
                    {
                        dst[i] = static_cast<T>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format == "%u")
                    {
                        dst[i] = static_cast<T>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format == "%f")
                    {
                        dst[i] = static_cast<T>(stof(single_value_str, nullptr));
                    }

                    //if (format == "%lf")
                    //{
                    //    dst[i] = static_cast<T>(stod(single_value_str, nullptr));
                    //}

                    //if (format == "%Lf")
                    //{
                    //    dst[i] = static_cast<T>(stold(single_value_str, nullptr));
                    //}
                }

                m_help_str = m_help_str.substr(single_value_end_index + 1, m_help_str.length() - single_value_end_index);

            }
        }
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void GetTwoTypeTabValues(string& arg_line,
        string instruction_name,
        string format_a,
        string format_b,
        TwoTypeArray<A, A_SIZE, B, B_SIZE>& dst)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len,
                arg_line.length() - intruction_name_len);

            m_help_str += ",";

            size_t single_value_end_index;
            string single_value_str;

            Utility::RemoveCharFromTheString(m_help_str, ' ');
            Utility::RemoveCharFromTheString(m_help_str, '\n');

            //cout << "\"" << m_help_str << "\"" << endl;

            const int number_of_types = 2;
            size_t size_of_array = A_SIZE;
            for (int j = first_type_of_two_type_array; j < number_of_types; ++j)
            {
                for (size_t i = 0; i < size_of_array; ++i)
                {
                    single_value_end_index = m_help_str.find(",");

                    single_value_str = m_help_str.substr(0, single_value_end_index);

                    if ((j == first_type_of_two_type_array) && (size_of_array == A_SIZE) && (!single_value_str.empty()))
                    {
                        if (format_a == "%d")
                        {
                            dst.a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                        }

                        if (format_a == "%u")
                        {
                            dst.a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                        }

                        if (format_a == "%f")
                        {
                            dst.a[i] = static_cast<A>(stof(single_value_str, nullptr));
                        }
                    }

                    if ((j == second_type_of_two_type_array) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                    {
                        if (format_b == "%d")
                        {
                            dst.b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                        }

                        if (format_b == "%u")
                        {
                            dst.b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                        }

                        if (format_b == "%f")
                        {
                            dst.b[i] = static_cast<B>(stof(single_value_str, nullptr));
                        }
                    }

                    m_help_str = m_help_str.substr(single_value_end_index + 1, m_help_str.length() - single_value_end_index);

                }

                size_of_array = B_SIZE;

            }


        }
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void GetTwoTypeTabValuesViaScheme(string& arg_line,
        string instruction_name,
        string format_a,
        string format_b,
        string scheme,
        TwoTypeArray<A, A_SIZE, B, B_SIZE>& dst)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len,
                arg_line.length() - intruction_name_len);

            m_help_str += ",";

            size_t single_value_end_index;
            string single_value_str;

            Utility::RemoveCharFromTheString(m_help_str, ' ');
            Utility::RemoveCharFromTheString(m_help_str, '\n');

            Utility::RemoveCharFromTheString(scheme, ' ');
            Utility::RemoveCharFromTheString(scheme, ',');

            //cout << "\"" << m_help_str << "\"" << endl;

            size_t size_of_array = 0;

            for (size_t i = 0; i < m_help_str.size(); ++i)
            {
                if (m_help_str[i] == ',')
                    ++size_of_array;
            }

            size_t j = 0;
            size_t k = 0;

            for (size_t i = 0; i < size_of_array; ++i)
            {
                single_value_end_index = m_help_str.find(",");

                single_value_str = m_help_str.substr(0, single_value_end_index);

                if ((!single_value_str.empty()) && (scheme[i] == 'a'))
                {
                    if (format_a == "%d")
                    {
                        dst.a[j] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%u")
                    {
                        dst.a[j] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%f")
                    {
                        dst.a[j] = static_cast<A>(stof(single_value_str, nullptr));
                    }
                    ++j;
                }

                if ((!single_value_str.empty()) && (scheme[i] == 'b'))
                {
                    if (format_b == "%d")
                    {
                        dst.b[k] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_b == "%u")
                    {
                        dst.b[k] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_b == "%f")
                    {
                        dst.b[k] = static_cast<B>(stof(single_value_str, nullptr));
                    }
                    ++k;
                }

                m_help_str = m_help_str.substr(single_value_end_index + 1, m_help_str.length() - single_value_end_index);

            }

        }
    }



    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void GetArrayOfTwoTypeTabValues(string& arg_line,
        string instruction_name,
        string format_a,
        string format_b,
        TwoTypeArray<A, A_SIZE, B, B_SIZE> dst[],
        int size)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len,
                arg_line.length() - intruction_name_len);

            m_help_str += ",";

            size_t single_value_end_index;
            string single_value_str;

            Utility::RemoveCharFromTheString(m_help_str, ' ');
            Utility::RemoveCharFromTheString(m_help_str, '\n');

            //cout << "\"" << m_help_str << "\"" << endl;

            for (int k = 0; k < size; ++k)
            {

                const int number_of_types = 2;
                size_t size_of_array = A_SIZE;
                for (int j = first_type_of_two_type_array; j < number_of_types; ++j)
                {
                    for (size_t i = 0; i < size_of_array; ++i)
                    {
                        single_value_end_index = m_help_str.find(",");

                        single_value_str = m_help_str.substr(0, single_value_end_index);

                        if ((j == first_type_of_two_type_array) && (size_of_array == A_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_a == "%d")
                            {
                                dst[k].a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%u")
                            {
                                dst[k].a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%f")
                            {
                                dst[k].a[i] = static_cast<A>(stof(single_value_str, nullptr));
                            }
                        }

                        if ((j == second_type_of_two_type_array) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_b == "%d")
                            {
                                dst[k].b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%u")
                            {
                                dst[k].b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%f")
                            {
                                dst[k].b[i] = static_cast<B>(stof(single_value_str, nullptr));
                            }
                        }

                        m_help_str = m_help_str.substr(single_value_end_index + 1, m_help_str.length() - single_value_end_index);

                    }

                    size_of_array = B_SIZE;

                }
            }

        }
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void GetArrayOfTwoTypeTabValues_PlusBonus(string& arg_line,
                                              string instruction_name,
                                              string format_a,
                                              string format_b,
                                              TwoTypeArray<A, A_SIZE, B, B_SIZE> dst[],
                                              int size,
                                              A& single_bonus_value)
    {
        instruction_name += " ";

        const size_t intruction_name_len = instruction_name.length();

        if (arg_line.substr(0, intruction_name_len) == instruction_name)
        {
            string m_help_str = arg_line.substr(intruction_name_len,
                arg_line.length() - intruction_name_len);

            m_help_str += ",";

            size_t single_value_end_index;
            string single_value_str;

            Utility::RemoveCharFromTheString(m_help_str, ' ');
            Utility::RemoveCharFromTheString(m_help_str, '\n');

            //cout << "\"" << m_help_str << "\"" << endl;

            size_t counter = 0;
            for (size_t i = 0; i < m_help_str.size(); ++i)
            {
                if (m_help_str[i] == ',')
                    ++counter;
            }

            for (int k = 0; k < size; ++k)
            {

                const int number_of_types = 2;
                size_t size_of_array = A_SIZE;
                for (int j = first_type_of_two_type_array; j < number_of_types; ++j)
                {
                    for (size_t i = 0; i < size_of_array; ++i)
                    {
                        single_value_end_index = m_help_str.find(",");

                        single_value_str = m_help_str.substr(0, single_value_end_index);

                        if ((j == first_type_of_two_type_array) && (size_of_array == A_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_a == "%d")
                            {
                                dst[k].a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%u")
                            {
                                dst[k].a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%f")
                            {
                                dst[k].a[i] = static_cast<A>(stof(single_value_str, nullptr));
                            }
                        }

                        if ((j == second_type_of_two_type_array) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_b == "%d")
                            {
                                dst[k].b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%u")
                            {
                                dst[k].b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%f")
                            {
                                dst[k].b[i] = static_cast<B>(stof(single_value_str, nullptr));
                            }
                        }

                        m_help_str = m_help_str.substr(single_value_end_index + 1, m_help_str.length() - single_value_end_index);

                    }

                    size_of_array = B_SIZE;

                }
            }

            if (counter > static_cast<size_t>((A_SIZE + B_SIZE) * size) )
            {
                single_value_end_index = m_help_str.find(",");

                single_value_str = m_help_str.substr(0, single_value_end_index);

                if (!single_value_str.empty())
                {
                    if (format_a == "%d")
                    {
                        single_bonus_value = static_cast<A>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%u")
                    {
                        single_bonus_value = static_cast<A>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%f")
                    {
                        single_bonus_value = static_cast<A>(stof(single_value_str, nullptr));
                    }
                }

            }

        }
    }



    void GetTeselateHeader(string& arg_line,
        AllTeselatesGroups& teselates_groups,
        int64_t& label_index,
        size_t& number_of_single_teselate_points_checksum,
        unordered_map<string, int64_t>& mapped_labels,
        curve_or_teselate_mode& mode);


    void GetTeselates(string& arg_line,
        AllTeselatesGroups& teselates_groups,
        int64_t& label_index,
        size_t& number_of_single_teselate_points_checksum,
        curve_or_teselate_mode& mode);



    void GetCurveHeader(string& arg_line,
        AllCurvesGroups& curves_groups,
        int64_t& label_index,
        size_t& number_of_single_curve_points_checksum,
        unordered_map<string, int64_t>& mapped_labels,
        curve_or_teselate_mode& mode);


    void GetCurves(string& arg_line,
        AllCurvesGroups& curves_groups,
        int64_t& label_index,
        size_t& number_of_single_curve_points_checksum,
        curve_or_teselate_mode& mode);
    

}

namespace AU = AodUtility;


namespace BinWriter
{
    template<typename T>
    void WriteValue(vector<char>& buff, const T& val)
    {
        const char* ptr = reinterpret_cast<const char*>(&val);
        buff.insert(buff.end(), ptr, ptr + sizeof(T));
    }

    template<typename T>
    void WriteTabFromTo(vector<char>& buff, const T tab[], const int from, const int to)
    {
        for (int i = from; i <= to; ++i)
            WriteValue(buff, tab[i]);
    }


    template<typename T>
    void WriteTabFromToReverse(vector<char>& buff, const T tab[], const int from, const int to)
    {
        for (int i = from; i >= to; --i)
            WriteValue(buff, tab[i]);
    }


    template<typename T>
    void WriteTabViaIndexes(vector<char>& buff, const T tab[], const std::vector<int> indexes)
    {
        for(int index : indexes)
        {
            WriteValue(buff, tab[index]);
        }
    }


    void WriteCurves(vector<char>& output_file_buff,
                     AllCurvesGroups& curves_groups,
                     bool e2160_switch = false);

    template<typename T>
    void WriteVector(vector<char>& buff, vector<T>& values)
    {
        for (const auto& single_val : values)
        {
            WriteValue(buff, single_val);
        }
    }



    template<typename T>
    void WriteTeselates(vector<char>& buff,
                        AllTeselatesGroups& teselates_groups,
                        bool two_worlds_switch = false)
    {
        size_t i = 0;

        for (auto& teselate : teselates_groups.teselates)
        {
            T number_of_points = static_cast<T>(teselate.size());
            WriteValue(buff, number_of_points);

            WriteVector(buff, teselate);

            if (two_worlds_switch == true)
            {
                bool single_teselate_flag = static_cast<bool>(teselates_groups.teselate_flags[i]);
                WriteValue(buff, single_teselate_flag);
            }

            WriteValue(buff, teselates_groups.iel_scalars[i]);
            
            ++i;
        }

    }

    void WriteString(vector<char>& output_file_buff, string& str, bool null_terminator_switch = false);
    

    void WriteVectorOfStrings(vector<char>& buff, vector<string>& strings);


    void WriteFirstStringFromVector(vector<char>& buff,
                                    vector<string>& strings,
                                    bool null_terminator_switch = false);

    void GenerateAndWriteGuid(vector<char>& buff);

}

namespace BW = BinWriter;


//-----------------
// Wazne struktury:
//-----------------

//-------------
// Effect Data
//-------------

struct EffectData
{
    EffectData();

    uint32_t particle_2D_switch; //ok
    uint32_t width; //ok
    uint32_t height; //ok

    //only for e2160:
    uint32_t e2160_effect_scale; //ok

    float start_time; //ok
    float time; // ok
    uint32_t loops; //ok

    constexpr const unsigned int static scale_a_size = 3; //ok
    constexpr const unsigned int static scale_b_size = 1; //ok
    constexpr const unsigned int static scales_size = 2; //ok
    TwoTypeArray<float, scale_a_size, uint32_t, scale_b_size> scales[scales_size] = {}; //ok

    string annotation; //ok 

    //-------------------------------
    uint32_t unknown_value_1; //ok ??
    uint32_t unknown_value_2; //ok ??
    //-------------------------------

    AllCurvesGroups curves_groups = {}; //ok
    AllTeselatesGroups teselates_groups = {}; //ok

    std::vector<uint32_t> linked_min_max = {}; //ok

    uint32_t all_teselate_zero_mask; // ok
};

//-----------------------
// Effect Implementation
//-----------------------

class EffectClass
{
public:
    EffectClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                string arg_effect_name = string() );

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

private:
    void InitializeHashMap();

    void WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff);
    void WriteToE2160Format(vector<char>& output_file_buff);

    curve_or_teselate_mode m_mode;
    
    int64_t m_curve_iel_index;
    size_t m_number_of_single_curve_points_checksum;

    int64_t m_teselate_iel_index;
    size_t m_number_of_single_teselate_points_checksum;

    unordered_map<string, int64_t> m_labels_map = {};

    EffectData m_effect_data = {};
    
    ParticleFileVersionInfo m_prt_file_version_info = {};
};



//---------------
// Emiter Data
//---------------

struct EmiterData
{
    EmiterData();

    string emiter_name; //ok
    float start_time; //ok
    float time; //ok

    uint32_t coordinates; //ok
    uint32_t budget_type; //ok

    vector<string> slot_strings = {}; //ok

    constexpr const unsigned int static surface_size = 3; //ok
    uint32_t surface[surface_size] = { 0 }; //ok
    uint32_t sun_ray_direction_switch; //ok


    constexpr const unsigned int static sphere_size = 5; //ok
    float sphere[sphere_size] = { 0.0 }; //ok

    // cubic a, a, a, b
    constexpr const unsigned int static cubic_a_size = 3; //ok
    constexpr const unsigned int static cubic_b_size = 1; //ok
    TwoTypeArray<float, cubic_a_size, uint32_t, cubic_b_size> cubic = {}; //ok

    // plane a, a, b
    constexpr const unsigned int static plane_a_size = 2; //ok
    constexpr const unsigned int static plane_b_size = 1; //ok
    TwoTypeArray<float, plane_a_size, uint32_t, plane_b_size> plane = {}; //ok
    uint32_t use_color_switch; //ok

    string texture; //ok

    uint32_t create_on_ground; //ok
    uint32_t trajectory_z_from_ground; //ok

    constexpr const unsigned int static rotation_size = 3; //ok
    uint32_t rotation[rotation_size] = { 0 }; //ok


    uint32_t enable_light; //ok
    float umbra_distance_mul; //ok


    // only for two worlds:
    //------------------------
    uint32_t cast_shadows; //ok
    //------------------------

    constexpr const unsigned int static scales_a_size = 14; //ok 
    constexpr const unsigned int static scales_b_size = 2; //ok
    TwoTypeArray<float, scales_a_size, uint32_t, scales_b_size> scales = {}; //ok

    uint32_t move_end_of_laser_switch; //ok
    uint32_t attach_to_end_of_laser_switch; //ok

    constexpr const unsigned int static random_seed_size = 2; //ok
    uint32_t random_seed[random_seed_size] = { 0 }; //ok

    uint32_t unknown_value_1; //ok
    uint32_t unknown_value_2; //ok
    uint32_t unknown_value_3; //ok
    uint32_t unknown_value_4; //ok
    uint32_t unknown_value_5; //ok

    AllCurvesGroups curves_groups = {}; //ok
    AllTeselatesGroups teselates_groups = {}; //ok

    vector <uint32_t> linked_min_max = {}; //ok

    uint32_t all_teselate_zero_mask; //ok
};

//-----------------------
// Emiter Implementation
//-----------------------

class EmiterClass
{
public:
    EmiterClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                string arg_emiter_name = string());

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

    

private:
    void InitializeHashMap();

    void WriteToKsFormat(vector<char>& output_file_buff);
    void WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff);
    void WriteToE2160Format(vector<char>& output_file_buff);

    curve_or_teselate_mode m_mode;

    int64_t m_curve_iel_index;
    size_t m_number_of_single_curve_points_checksum;

    int64_t m_teselate_iel_index;
    size_t m_number_of_single_teselate_points_checksum;

    unordered_map<string, int64_t> m_labels_map = {};

    EmiterData m_emiter_data = {};
    
    ParticleFileVersionInfo m_prt_file_version_info = {};

    uint32_t m_write_cast_shadows_to_bin;
};


//---------------
// Particle Data
//---------------

struct ParticleData
{
    ParticleData();

    std::string particle_name; //ok

    float start_time; //ok
    float time; //ok

    uint32_t coordinates; //ok
    uint32_t type; //ok

    std::string mesh; //ok

    // face a, b, b, b, b, b 
    constexpr const unsigned int static face_a_size = 1; //ok
    constexpr const unsigned int static face_b_size = 5; //ok
    TwoTypeArray<uint32_t, face_a_size, float, face_b_size> face = {}; //ok

    float triangle_a; // ------ only for e2160 //ok
    float screen_depth_offset; //ok

    uint32_t multiply_dxy_by_prt_scale_switch; //ok

    // trail a, b
    constexpr const unsigned int static trail_a_size = 1; //ok
    constexpr const unsigned int static trail_b_size = 1; //ok
    TwoTypeArray<float, trail_a_size, uint32_t, trail_b_size> trail = {}; //ok

    float y_texture_mul; //ok

    vector<string> textures = {}; //ok

    vector<string> bump_textures = {}; //ok

    // texAnim a, a, a, a, a, a, a, a, a, b, a
    constexpr const unsigned int static tex_anim_a_size = 10; //ok
    constexpr const unsigned int static tex_anim_b_size = 1; //ok
    TwoTypeArray<uint32_t, tex_anim_a_size, float, tex_anim_b_size> tex_anim = {}; //ok


    constexpr const unsigned int static coordinates_ltrb_size = 4; //ok
    float coordinates_ltrb[coordinates_ltrb_size] = { 0.0 }; //ok

    uint32_t distortion_particle_switch; //ok

    uint32_t x_mirrored; //ok
    uint32_t y_mirrored; //ok

    uint32_t color_rand_type; //ok

    uint32_t unknown_value_1; //ok
    uint32_t unknown_value_2; //ok
    uint32_t unknown_value_3; //ok

    constexpr const unsigned int static trajectory_size = 2; //ok
    uint32_t trajectory[trajectory_size] = { 0 }; //ok

    // gravity a, a, a, a, a, a, a, b 
    constexpr const unsigned int static gravity_a_size = 7; //ok
    constexpr const unsigned int static gravity_b_size = 1; //ok
    TwoTypeArray<float, gravity_a_size, uint32_t, gravity_b_size> gravity = {}; //ok

    uint32_t enable_light; // ----- only for ks //ok

    uint32_t gravity_rot_from_trajectory_z_switch; //ok
    uint32_t gravity_rot_from_trajectory_x_switch; //ok
    uint32_t gravity_rot_from_trajectory_y_switch; //ok

    uint32_t dissappears_on_ground; //ok

    uint32_t create_on_ground_switch; //ok
    uint32_t trajectory_z_from_ground; //ok


    constexpr const unsigned int static scale_a_size = 3; //ok
    constexpr const unsigned int static scale_b_size = 1; //ok
    constexpr const unsigned int static scales_size = 3; //ok
    TwoTypeArray<float, scale_a_size, uint32_t, scale_b_size> scales[scales_size] = {}; //ok

    float scales_end; // ----------- only for ks  //ok


    AllCurvesGroups curves_groups = {}; //ok
    AllTeselatesGroups teselates_groups = {}; //ok

    std::vector<uint32_t> linked_min_max = {}; //ok

    uint32_t all_teselate_zero_mask; //ok

    uint32_t layer; //ok

};

//-------------------------
// Particle Implementation
//-------------------------

class ParticleClass
{
public:
    ParticleClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                  string arg_particle_name = string());

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

    
private:
    void InitializeHashMap();

    void WriteToParticleGenAndTwoWorldsFormat(vector<char>& output_file_buff);
    void WriteToE2160Format(vector<char>& output_file_buff);
    void WriteToKsFormat(vector<char>& output_file_buff);

    curve_or_teselate_mode m_mode;

    int64_t m_curve_iel_index;
    size_t m_number_of_single_curve_points_checksum;

    int64_t m_teselate_iel_index;
    size_t m_number_of_single_teselate_points_checksum;

    unordered_map<string, int64_t> m_labels_map = {};
    
    ParticleData m_particle_data = {};

    ParticleFileVersionInfo m_prt_file_version_info = {};
};



//--------------------------
//  Pair Particle Emiter Data
//--------------------------

struct PairParticleEmiterData
{
    PairParticleEmiterData();

    string group_name; //ok

    string annotation; //ok

    string emiter_name; //ok
    string particle_name; //ok

    uint32_t emiter_index; //ok
    uint32_t particle_index; //ok

    float time; //ok

    // loopedemission a, a, a, b 
    constexpr const unsigned int static looped_emission_a_size = 3; //ok
    constexpr const unsigned int static looped_emission_b_size = 1; //ok
    TwoTypeArray<uint32_t, looped_emission_a_size, float, looped_emission_b_size> looped_emission = {}; //ok

    uint32_t simple_one_particle_switch; //ok
    uint32_t mask_2d_switch; //ok
    uint32_t hardware_cursor_switch; //ok
    uint32_t stop_in_partial_pause_switch; //ok
    uint32_t finish_missile_switch; //ok

    uint32_t emits_groups_switch; //ok

    vector<string> emits_groups = {}; //ok

    uint32_t only_emited_by_other_emiter_switch; //ok
    uint32_t draw_particle_emiter; //ok

};

//-------------------------------------
// Pair Particle Emiter Implementation
//-------------------------------------

class PairParticleEmiterClass
{
public:
    PairParticleEmiterClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                            string arg_pair_particle_emiter_name = string());

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

    

private:
    void WriteToPgAndTwAndE2160Format(vector<char>& output_file_buff);
    void WriteToKsFormat(vector<char>& output_file_buff);

    PairParticleEmiterData m_pair_particle_emiter_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};



//------------------
// ParticleEndData
//------------------

struct ParticleEndData
{
    ParticleEndData();

    constexpr const unsigned int static end_particle_values_size = 18;
    float end_particle_values[end_particle_values_size] = { 0.0 };
    uint32_t gamerate;
};

//------------------
// ParticleEndImpl
//------------------

class ParticleEndClass
{
public:
    ParticleEndClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo());

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

    

private:
    void WriteToPgAndTwFormat(vector<char>& output_file_buff) const;
    void WriteToE2160Format(vector<char>& output_file_buff) const;
    void WriteToKsFormat(vector<char>& output_file_buff) const;


    ParticleEndData m_particle_end_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};


namespace CustomBlockUtility
{
    void SetCustomBlockVersionForNotChild(vector<char>& output_file_buff,
                                          vector<uint32_t>& custom_block_data,
                                          old_dynamic_particle_type_value& dst);

    void  SetCustomBlockVersionForChild(vector<char>& output_file_buff,
                                        vector<uint32_t>& custom_block_data,
                                        old_dynamic_particle_type_value& dst);

}

namespace CBU = CustomBlockUtility;


class IntroductionHeaderClass
{
public:
    IntroductionHeaderClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                            string arg_extra_data_file_name = string(),
                            ostream& arg_o = cout);

    void GetAndWriteToFile(vector<char>& output_file_buff, bool is_child = false);

   
private:
    void WriteToDynamicParticleFormat(vector<char>& output_file_buff, bool is_child = false);
    
    void WriteToKsParticleEmiterFormat(vector<char>& output_file_buff, bool is_child = false);
    
    void WriteToPgAndTwFormat(vector<char>& output_file_buff) const;

    ostream& m_o;

    string m_extra_data_file_name;

    ParticleFileVersionInfo m_prt_file_version_info = {};
};



//---------------------------
// Dynamic Particle Structure
//---------------------------

struct DynamicParticleData
{
    DynamicParticleData();

    // --------------
    string m_obj_name;
    // --------------

    uint32_t type;
    uint32_t light_type;

    constexpr const unsigned int static frames_size = 5;
    int32_t frames[frames_size] = { 0 };

    constexpr const unsigned int static time_size = 2;
    float time[time_size] = { 0.0 }; //wczesniej int64_t

    constexpr const unsigned int static frames_ratio_size = 2;
    float frames_ratio[frames_ratio_size] = { 0.0 };

    constexpr const unsigned int static size_size = 5;
    float size[size_size] = { 0.0 };

    constexpr const unsigned int static size_2_size = 4;
    float size_2[size_2_size] = { 0.0 };

    float delta_size;

    uint32_t null_space;

    uint32_t additive;

    constexpr const unsigned int static light_size = 4;
    float light[light_size] = { 0.0, 0.0, 0.0, 1.0 };

    constexpr const unsigned int static color_size = 4;
    float color[color_size] = { 0.0 };

    constexpr const unsigned int static alpha_a_size = 2;
    constexpr const unsigned int static alpha_b_size = 1;
    TwoTypeArray<float, alpha_a_size, uint32_t, alpha_b_size> alpha = {};

    constexpr const unsigned int static scale_size = 2;
    float scale[scale_size] = { 0.0 };

    constexpr const unsigned int static position_size = 3;
    float position[position_size] = { 0.0 };

    constexpr const unsigned int static position_2_size = 3;
    float position_2[position_2_size] = { 0.0 };

    string mesh;
    string texture;

    uint32_t number_of_nested_particles;

    vector<DynamicParticleData> children;

    uint32_t layer;

};


class DynamicParticleClass
{
public:
    DynamicParticleClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                         string arg_input_directory_name = string(),
                         vector<char> arg_clean_file_buffer = {});

    void GetFromFile(string& arg_line);
    void WriteToFile(vector<char>& output_file_buff);

private:

    void GetWholeObject(DynamicParticleData& arg_dynamic_particle_data, string& arg_line);
    //{
    void GetSingleObject(DynamicParticleData& arg_dynamic_particle_data, string& arg_line);
    void GetTime(DynamicParticleData& arg_dynamic_particle_data, string& arg_line);
    //}

    void WriteDynamicParticleDataToFileBuffer(vector<char>& output_file_buff,
                                              DynamicParticleData& arg_dynamic_particle_data);
    //{
    void CalculateImportantValues(DynamicParticleData& arg_dynamic_particle_data);
    //}

    void InitializeHashMaps();

    DynamicParticleData m_dynamic_particle_data = {};

    ParticleFileVersionInfo m_prt_file_version_info = {};

    unordered_map<string, uint32_t> m_list_of_type_consts = {};
    unordered_map<string, uint32_t> m_list_of_lighttype_consts = {};

    string m_input_directory_name;

    stringstream m_dynamic_particle_file_stream;
};




//----------------------------------
// Whole dynamic particle container
//----------------------------------

class WholeDynamicParticleContainer
{
public:
    vector<DynamicParticleClass> m_dynamic_particles = {};
    DynamicParticleClass m_single_dynamic_particle;

    void WriteToFileBuffer(vector<char>& output_file_buff);
};


//----------------------------------
// Whole particle emiter container
//----------------------------------

class WholeParticleEmiterContainer
{
public:

    vector<EffectClass> m_effects = {};
    EffectClass m_single_effect;

    vector<EmiterClass> m_emiters = {};
    EmiterClass m_single_emiter;

    vector<ParticleClass> m_particles = {};
    ParticleClass m_single_particle;

    vector<PairParticleEmiterClass> m_pair_particles_emiters = {};
    PairParticleEmiterClass m_single_pair_particle_emiter;

    ParticleEndClass m_single_particle_end;

    void WriteToFileBuffer(vector<char>& output_file_buff);

};





//--------------------
// Class InputBinFile 
//--------------------

class InputBinFile
{
public:
    //Pola:
    vector<char> m_file_buffer = {};

    InputBinFile(string argfilename, ostream& arg_o = cout, string argmode = "text");
    ~InputBinFile();

    void Informations() const;
    void ExitWhenFileDontExist() const;

    //Gettery:
    bool GetIsOpened() const;
    bool GetIsOpenNow() const;

    streamsize GetFileLength() const;
    string GetFileName() const;

private:
    //Pola:
    ostream& m_input_file_o;

    fstream m_input_file;
    string m_file_name;

    streamsize m_file_length;

    bool m_is_opened;
    bool m_is_open_now;

    string m_work_mode;

    //Metody:
    void OpenFile();
    void MeasureFile();
    void PutFileToBuffer();

};

//----------------
// Cpp Compilator
//----------------

class CppCompilator
{
public:
    // metody:

    CppCompilator(vector <char>& arg_file_buffer);
    void CompileCppFileToBin(vector <char>& arg_output_file_buff);

    ParticleFileVersionInfo GetDynamicParticleVersion() const;

private:
    //Pola:
    bool m_its_string;
    bool m_its_comment;

    vector <char> m_file_buffer;

    uint64_t m_file_length;

    string m_instruction;
    string m_value_type;
    string m_name;
    string m_value;

    vector <uint8_t> m_clean_file = {};

    ParticleFileVersionInfo m_dynamic_particle_file_version_info = {};
    unordered_map<string, ParticleFileVersionInfo> m_dynamic_particle_file_version_info_hash_map = {};

    //Metody:
    void InitializeHashMaps();


    void DeleteComments();
    void DeleteContrCharAndPutToVector();

    void WriteGUID(vector<char>& output_file_buff, string& slowo);

    template <typename T>
    void WriteValueToFile(vector<char>& output_file_buff, string& value, uint8_t mode)
    {
        T typevalue;

        switch (mode)
        {
        case 'u':
            typevalue = static_cast<T>(stoul(value));
        break;

        case 'd':
            typevalue = static_cast<T>(stol(value));
        break;

        default: break;
        }

        BW::WriteValue<T>(output_file_buff, typevalue);
    }

    template <typename T>
    void WriteTabToFile(vector<char>& output_file_buff, string& str, uint8_t mode)
    {
        T writedvalue = 0;

        if (m_value == "{}") return;

        string scanedvalue = string();
        for (uint64_t i = 0; i < str.length(); ++i)
        {
            if (str[i] == '{') continue;


            if ((str[i] == ',') || (str[i] == '}'))
            {

                switch (mode)
                {

                case 'd': writedvalue = static_cast<T>(stol(scanedvalue));  break;
                case 'u': writedvalue = static_cast<T>(stoul(scanedvalue)); break;
                default: break;
                }

                BW::WriteValue<T>(output_file_buff, writedvalue);

                scanedvalue = string();
                continue;
            }

            scanedvalue += str[i];
        }
    }

    void GetValuesFromString();
    void WriteInstructionToBin(vector<char>& output_file_buff, 
                               string& valuetype, 
                               string& name, 
                               string& value);

    uint64_t CountValuesInTab(string& value);

};






//-------------------
// Class AodCompiler
//-------------------


class GlobalQueues
{
public:
    static queue<string> m_effect_names_q;
    static queue<string> m_emiter_names_q;
    static queue<string> m_particle_names_q;
    static queue<string> m_pair_particle_emiter_names_q;

    static queue<string> m_annotations_q;

    static queue<string> m_slot_strings_q;
    static queue<string> m_texture_q;
    static queue<string> m_mesh_q;
    static queue<string> m_bump_texture_q;
    static queue<string> m_emits_groups_q;

    //Only For Dynamic Particle:
    static queue<string> m_dynamic_particle_Texture_q;
    static queue<string> m_dynamic_particle_Mesh_q;
};

class AodCompilator
{
public:
	// metody:
	AodCompilator(string arg_input_directory_name = string(), ostream& arg_o = cout);

	void CompileTxtFileToBinBuffer();

	static void SaveOutputFileBufferToBinFile(string& output_file_name);

    static vector<char> s_output_file_buffer;

    particle_type_value GetParticleVersion() const;

private:

    WholeParticleEmiterContainer m_whole_particle_emiter_container = {};
    WholeDynamicParticleContainer m_whole_dynamic_particle_container = {};

    string m_input_directory_name;
    string m_extra_data_file_name;

    string m_compilator_flags_cfg_file_path;

	//Pola:
    bool its_pair_particle_emiter;

    bool m_end_of_file_initialize_switch;

    bool m_is_version_getted;
    int64_t bracket_counter;

    string m_obj_name;
    string m_inside_obj_type;
    string m_inside_obj_name;

    vector <char> m_file_buffer = {};
    vector <char> m_clean_file = {};

	string m_instruction;

    particle_type_value m_particle_type_version;
    ParticleFileVersionInfo m_particle_file_version_info;

    unordered_map<particle_type_value, ParticleFileVersionInfo> m_list_of_prt_file_ver_info = {};


    ostream& m_o;

	//Metody:
    void CleanFileBuffer(vector<char>& arg_clean_file, bool change_newline_switch = true);


    bool InterpretInstructions(string& m_instruction);
    //Dla dynamic_particle zwraca true
    //Dla innych false
    

    void InitializeFormatHashMaps();

    void GetPrtVersion(string& arg_line,
                       particle_type_value& dst,
                       ParticleFileVersionInfo& arg_particle_file_version_info);

    void GetDynamicParticleVersion(string& arg_line,
                                   particle_type_value& dst,
                                   ParticleFileVersionInfo& arg_particle_file_version_info);

    
    void InterpretParticlesEmiterInstructions(string& instruction);

    void CompileDynamicParticle();



    template<class T>
    void CompileObject(string& instruction, T& single_object, vector<T>& objects_vec)
    {
        if (instruction == "{" && bracket_counter == initialize_signal_value)
        {
            ++bracket_counter;
            DEBUG_PRINT("Initialize...\n");
            single_object = T(m_particle_file_version_info, m_inside_obj_name);
        }

        if (instruction == "{")
        {
            ++bracket_counter;
        }

        if (bracket_counter > 0)
        {
            single_object.GetFromFile(instruction);
        }

        if (instruction == "}")
        {
            --bracket_counter;
        }

        if (instruction == "}" && bracket_counter == push_back_signal_value)
        {
            DEBUG_PRINT("PushBack\n");
            objects_vec.push_back(single_object);
            single_object = T(m_particle_file_version_info);
            bracket_counter = initialize_signal_value;

            m_inside_obj_type = string();
            m_inside_obj_name = string();
        }

    }

    void CompileEndOfFile(string& instruction, ParticleEndClass& single_particle_end);

};

//--------------------------
// Class ParticleCompilerApp
//--------------------------

class ParticleCompilerApp
{

public:
    //Metody:
    ParticleCompilerApp(istream& argin, ostream& argo, int my_argc, char* my_argv[]);

private:
    //Pola:
    constexpr const unsigned int static c_correct_number_of_args = 2;

    istream& m_in_stream;
    ostream& m_o;

    string m_input_directory_name;

    //Metody:
    void ShowStartScreen();
    void StandardProgramExecution();

    string CreateOutputFileName(const particle_type_value& version) const;

};