#include "my_aod_to_particle.hpp"


void BinWriter::WriteCurves(vector<char>& output_file_buff,
                            AllCurvesGroups& curves_groups,
                            bool e2160_switch)
{
    size_t i = 0;

    for (auto& curve : curves_groups.curves)
    {
        uint64_t number_of_points = curve.size();
        BW::WriteValue(output_file_buff, number_of_points);

        BW::WriteVector(output_file_buff, curve);

        BW::WriteValue(output_file_buff, curves_groups.curve_scalars[i]);

        if (e2160_switch == true)
        {
            BW::WriteValue(output_file_buff, curves_groups.curve_bonus_values[i]);
        }

        ++i;
    }
  
}

void BinWriter::WriteString(vector<char>& output_file_buff, string& str, bool null_terminator_switch)
{
    
    uint32_t size_of_str = static_cast<uint32_t>(str.length());

    if (null_terminator_switch == true)
    {
        size_of_str += sizeof(char);
    }

    BW::WriteValue<uint32_t>(output_file_buff, size_of_str);
    output_file_buff.insert(output_file_buff.end(), str.begin(), str.end());

    if (null_terminator_switch == true)
    {
        BW::WriteValue<char>(output_file_buff, '\0');
    }
}


void BinWriter::WriteVectorOfStrings(vector<char>& buff, vector<string>& strings)
{
    BW::WriteValue<uint64_t>(buff, static_cast<uint64_t>(strings.size()) );

    for (auto& single_str : strings)
    {
        WriteString(buff, single_str);
    }
}

void BinWriter::WriteFirstStringFromVector(vector<char>& buff, vector<string>& strings, bool null_terminator_switch)
{
    string single_string = string();
    if (!strings.empty())
    {
        single_string = strings[0];
    }
    BW::WriteString(buff, single_string, null_terminator_switch);

}

void BinWriter::GenerateAndWriteGuid(vector<char>& buff)
{
    GUID guid;
    if (CoCreateGuid(&guid) == S_OK)
    {
        BW::WriteValue<GUID>(buff, guid);


        /*
        cout << endl << "DEBUG GUID: -------------------------------------------------------------";
        cout << endl << "WYGENEROWANE GUID = " << uppercase << hex << setfill('0')
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
             << setw(2) << static_cast<int>(guid.Data4[7]) << endl;
        cout << "-------------------------------------------------------------------------" << endl << endl;
        */
    }
    else
    {
        cerr << "Failed to generate GUID!" << endl;
        throw;
    }

}