#include <filesystem>
#include <fstream>
#include <vector>
#include <format>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <Windows.h>

using namespace std;

#ifdef _DEBUG
    #define DEBUG_PRINT(x) printf_s("%s",x)
#else
    #define DEBUG_PRINT(x)
#endif

namespace fs = filesystem;

constexpr uint32_t locked_linked_min_max = 1;
constexpr uint32_t unlocked_linked_min_max = 0;

constexpr float e2160_standard_curve_scalar = 3.000000;
constexpr float e2160_standard_bonus_value = 0.0;
constexpr float ks_example_curve_scalar = 1.000000;

constexpr int one_quad = 1;
constexpr int two_quads = 2;
constexpr int three_quads = 3;

constexpr size_t size_of_e2150_root_data_block = 0x364;                 // 868 (dec)

constexpr size_t size_of_ww3_or_heli_heroes_root_data_block = 0xAAC;    // 2732 (dec)

constexpr size_t size_of_ww2_or_fa_or_pc2_root_data_block = 0xCA4;      // 3236 (dec)

constexpr size_t size_of_ks_root_data_block = 0xFB4;                    // 4020 (dec)

const char e2150_format_str[] = "e2150";
const char ww3_or_hh_format_str[] = "ww3_or_hh";
const char ww2_or_fa_or_pc2_format_str[] = "ww2_or_fa_or_pc2";
const char ks_format_str[] = "ks";

const char earth2160_format_str[] = "e2160";
const char tw_format_str[] = "tw";
const char pg_format_str[] = "pg";

const char my_aod_format[] = ".myaod";
const char bonus_file_rest_name[] = "_extra_data.cpp";
const char dynamic_particle_cfg_file_name[] = "DynamicParticle.cfg";

constexpr uint8_t tw_old_format_fourth_byte_of_header = 1;
constexpr uint8_t tw_new_format_fourth_byte_of_header = 2;

constexpr streamsize aod_float_precision = 17;

inline void Margin(stringstream& output, int ile)
{
    for (int i = 0; i < ile; ++i)
        output << '\t';
}

namespace Misc
{
    inline bool CheckAnswerWithEveryElementInVec(string answer, const vector<string> elements)
    {
        bool result = false;

        for (const auto& element : elements)
        {
            result |= (answer == element);
        }

        return result;
    }

    inline void RemoveCharFromTheString(string& str, char symbol)
    {
        for (size_t i = 0; i < str.length(); )
        {
            if (str[i] == symbol)
            {
                str.erase(i, 1);
            }
            else
            {
                ++i;
            }
        }
    }

}


struct SingleCurvePoint
{
    float x = 0;
    float y = 0;

    constexpr const unsigned int static bonus_data_size = 3;
    float bonus_data[bonus_data_size] = { 0.0 };
};

constexpr SingleCurvePoint e2160_example_curve_point = { 0.0, 1.0, {-1.0, -1.0, 0.0} };
constexpr SingleCurvePoint e2160_example_curve_point_beg = { 0.0, 1.0, {-1.0, -1.0, 0.0} };
constexpr SingleCurvePoint e2160_example_curve_point_end = { 1.0, 1.0, {-1.0, -1.0, 0.0} };

constexpr SingleCurvePoint tw_and_pg_example_curve_point = { 0.0, 0.0, { 3.0, 3.0, 0.0} };
constexpr SingleCurvePoint tw_and_pg_example_curve_point_beg = { 0.0, 0.0, { 3.0, 3.0, 0.0} };
constexpr SingleCurvePoint tw_and_pg_example_curve_point_end = { 1.0, 0.0, { 3.0, 3.0, 0.0} };


struct TeselateAndCurvePair
{
    vector<SingleCurvePoint> curve = { tw_and_pg_example_curve_point_beg, 
                                       tw_and_pg_example_curve_point_end };
    float curve_scalar = 0.0;
    float curve_bonus_value = 0.0;

    vector<float> teselate = {};
    float iel_scalar = 0.0;
    uint32_t teselate_flag = 0;
    
};

const TeselateAndCurvePair e2160_standard_teselate_and_curve_pair =
{
    {e2160_example_curve_point_beg, e2160_example_curve_point_end},
    e2160_standard_curve_scalar,
    e2160_standard_bonus_value,

    {},
    0.0,
    0
};


struct AllCurvesGroups
{
    vector <vector<SingleCurvePoint> > curves = { };
    vector< float > curve_scalars = {};
    vector< float > curve_bonus_values = {};
};

struct AllTeselatesGroups
{
    vector< vector<float> > teselates = {};
    vector<float> iel_scalars = {};
    vector<uint32_t> teselate_flags = {};
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
    ::size_of_e2150_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

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
    ::size_of_ww3_or_heli_heroes_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

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
    ::size_of_ww2_or_fa_or_pc2_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

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
    ::size_of_ks_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

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
    ::size_of_ks_root_data_block / sizeof(uint32_t) ,  //custom_data_block_size

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
    
    12/2,
    42/2,
    28/2,

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


//---------------
// Binary Reader
//---------------

namespace BinReader
{
    template<typename T>
    T GetBinVal(uint8_t* buff, size_t& curr_offset)
    {

        T value = reinterpret_cast<T&>(buff[curr_offset]);

        curr_offset += sizeof(T);

        return value;
    }

    template<typename T>
    void GetBinTabViaIndexes(T arr[], const vector<int> indexes, uint8_t* buff, size_t& curr_offset)
    {
        for (int index : indexes)
        {
            arr[index] = GetBinVal<T>(buff, curr_offset);
        }
    }


    template<typename T>
    void GetBinTab(T arr[], int arr_size, uint8_t* buff, size_t& curr_offset)
    {
        for (int i = 0; i < arr_size; ++i)
            arr[i] = GetBinVal<T>(buff, curr_offset);
    }


    template<typename T>
    void GetBinTabFromTo(T arr[], int from, int to, uint8_t* buff, size_t& curr_offset)
    {
        for (int i = from; i <= to; ++i)
            arr[i] = GetBinVal<T>(buff, curr_offset);
    }


    template<typename T>
    void GetBinTabReverse(T arr[], int arr_size, uint8_t* buff, size_t& curr_offset)
    {
        for (int i = arr_size - 1; i >= 0; --i)
            arr[i] = GetBinVal<T>(buff, curr_offset);
    }


    string GetGuid(uint8_t* buff, size_t& curr_offset);
    string TakeString(uint8_t* buff, size_t& curr_offset);

    template<typename T>
    void GetTeselates(size_t& number_of_iel,
                      AllTeselatesGroups& teselates_groups,
                      uint8_t* buff,
                      size_t& curr_offset,
                      bool two_worlds_switch = false
                      )
    {
        T teselate_vector_size = 0;
        vector<float> teselate_vector = {};
        float teselate = 0.0;
        uint32_t teselate_flag = 0;
        float iel_scalar = 0.0;

        for (size_t i = 0; i < number_of_iel; ++i)
        {

            teselate_vector_size = BinReader::GetBinVal<T>(buff, curr_offset);

            for (T j = 0; j < teselate_vector_size; ++j)
            {
                teselate = BinReader::GetBinVal<float>(buff, curr_offset);
                teselate_vector.push_back(teselate);
            }

            teselates_groups.teselates.push_back(move(teselate_vector));
            teselate_vector.clear();

            if (two_worlds_switch == true)
            {
                teselate_flag = static_cast<uint32_t>(BinReader::GetBinVal<bool>(buff, curr_offset));
                teselates_groups.teselate_flags.push_back(teselate_flag);
            }

            iel_scalar = BinReader::GetBinVal<float>(buff, curr_offset);
            teselates_groups.iel_scalars.push_back(iel_scalar);

        }
    }


    void GetCurves(size_t& number_of_iel,
                   AllCurvesGroups& curves_groups,
                   uint8_t* buff,
                   size_t& curr_offset,
                   bool e2160_switch = false
    );
};



template <typename A, int A_SIZE, typename B, int B_SIZE>
struct TwoTypeArray
{
    A a[A_SIZE] = {};
    B b[B_SIZE] = {};
};

//------------
// Aod Writer
//------------

namespace AodWriter
{
    template<typename T>
    inline void WrtVal(string name, T value, stringstream& output, int margin_val = two_quads)
    {
        Margin(output, margin_val); output << name << " " << value << endl;
    }

    inline void WrtSinStr(string sin_str, stringstream& output, int margin_val = one_quad)
    {
        Margin(output, margin_val); output << sin_str << endl;
    }


    template<typename T>
    void WriteConstValueToAod(string name, 
                             const T& value, 
                             unordered_map<T, string>& list_of_masks,
                             stringstream& output,
                             int quadcount)
    {
        if (list_of_masks[value] == "" && value == 0) return;

        Margin(output, quadcount);

        if (list_of_masks[value] == "" && value != 0)
        {
            output << name << " " << value << endl;
            return;
        }

        output << name << " " << list_of_masks[value] << endl;
    }

    template<typename T>
    void WriteArrayToAod(string name, 
                        const T arr[], 
                        int size,  
                        stringstream& output,
                        int quadcount = two_quads)
    {
        Margin(output, quadcount);
        output << name << " ";

        for (int i = 0; i < size; ++i)
        {
            if (!(i == size - 1))
            {
                output << arr[i] << ", ";
            }
            else
            {
                output << arr[i];
            }

        }

        output << endl;
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void WriteTwoTypeArrayToAod(string name, 
                                TwoTypeArray<A, A_SIZE, B, B_SIZE>& arr, 
                                stringstream& output,
                                int quadcount = two_quads)
    {
        Margin(output, quadcount);
        output << name << " ";

        for (int i = 0; i < A_SIZE; ++i)
        {
                output << arr.a[i] << ", ";
        }

        for (int i = 0; i < B_SIZE; ++i)
        {
            if (!(i == B_SIZE - 1))
            {
                output << arr.b[i] << ", ";
            }
            else
            {
                output << arr.b[i];
            }

        }

        output << endl;
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void WriteTwoTypeArrayToAodViaScheme(string name,
                                         TwoTypeArray<A, A_SIZE, B, B_SIZE>& arr,
                                         string scheme,
                                         stringstream& output,
                                         int quadcount = two_quads)
    {
        Margin(output, quadcount);
        output << name << " ";

        Misc::RemoveCharFromTheString(scheme, ' ');

        size_t j = 0;
        size_t k = 0;
        for (size_t i = 0; i < scheme.size(); ++i)
        {
            if (scheme[i] == 'a')
            {
                output << arr.a[j];
                ++j;
            }

            if (scheme[i] == 'b')
            {
                output << arr.b[k];
                ++k;
            }

            if (scheme[i] == ',')
            {
                output << ", ";
            }

        }
        
        output << endl;
    }


    template <typename A, int A_SIZE, typename B, int B_SIZE>
    void WriteArrayOfTwoTypeArrayToAod(string name,
                                       TwoTypeArray<A, A_SIZE, B, B_SIZE> arr[],
                                       int size,
                                       int quadcount,
                                       stringstream& output,
                                       bool print_endl = true)
    {
        Margin(output, quadcount);
        output << name << " ";

        for (int j = 0; j < size; ++j)
        {

            for (int i = 0; i < A_SIZE; ++i)
            {
                output << arr[j].a[i] << ", ";
            }

            for (int i = 0; i < B_SIZE; ++i)
            {
                if ( (j == size - 1) && (i == B_SIZE - 1) && (print_endl == true) )
                {
                    output << arr[j].b[i];
                    break;
                }

                output << arr[j].b[i] << ", ";
            }
        }

        if (print_endl == true)
        {
            output << endl;
        }
    }



    void WriteStreamToFile(stringstream& output_stream, string output_file_name);

    void WriteCurvesAndTeselates(
        AllCurvesGroups& curves_groups,
        AllTeselatesGroups& teselates_groups,
        vector<string>& labels,
        stringstream& output
        );


    template<typename T>
    void WriteVector(string name, vector<T>& vec,
        stringstream& output)
    {

        Margin(output, two_quads); output << name << " ";

        size_t j = 0;
        for (const auto& single_element : vec)
        {
            output << single_element << ((j == vec.size() - 1) ? ("") : (", "));
            ++j;
        }
        output << endl;

    }



}

namespace AW = AodWriter;

namespace CppWriter
{
    template<typename Type>
    inline string ConvertTrivialTypeToString()
    {
        stringstream result = { };

        if (is_unsigned_v<Type>) 
        {
            result << 'u';
        }

        result << "int" << sizeof(Type) * 8 << "_t";

        return result.str();
    }


    template<typename Type>
    inline string ConvertTrivialTypeToString(Type)
    {
        return ConvertTrivialTypeToString<Type>();
    }

    template<typename Type>
    void WriteTrivialValue(stringstream& ss, string name, Type& value, bool bool_value = false)
    {
        ss << ConvertTrivialTypeToString(value) << " " << name << " = ";
        
        if (bool_value == false)
        {
            ss << value << ";";
            return;
        }

        if (bool_value == true)
        {
            uint32_t help_val = static_cast<uint32_t>(value);
            switch (help_val)
            {
            case 0: 
                ss << "false;"; 
            break;

            case 1:
                ss << "true;";
            break;
            }

            return;
        }

    }

    void WriteFloatValue(stringstream& ss, string name, float& value);

    void WriteLabel(stringstream& ss, string name);

    void WriteComLine(stringstream& ss, int count, char symbol = '-');

}

namespace CPPW = CppWriter;


//---------------------
// InputBinFile CLASS
//---------------------

class InputBinFile
{
public:

    //Pola:
    uint8_t* m_file_buffer;

    //Metody:
    void Informations(ostream& o) const;
    void ExitWhenFileDontExist(ostream& o) const;

    InputBinFile(char* argfilename);
    ~InputBinFile();

    //Getery:
    bool GetIsOpened() const;
    bool GetIsOpenNow() const;
    size_t GetFileLength() const;

private:
    //Pola:
    fstream m_input_file;
    char* m_file_name;
    size_t m_file_length;
    bool m_is_opened;
    bool m_is_open_now;

    //Metody:
    void OpenFile();
    void MeasureFile();
    void PutFileToBuffer();

};


//-----------------
// Wazne struktury:
//-----------------

//-------------
// Effect Data
//-------------

struct EffectData
{
    EffectData();

    uint32_t particle_2D_switch;
    uint32_t width;
    uint32_t height;

    //only for e2160:
    uint32_t e2160_effect_scale;

    float start_time;
    float time;
    uint32_t loops;

    constexpr const unsigned int static scale_a_size = 3;  
    constexpr const unsigned int static scale_b_size = 1;
    constexpr const unsigned int static scales_size = 2;
    TwoTypeArray<float, scale_a_size, uint32_t, scale_b_size> scales[scales_size] = {};

    string annotation;

    //------------------------
    uint32_t unknown_value_1; // ??
    uint32_t unknown_value_2; // ??
    //------------------------

    AllCurvesGroups curves_groups = {};
    AllTeselatesGroups teselates_groups = {};

    vector<uint32_t> linked_min_max = {};

    uint32_t all_teselate_zero_mask;
};

//-----------------------
// Effect Implementation
//-----------------------

class EffectClass
{
public:
    EffectClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo());

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

private:
    void GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset);
    void GetFromE2160File(uint8_t* buff, size_t& curr_offset);
    
    void WriteAsParticleGenAndTwoWorldsFormat(stringstream& output);
    void WriteAsE2160Format(stringstream& output) const;


    EffectData m_effect_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};


//---------------
// Emiter Data
//---------------

struct EmiterData
{
    EmiterData();

    string emiter_name; 
    float start_time; 
    float time; 

    uint32_t coordinates; 
    uint32_t budget_type;

    vector<string> slot_strings = {}; 

    constexpr const unsigned int static surface_size = 3; 
    uint32_t surface[surface_size] = { 0 }; 
    uint32_t sun_ray_direction_switch; 


    constexpr const unsigned int static sphere_size = 5; 
    float sphere[sphere_size] = { 0.0 }; 

    // cubic a, a, a, b
    constexpr const unsigned int static cubic_a_size = 3; 
    constexpr const unsigned int static cubic_b_size = 1; 
    TwoTypeArray<float, cubic_a_size, uint32_t, cubic_b_size> cubic = {}; 

    // plane a, a, b
    constexpr const unsigned int static plane_a_size = 2; 
    constexpr const unsigned int static plane_b_size = 1; 
    TwoTypeArray<float, plane_a_size, uint32_t, plane_b_size> plane = {}; 
    uint32_t use_color_switch; 

    string texture; 

    uint32_t create_on_ground; 
    uint32_t trajectory_z_from_ground; 

    constexpr const unsigned int static rotation_size = 3; 
    uint32_t rotation[rotation_size] = { 0 }; 


    uint32_t enable_light; 
    float umbra_distance_mul; 


    // only for two worlds:
    //------------------------
    uint32_t cast_shadows; 
    //------------------------

    constexpr const unsigned int static scales_a_size = 14; 
    constexpr const unsigned int static scales_b_size = 2;
    TwoTypeArray<float, scales_a_size, uint32_t, scales_b_size> scales = {};

    uint32_t move_end_of_laser_switch; 
    uint32_t attach_to_end_of_laser_switch; 

    constexpr const unsigned int static random_seed_size = 2; 
    uint32_t random_seed[random_seed_size] = { 0 }; 

    uint32_t unknown_value_1; 
    uint32_t unknown_value_2; 
    uint32_t unknown_value_3; 
    uint32_t unknown_value_4; 
    uint32_t unknown_value_5; 

    TeselateAndCurvePair convert_backup = {};

    AllCurvesGroups curves_groups = {};
    AllTeselatesGroups teselates_groups = {};

    vector <uint32_t> linked_min_max = {};

    uint32_t all_teselate_zero_mask; 
};

//-----------------------
// Emiter Implementation
//-----------------------

class EmiterClass
{
public:
    EmiterClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                uint8_t arg_fourth_byte_of_header = 0);

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);
    
    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);


private:

    void GetFromKsFile(uint8_t* buff, size_t& curr_offset);
    void GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset);
    void GetFromE2160File(uint8_t* buff, size_t& curr_offset);

    void WriteAsKsFormat(stringstream& output);
    void WriteAsParticleGenAndTwoWorldsFormat(stringstream& output);
    void WriteAsE2160Format(stringstream& output);


    static uint32_t emiter_number;

    EmiterData m_emiter_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};

    uint8_t m_fourth_byte_of_header;
};

//---------------
// Particle Data
//---------------

struct ParticleData
{
    ParticleData();

    string particle_name; 

    float start_time; 
    float time; 

    uint32_t coordinates; 
    uint32_t type; 

    string mesh; 

    // face a, b, b, b, b, b 
    constexpr const unsigned int static face_a_size = 1; 
    constexpr const unsigned int static face_b_size = 5; 
    TwoTypeArray<uint32_t, face_a_size, float, face_b_size> face = {}; 

    float triangle_a; // ------ only for e2160
    float screen_depth_offset; 

    uint32_t multiply_dxy_by_prt_scale_switch; 

    // trail a, b
    constexpr const unsigned int static trail_a_size = 1; 
    constexpr const unsigned int static trail_b_size = 1; 
    TwoTypeArray<float, trail_a_size, uint32_t, trail_b_size> trail = {};

    float y_texture_mul;

    vector<string> textures = {}; 

    vector<string> bump_textures = {}; 

    constexpr const unsigned int static tex_anim_a_size = 10;
    constexpr const unsigned int static tex_anim_b_size = 1;
    TwoTypeArray<uint32_t, tex_anim_a_size, float, tex_anim_b_size> tex_anim = {};

    constexpr const unsigned int static coordinates_ltrb_size = 4; 
    float coordinates_ltrb[coordinates_ltrb_size] = { 0.0 }; 

    uint32_t distortion_particle_switch; 

    uint32_t x_mirrored; 
    uint32_t y_mirrored; 

    uint32_t color_rand_type; 

    uint32_t unknown_value_1; 
    uint32_t unknown_value_2; 
    uint32_t unknown_value_3; 

    constexpr const unsigned int static trajectory_size = 2; 
    uint32_t trajectory[trajectory_size] = { 0 }; 

    // gravity a, a, a, a, a, a, a, b 
    constexpr const unsigned int static gravity_a_size = 7; 
    constexpr const unsigned int static gravity_b_size = 1; 
    TwoTypeArray<float, gravity_a_size, uint32_t, gravity_b_size> gravity = {}; 

    uint32_t enable_light; // ----- only for ks

    uint32_t gravity_rot_from_trajectory_z_switch; 
    uint32_t gravity_rot_from_trajectory_x_switch; 
    uint32_t gravity_rot_from_trajectory_y_switch; 

    uint32_t dissappears_on_ground; 

    uint32_t create_on_ground_switch; 
    uint32_t trajectory_z_from_ground; 


    constexpr const unsigned int static scale_a_size = 3; 
    constexpr const unsigned int static scale_b_size = 1; 
    constexpr const unsigned int static scales_size = 3; 
    TwoTypeArray<float, scale_a_size, uint32_t, scale_b_size> scales[scales_size] = {}; 

    float scales_end; // ----------- only for ks 


    TeselateAndCurvePair convert_backup = {};

    AllCurvesGroups curves_groups = {};
    AllTeselatesGroups teselates_groups = {};

    vector<uint32_t> linked_min_max = {}; 

    uint32_t all_teselate_zero_mask; 

    uint32_t layer; 

};

//-------------------------
// Particle Implementation
//-------------------------

class ParticleClass
{
public:
    ParticleClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo());

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

private:

    void GetFromParticleGenAndTwoWorldsFile(uint8_t* buff, size_t& curr_offset);
    void GetFromE2160File(uint8_t* buff, size_t& curr_offset);
    void GetFromKsFile(uint8_t* buff, size_t& curr_offset);

    void WriteAsParticleGenAndTwoWorldsFormat(stringstream& output);
    void WriteAsE2160Format(stringstream& output);
    void WriteAsKsFormat(stringstream& output);

    static uint32_t particle_number;

    ParticleData m_particle_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};

//--------------------------
//  Pair Particle Emiter Data
//--------------------------

struct PairParticleEmiterData
{
    PairParticleEmiterData();

    string group_name;

    string annotation;

    string emiter_name;
    string particle_name;

    uint32_t emiter_index;
    uint32_t particle_index;

    float time;

    // loopedemission a, a, a, b 
    constexpr const unsigned int static looped_emission_a_size = 3;
    constexpr const unsigned int static looped_emission_b_size = 1;
    TwoTypeArray<uint32_t, looped_emission_a_size, float, looped_emission_b_size> looped_emission = {};

    uint32_t simple_one_particle_switch;
    uint32_t mask_2d_switch;
    uint32_t hardware_cursor_switch;
    uint32_t stop_in_partial_pause_switch;
    uint32_t finish_missile_switch;

    uint32_t emits_groups_switch;

    vector<string> emits_groups = {};

    uint32_t only_emited_by_other_emiter_switch;
    uint32_t draw_particle_emiter;

};

//-------------------------------------
// Pg Pair Particle Emiter Implementation
//-------------------------------------

class PairParticleEmiterClass
{
public:
    PairParticleEmiterClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo());

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

private:
    void GetFromPgAndTwAndE2160File(uint8_t* buff, size_t& curr_offset);
    void GetFromKsFile(uint8_t* buff, size_t& curr_offset);

    void WriteAsPgAndTwAndE2160Format(stringstream& output);
    void WriteAsKsFormat(stringstream& output);

    static uint32_t group_number;

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

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

private:
    void GetFromPgAndTwFile(uint8_t* buff, size_t& curr_offset);
    void GetFromE2160File(uint8_t* buff, size_t& curr_offset);
    void GetFromKsFile(uint8_t* buff, size_t& curr_offset);


    void WriteAsPgAndTwAndE2160Format(stringstream& output) const;
    void WriteAsKsFormat(stringstream& output) const;

    ParticleEndData m_particle_end_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};


struct CfgFileData
{
    ParticleFileVersionInfo input_dynamic_particle_file_version_info = ks_dynamic_file_type;

    bool force_dynamic_particle_export_format_switch = false;

    ParticleFileVersionInfo forced_dynamic_particle_file_version_info_for_export = ks_dynamic_file_type;
};


class CfgCompilator
{
public:
    CfgCompilator(const char arg_cfg_file_name[], ostream& arg_o);

    CfgFileData GetCfgFileData() const;

private:
    void GetInfoFromCfg();
    void GetValuesFromString();
    void InitializeSwitches(string& value_type, string& value);

    string m_instruction;
    string m_value_type;
    string m_value;

    void InitializeDynamicParticleHashMap();
    unordered_map<string, ParticleFileVersionInfo> m_dynamic_particle_file_version_map = {};

    vector <uint8_t> m_cfg_clean_file = {};
    ostream& m_o;

    CfgFileData m_cfg_file_data = {};

};


//---------------------------
// Dynamic Particle Structure
//---------------------------

struct DynamicParticleData
{
    DynamicParticleData();

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

    constexpr const unsigned int static size2_size = 4;
    float size_2[size2_size] = { 0.0 };

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

//--------------------------------
// Dynamic Particle Implementation
//--------------------------------

class DynamicParticleClass
{
public:

    DynamicParticleClass(CfgFileData arg_cfg_file_data = CfgFileData());

    void GetAndSetAndWriteDynamicParticleToAod(uint8_t* buffer,
        size_t& offset, stringstream& output, string& particle_name);

    void SetParticleDirectoryPath(fs::path& arg_pth);

    fs::path GetParticleDirectoryPath() const;

private:
    //Pola:
    fs::path m_particle_directory_path;

    DynamicParticleData m_private_dynamic_particle_data = {};

    CfgFileData m_dynamic_particle_cfg = {};

    unordered_map<uint32_t, string> m_list_of_type_consts = {};
    unordered_map<uint32_t, string> m_list_of_lighttype_consts = {};

    //Metody:
    bool GetData(uint8_t* buffer, size_t& offset, string particle_name,
                 DynamicParticleData& dynamic_particle_data);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

    void WriteToFile(const DynamicParticleData& dynamic_particle_data,
                     stringstream& output, string particle_name, int count_of_tabulation);

    void WriteCompilatorFlagsFile() const;

    void InitializeHashMaps();

};

//----------------
// FileHeaderData
//----------------
struct FileHeaderData
{
    //Pola:
    FileHeaderData();

    constexpr const unsigned int static c_header_size = 4;
    uint8_t header[c_header_size] = {0};
    particle_type_value particle_file_type;
    string guid;

};


//--------------------------
// FileHeader Implementation
//--------------------------

class FileHeaderClass
{
public:
    FileHeaderClass();
    
    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(particle_type_value arg_particle_type_value);

    particle_type_value GetParticleFileType() const;

    uint8_t GetFourthByteOfHeader() const;

private:

    void WriteAsKsFormat(stringstream& output) const;
    void WriteAsPgAndTwFormat(stringstream& output) const;
    void WriteAsE2160Format(stringstream& output) const;

    FileHeaderData m_file_header_data = {};
    particle_type_value m_app_particle_file_type;
};

//----------------
// CustomBlockData
//----------------

struct CustomBlockData
{
    constexpr const unsigned int static c_format_size = 4;
    uint8_t format[c_format_size] = { 0 };
    vector<uint32_t> data = {};
};

//----------------------------
// CustomBlock Implementation
//----------------------------

class CustomBlockClass
{
public:
    CustomBlockClass(ParticleFileVersionInfo arg_prt_file_version_info = ParticleFileVersionInfo(),
                     bool arg_is_child = false);

    void GetFromFile(uint8_t* buff, size_t& curr_offset);
    void WriteToFile(stringstream& output);

    void SetPrtVersion(ParticleFileVersionInfo arg_prt_file_version_info);

private:
    void GetFromKsFile(uint8_t* buff, size_t& curr_offset);
    void GetFromPgAndTwAndE2160File(uint8_t* buff, size_t& curr_offset);

    void WriteAsKsFormat(stringstream& output);

    bool m_is_child;

    CustomBlockData m_custom_block_data = {};
    ParticleFileVersionInfo m_prt_file_version_info = {};
};


namespace Utility
{
    void InsertLinkedMinMax(vector<uint32_t>& linked_min_max,
                                  size_t index, 
                                  uint32_t value = locked_linked_min_max);

    void EraseLinkedMinMax(vector<uint32_t>& linked_min_max, size_t index);



    void CopyCurveToDestination(AllCurvesGroups& curves_groups,
                                       size_t destination,
                                       size_t source);

    void CopyTeselateAndCurvePairToBackup(AllCurvesGroups& curves_groups,
        AllTeselatesGroups& teselates_groups,
        TeselateAndCurvePair& backup,
        size_t index);


    void InsertTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
        AllTeselatesGroups& teselates_groups,
        size_t index,
        TeselateAndCurvePair arg_tes_and_curve = TeselateAndCurvePair());


    void EraseTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
        AllTeselatesGroups& teselates_groups,
        size_t index);

    void EraseSetOfTeselateAndCurveGroup(AllCurvesGroups& curves_groups,
        AllTeselatesGroups& teselates_groups,
        size_t index_from,
        size_t index_to);

    void MakeCurves(AllTeselatesGroups& teselates_groups,
        AllCurvesGroups& curves_groups,
        const SingleCurvePoint& template_curve_point);

    template<typename T>
    void InsertToVector(vector<T>& vec, const T& value, size_t n)
    {
        for (size_t i = 0; i < n; ++i)
            vec.push_back(value);
    }

    string GenerateGuidString();
}

namespace EmiterUtility
{

    void ConvertTeselatesAndCurves(EmiterData& emiter_data,
        particle_type_value& src,
        particle_type_value& dst);

    void ConvertTeselatesAndCurvesWholeFormat(EmiterData& emiter_data,
        particle_type_value& src,
        particle_type_value& dst);
};

namespace ParticleUtility
{
    void ConvertTeselatesAndCurves(ParticleData& particle_data,
        particle_type_value& src,
        particle_type_value& dst);

    void ConvertTeselatesAndCurvesWholeFormat(ParticleData& particle_data,
        particle_type_value& src,
        particle_type_value& dst);
};

namespace EffectUtility
{
    void ConvertTeselatesAndCurves(EffectData& effect_data,
        particle_type_value& src,
        particle_type_value& dst);

    void ConvertTeselatesAndCurvesWholeFormat(EffectData& effect_data,
        particle_type_value& src,
        particle_type_value& dst);
};

namespace CustomBlockUtility
{
    void SetCustomBlockVersionForNotChild(CustomBlockData& custom_block_data,
        old_dynamic_particle_type_value& dst);

    void SetCustomBlockVersionForChild(CustomBlockData& custom_block_data,
         old_dynamic_particle_type_value& dst);

    void SetCustomBlockVersion(CustomBlockData& custom_block_data,
                               old_dynamic_particle_type_value& src,
                               old_dynamic_particle_type_value& dst,
                               bool& is_child);
}



class ParticleDecompiler
{
public:
    //Metody:
    ParticleDecompiler(istream& argin, ostream& argo, int my_argc, char* my_argv[]);

private:
    fs::path m_input_path;
    fs::path m_particle_directory_path;
    fs::path m_program_parent_path;

    constexpr const unsigned int static c_correct_number_of_args = 2;

    constexpr const unsigned int static c_number_of_args_with_force_format = 4;

    const vector<string> correct_formats = { ks_format_str, earth2160_format_str, tw_format_str, pg_format_str };

    unordered_map<particle_type_value, string> m_format_names = {};

    unordered_map<string, ParticleFileVersionInfo> m_format_containers = {};

    istream& m_in_stream;
    ostream& m_o_stream;

    //input file
    string m_input_name;

    //output file
    stringstream m_output_stream;
    string m_output_file_path;

    //additional data file
    stringstream m_bonus_file_stream;
    string m_bonus_file_path;

    //iloœæ obiektów w pliku
    uint32_t m_number_of_emiters;
    uint32_t m_number_of_particles;
    uint32_t m_number_of_pairs;

    string m_obj_name;

    size_t m_offset_in_file;

    bool m_force_end_format;
    ParticleFileVersionInfo m_forced_format = {};

    particle_type_value m_my_particle_file_type;

    uint8_t m_fourth_byte_of_header;

    //Metody:
    void StandardProgramExecution();
    //{
    void ShowIntroScreen();
    //}

    void ComputeAdditionalInfo();

    void CheckParticleStatus(particle_type_value& my_particle_file_type);

    void WriteIntroToFile(particle_type_value& my_particle_file_type);

    void DecompileDynamicParticleToMyAod(InputBinFile& input_file);

    void DecompileParticleEditFileToMyAod(ParticleFileVersionInfo arg_particle_file_type, 
                                         InputBinFile& input_file);
    
    void InitializeFormatHashMaps();
    void ShowInfoAboutParticle(particle_type_value& arg_particle_file_type);


    void GetInformationAboutEndFormat();

};