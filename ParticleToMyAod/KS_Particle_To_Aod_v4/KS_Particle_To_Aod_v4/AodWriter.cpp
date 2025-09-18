#include "KS_Particle_To_Aod.hpp"


void AodWriter::WriteStreamToFile(stringstream& output_stream, string output_file_name)
{
    fstream output_file;

    output_file.open(output_file_name, ios::out);
    auto output_stream_data(output_stream.str());
    output_file.write(&output_stream_data[0], output_stream_data.size());
    output_file.close();
}


void AodWriter::WriteCurvesAndTeselates(
    AllCurvesGroups& curves_groups,
    AllTeselatesGroups& teselates_groups,
    vector<string>& labels,
    stringstream& output
)
{
    for (size_t i = 0; i < teselates_groups.teselates.size(); ++i)
    {
        const auto& single_curve = curves_groups.curves[i];

        Margin(output, two_quads); output << "curve"
                                  << " " 
                                  << single_curve.size()
                                  << " " 
                                  << curves_groups.curve_scalars[i] 
                                  << " " 
                                  << labels[i] 
                                  << endl;

        Margin(output, two_quads); output << "{" << endl;


        for (const auto& single_curve_point : single_curve)
        {
            Margin(output, three_quads); output << single_curve_point.x << ", "
                                      << single_curve_point.y << ", " 
                                      << single_curve_point.bonus_data[0] << ", " 
                                      << single_curve_point.bonus_data[1] << ", " 
                                      << single_curve_point.bonus_data[2] << endl;
        }

        Margin(output, two_quads); output << "}" << endl;


        const auto& teselate_vector = teselates_groups.teselates[i];

        Margin(output, two_quads); output << "teselate"
                                  << " " 
                                  << teselate_vector.size() 
                                  << " " 
                                  << teselates_groups.teselate_flags[i] 
                                  << " "
                                  << teselates_groups.iel_scalars[i] 
                                  << " " 
                                  << labels[i] 
                                  << endl;

        Margin(output, two_quads); output << "{" << endl;

        for (const auto& single_teselate : teselate_vector)
        {
            Margin(output, three_quads); output << single_teselate << endl;
        }

        Margin(output, two_quads); output << "}" << endl;
    }

}

