#include "my_aod_to_particle.hpp"

constexpr size_t number_of_value_types = 30;
const char* value_types[] = 
{
	"char",
	"char[]",

	"uint8_t",
	"uint8_t[]",

	"uint16_t",
	"uint16_t[]",

	"uint32_t",
	"uint32_t[]",

	"uint64_t",
	"uint64_t[]",

	"GUID",
	"string",
	"struct",

	"int8_t",
	"int8_t[]",

	"int16_t",
	"int16_t[]",

	"int32_t",
	"int32_t[]",

	"int64_t",
	"int64_t[]",

	"short",
	"short[]",

	"int",
	"int[]",

	"longint",
	"longint[]",

	"bool",
	"bool[]",

	"__compiler_var__"

};

const vector<string> dynamic_particle_correct_formats = { e2150_format_str,
														  ks_format_str,
														  ww3_or_hh_format_str,
														  ww2_or_fa_or_pc2_format_str };


CppCompilator::CppCompilator(vector <char>& arg_file_buffer) :
	m_file_buffer(arg_file_buffer), m_file_length(arg_file_buffer.size()), 

	m_its_string(false), 
	m_its_comment(false),
	m_instruction(string()), 
	m_value_type(string()), 
	m_name(string()), 
	m_value(string())
{

	if (m_file_buffer.empty() == false && m_file_length != 0)
	{
		InitializeHashMaps();

		DeleteComments();

		DeleteContrCharAndPutToVector();
	}

}

void CppCompilator::InitializeHashMaps()
{
	m_dynamic_particle_file_version_info_hash_map[""] = ParticleFileVersionInfo();
	m_dynamic_particle_file_version_info_hash_map[e2150_format_str] = earth2150_dynamic_file_type;
	m_dynamic_particle_file_version_info_hash_map[ww3_or_hh_format_str] = ww3_or_heli_heroes_dynamic_file_type;
	m_dynamic_particle_file_version_info_hash_map[ww2_or_fa_or_pc2_format_str] = ww2_or_fa_or_pc2_dynamic_file_type;
	m_dynamic_particle_file_version_info_hash_map[ks_format_str] = ks_dynamic_file_type;
}


ParticleFileVersionInfo CppCompilator::GetDynamicParticleVersion() const { return m_dynamic_particle_file_version_info; }

void CppCompilator::WriteGUID(vector<char>& output_file_buff, string& slowo)
{
	const int hex_base = 16;

	uint32_t four_bytes = stoul(slowo.substr(0, 8), nullptr, hex_base);
	BW::WriteValue<uint32_t>(output_file_buff, four_bytes);

	uint16_t two_bytes = static_cast<uint16_t>(stoul(slowo.substr(9, 4), nullptr, hex_base));
	BW::WriteValue<uint16_t>(output_file_buff, two_bytes);

	two_bytes = static_cast<uint16_t>(stoul(slowo.substr(14, 4), nullptr, hex_base));
	BW::WriteValue<uint16_t>(output_file_buff, two_bytes);

	two_bytes = static_cast<uint16_t>(_byteswap_ushort(static_cast<unsigned short>(stoul(slowo.substr(19, 4), nullptr, hex_base))));
	BW::WriteValue<uint16_t>(output_file_buff, two_bytes);

	four_bytes = static_cast<uint32_t>(_byteswap_ulong(stoul(slowo.substr(24, 8), nullptr, hex_base)));
	BW::WriteValue<uint32_t>(output_file_buff, four_bytes);

	two_bytes = static_cast<uint32_t>(_byteswap_ushort(static_cast<unsigned short>(stoul(slowo.substr(32, 4), nullptr, hex_base))));
	BW::WriteValue<uint16_t>(output_file_buff, two_bytes);

}

void CppCompilator::GetValuesFromString()
{
	while (1)
	{
		bool nopswitch = true;

		//Sprawdzenie czy jest prawidlowy typ
		for (uint32_t i = 0; i < number_of_value_types; ++i)
		{
			if (m_instruction.substr(0, string(value_types[i]).length()) == value_types[i])
			{
				m_value_type = value_types[i];
				nopswitch = false;
				break;
			}
		}

		// Jesli typ byl nieprawidlowy czyli nopswitch == true to wychodze
		if (nopswitch == true)
		{
			m_value_type = "nop";
			m_name = string();
			m_value = string();
			return;
		}

		//Wyodrebnienie nazwy od instrukcji
		for (uint64_t i = m_value_type.length(); i < m_instruction.length(); ++i)
		{
			if ((m_instruction[i] == '=') || (m_instruction[i] == '['))
			{
				if (m_instruction[i] == '[')
				{
					m_value_type += "[]";
				}

				break;
			}

			m_name += m_instruction[i];
		}

		//Wyodrebnienie wartosci od instrukcji
		uint64_t valueindex = m_instruction.find("=") + 1;
		m_value = m_instruction.substr(valueindex, m_instruction.length() - valueindex - 1);


		// Typ string lub tablica char[] lub GUID lub samo char
		if ((m_value_type == "string") || (m_value_type == "char[]") || (m_value_type == "GUID") || (m_value_type == "char"))
		{
			m_value = m_instruction.substr(valueindex + 1, m_instruction.length() - (valueindex + 1) - 2);
			return;
		}

		// Typ struktury
		if (m_value_type == "struct")
		{
			valueindex = m_instruction.find("{") + 1;
			m_instruction = m_instruction.substr(valueindex, m_instruction.length() - valueindex);

			m_value_type = string();
			m_name = string();
			m_value = string();

			continue;

		}

		// Typ boolean
		if (m_value_type == "bool")
		{
			if (m_value == "true")
			{
				m_value = "1";
			}
			if (m_value == "false")
			{
				m_value = "0";
			}
			return;
		}


		break;
	}

}



void CppCompilator::WriteInstructionToBin(vector<char>& output_file_buff, string& valuetype, string& name, string& value)
{

	// uints
	if (valuetype.substr(0, 4) == "uint")
	{
		// uint32_t
		if (valuetype == value_types[6])
		{
			WriteValueToFile<uint32_t>(output_file_buff, value, 'u');
			return;
		}

		// uint8_t
		if (valuetype == value_types[2])
		{
			output_file_buff.push_back(static_cast<uint8_t>(stoul(value)));
			return;
		}

		// uint16_t
		if (valuetype == value_types[4])
		{
			WriteValueToFile<uint16_t>(output_file_buff, value, 'u');
			return;
		}

		// uint64_t
		if (valuetype == value_types[8])
		{
			WriteValueToFile<uint64_t>(output_file_buff, value, 'u');
			return;
		}

		// uint32_t[]
		if (valuetype == value_types[7])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<uint32_t>(output_file_buff, value, 'u');

			return;
		}


		// uint8_t[]
		if (valuetype == value_types[3])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));
			WriteTabToFile<uint8_t>(output_file_buff, value, 'u');

			return;
		}

		// uint16_t[]
		if (valuetype == value_types[5])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<uint16_t>(output_file_buff, value, 'u');

			return;
		}

		// uint64_t[]
		if (valuetype == value_types[9])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<uint64_t>(output_file_buff, value, 'u');

			return;
		}
	}

	// string
	if (valuetype == value_types[11])
	{
		BW::WriteString(output_file_buff, value, false);
		return;
	}

	// bool
	if (valuetype == value_types[27])
	{
		output_file_buff.push_back(static_cast<int8_t>(stol(value)));
		return;
	}

	// bool[]
	if (valuetype == value_types[28])
	{
		BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

		WriteTabToFile<int8_t>(output_file_buff, value, 'd');

		return;
	}


	if (valuetype.substr(0, 3) == "int")
	{

		// int32_t albo int
		if ((valuetype == value_types[17]) || (valuetype == value_types[23]))
		{
			WriteValueToFile<int32_t>(output_file_buff, value, 'd');
			return;
		}

		// int8_t
		if (valuetype == value_types[13])
		{
			output_file_buff.push_back(static_cast<int8_t>(std::stol(value)));
			return;
		}

		// int16_t 
		if (valuetype == value_types[15])
		{
			WriteValueToFile<int16_t>(output_file_buff, value, 'd');
			return;
		}

		// int64_t 
		if (valuetype == value_types[19])
		{
			WriteValueToFile<int64_t>(output_file_buff, value, 'd');
			return;
		}


		// int32_t[] albo int[]
		if ((valuetype == value_types[18]) || (valuetype == value_types[24]))
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<int32_t>(output_file_buff, value, 'd');

			return;
		}

		// int8_t[]
		if (valuetype == value_types[14])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<int8_t>(output_file_buff, value, 'd');

			return;
		}


		// int16_t[] 
		if (valuetype == value_types[16])
		{
			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<int16_t>(output_file_buff, value, 'd');

			return;
		}


		// int64_t[] 
		if (valuetype == value_types[20])
		{

			BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

			WriteTabToFile<int64_t>(output_file_buff, value, 'd');

			return;
		}

	}

	//  char || char[]
	if ((valuetype == value_types[0]) || (valuetype == value_types[1]))
	{
		output_file_buff.insert(output_file_buff.end(), value.begin(), value.end());
		return;
	}

	// short
	if (valuetype == value_types[21])
	{
		WriteValueToFile<int16_t>(output_file_buff, value, 'd');
		return;
	}

	// short[]
	if (valuetype == value_types[22])
	{

		BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));

		WriteTabToFile<int16_t>(output_file_buff, value, 'd');

		return;
	}

	// long int
	if (valuetype == value_types[25])
	{
		WriteValueToFile<int64_t>(output_file_buff, value, 'd');
		return;
	}

	// long int[]
	if (valuetype == value_types[26])
	{

		BW::WriteValue<uint32_t>(output_file_buff, static_cast<uint32_t>(CountValuesInTab(value)));
		WriteTabToFile<int64_t>(output_file_buff, value, 'd');

		return;
	}


	// GUID
	if (valuetype == value_types[10])
	{
		WriteGUID(output_file_buff, value);
		return;
	}


	// __compiler_var__
	if(valuetype == value_types[29]) 
	{
		if (name == "dynamic_particle_version")
		{
			if (!(Utility::CheckAnswerWithEveryElementInVec(value, dynamic_particle_correct_formats)))
			{
				cerr << "Error: The __compilator_flags__.cfg file contains an incorrect dynamic particle format." << endl
					 << "       Please correct the __compilator_flags__.cfg file." << endl
					 << "       At this point, the program will assume that it is ks format." << endl;

				value = ks_format_str;
			}

			m_dynamic_particle_file_version_info = m_dynamic_particle_file_version_info_hash_map[value];

			cout	<< endl
					<< "----------------------------------------------------------------------------------" << endl
					<< "The following format was found in the file __compilator_flags__.cfg: " << value		<< endl
					<< "----------------------------------------------------------------------------------" << endl << endl;

			return;
		}
		
		if (name != "dynamic_particle_version")
		{
			cerr << "Error: The __compilator_flags__.cfg file contains an incorrect __compiler_var__ flag." << endl
				 << "       Please correct the __compilator_flags__.cfg file." << endl;

			m_value_type = string();
			m_name = string();
			m_value = string();

			exit(EXIT_FAILURE);
		}

		return;
	}

}

uint64_t CppCompilator::CountValuesInTab(string& value)
{
	uint64_t tabsize = 0;

	for (uint64_t i = 0; i < value.length(); ++i)
	{
		if (value[i] == ',')
			++tabsize;
	}

	if (tabsize != 0) ++tabsize;

	return tabsize;
}

void CppCompilator::DeleteComments()
{
	for (uint64_t i = 0; i < m_file_length - 1; ++i)
	{
		if ((m_file_buffer[i] == '/') && (m_file_buffer[i + 1] == '/'))
		{
			m_its_comment = !(m_its_comment);
		}

		if ((m_file_buffer[i] == 0x0D) && (m_file_buffer[i + 1] == 0x0A) && (m_its_comment == true))
		{
			m_its_comment = !(m_its_comment);
		}

		if (m_its_comment == true)
		{
			m_file_buffer[i] = ' ';
			continue;
		}
	}

}

void CppCompilator::DeleteContrCharAndPutToVector()
{
	for (uint64_t i = 0; i < m_file_length - 1; ++i)
	{
		if (m_file_buffer[i] == '\"')
		{
			m_its_string = !m_its_string;
		}

		if ((m_file_buffer[i] <= 32) && (m_its_string == false))
		{
			continue;
		}

		m_clean_file.push_back(m_file_buffer[i]);
	}
}

//Cpp
void CppCompilator::CompileCppFileToBin(vector <char>& arg_output_file_buff)
{
	for (uint64_t i = 0; i < m_clean_file.size(); ++i)
	{
		m_instruction += m_clean_file[i];

		if (m_clean_file[i] == ';')
		{
			DEBUG_PRINT("----\n");
			DEBUG_PRINT(m_instruction.c_str());
			DEBUG_PRINT("\n");

			GetValuesFromString();
			WriteInstructionToBin(arg_output_file_buff, m_value_type, m_name, m_value);

			DEBUG_PRINT(m_value_type.c_str()); DEBUG_PRINT("\n");
			DEBUG_PRINT(m_name.c_str()); DEBUG_PRINT("\n");
			DEBUG_PRINT(m_value.c_str()); DEBUG_PRINT("\n");
			DEBUG_PRINT("----\n");

			m_instruction = string();
			m_value_type = string();
			m_name = string();
			m_value = string();
		}
	}
}
