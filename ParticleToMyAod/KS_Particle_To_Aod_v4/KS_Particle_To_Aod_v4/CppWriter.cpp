#include "KS_Particle_To_Aod.hpp"

void CppWriter::WriteFloatValue(stringstream& ss, string name, float& value)
{
    ss << "float " << name << " = " << value << ";";
}

void CppWriter::WriteLabel(stringstream& ss, string name)
{
    ss << "//-- " << name;
}


void CppWriter::WriteComLine(stringstream& ss, int count, char symbol)
{
    string line = string();

    for (int i = 0; i < count; ++i)
        line += symbol;

    ss << line;
}
