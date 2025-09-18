#include "my_aod_to_particle.hpp"

//----------------------------
// Metody DynamicParticleData
//----------------------------

DynamicParticleData::DynamicParticleData() :
	type(0), 
	light_type(0), 
	delta_size(0.0), 
	null_space(0), 
	additive(0),
	mesh(string()), 
	texture(string()), 
	number_of_nested_particles(0), 
	layer(0),

	m_obj_name(string())
{
}

void DynamicParticleClass::InitializeHashMaps()
{
	if (m_prt_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
	{
		m_list_of_type_consts[""]					= 0;
		m_list_of_type_consts["Explosion"]			= 1;
		m_list_of_type_consts["Track"]				= 2;
		m_list_of_type_consts["ScaleableObject"]	= 3;
		m_list_of_type_consts["MappedExplosion"]	= 4;
		m_list_of_type_consts["FlatExplosion"]		= 5;
		m_list_of_type_consts["Laser"]				= 6;
		m_list_of_type_consts["LaserWall"]			= 7;
		m_list_of_type_consts["Shockwave"]			= 8;
		m_list_of_type_consts["Line"]				= 9;
		m_list_of_type_consts["ElectricalCannon"]	= 10;
		m_list_of_type_consts["Lighting"]			= 11;
		m_list_of_type_consts["Smoke"]				= 12;
		m_list_of_type_consts["Kilwater"]			= 13;
		m_list_of_type_consts["Blood"]				= 14;


		m_list_of_lighttype_consts["Const"]			= 1;
		m_list_of_lighttype_consts["Pyramid"]		= 2;
		m_list_of_lighttype_consts["Trapezium"]		= 3;
		m_list_of_lighttype_consts["Random"]		= 4;
	}


	if (m_prt_file_version_info.dynamic_particle_version == e2150_old_dynamic_particle)
	{
		m_list_of_type_consts[""]					= 0;
		m_list_of_type_consts["Explosion"]			= 1;
		m_list_of_type_consts["Track"]				= 2;
		m_list_of_type_consts["ScaleableObject"]	= 3;
		m_list_of_type_consts["MappedExplosion"]	= 4;
		m_list_of_type_consts["FlatExplosion"]		= 5;
		m_list_of_type_consts["Laser"]				= 6;
		m_list_of_type_consts["LaserWall"]			= 7;
		m_list_of_type_consts["Shockwave"]			= 8;
		m_list_of_type_consts["Line"]				= 9;
		m_list_of_type_consts["Sphere"]				= 10;
		m_list_of_type_consts["ElectricalCannon"]	= 11;
		m_list_of_type_consts["Lighting"]			= 12;
		m_list_of_type_consts["Smoke"]				= 13;
		m_list_of_type_consts["Kilwater"]			= 14;
		m_list_of_type_consts["Blood"]				= 15;

		m_list_of_lighttype_consts["Const"]			= 0;
		m_list_of_lighttype_consts["Pyramid"]		= 1;
		m_list_of_lighttype_consts["Trapezium"]		= 2;
		m_list_of_lighttype_consts["Random"]		= 3;

	}

}


DynamicParticleClass::DynamicParticleClass(ParticleFileVersionInfo arg_prt_file_version_info,
										   string arg_input_directory_name,
										   vector<char> arg_clean_file_buffer) :
	
	m_prt_file_version_info(arg_prt_file_version_info),
	m_input_directory_name(arg_input_directory_name)
{
	if (arg_clean_file_buffer.empty() == false)
	{
		m_dynamic_particle_file_stream = stringstream(&arg_clean_file_buffer[0]);

		InitializeHashMaps();
	}
}

void DynamicParticleClass::GetFromFile(string& arg_line)
{
	const int introduction_number_of_lines = 3;

	// 1//DynamicParticle
	// 2//{
	// 3//}

	AU::MoveByLines(m_dynamic_particle_file_stream, introduction_number_of_lines);

	getline(m_dynamic_particle_file_stream, arg_line);

	GetWholeObject(m_dynamic_particle_data, arg_line);
}


void DynamicParticleClass::GetSingleObject(DynamicParticleData& arg_dynamic_particle_data, string& arg_line)
{
	const string object_string = "Object ";

	if (arg_line.substr(0, object_string.length() ) == object_string)
	{
		DynamicParticleData child;

		GetWholeObject(child, arg_line);

		arg_dynamic_particle_data.children.push_back( move(child) );

		++arg_dynamic_particle_data.number_of_nested_particles;
	}

}

void DynamicParticleClass::GetTime(DynamicParticleData& arg_dynamic_particle_data, string& arg_line)
{
	const string time_string = "Time ";
	const size_t time_string_len = time_string.length();

	if (arg_line.substr(0, time_string_len) == time_string)
	{
		float help_time_1, help_time_2;

		string m_help_str = arg_line.substr(time_string_len, arg_line.length() - time_string_len);

		sscanf_s(m_help_str.c_str(), "%f, %f", &help_time_1, &help_time_2);

		DEBUG_PRINT("Time = ");
		DEBUG_PRINT(to_string(help_time_1).c_str());
		DEBUG_PRINT(", ");
		DEBUG_PRINT(to_string(help_time_2).c_str());
		DEBUG_PRINT("\n");

		arg_dynamic_particle_data.time[0] = static_cast<int64_t>( help_time_1 * 1000.0 );
		arg_dynamic_particle_data.time[1] = static_cast<int64_t>( (1.0 - help_time_2) * 1000.0 );

	}

}


void DynamicParticleClass::GetWholeObject(DynamicParticleData& arg_dynamic_particle_data, string& arg_line)
{
	
	AU::GetSingleString(arg_line, "Object", arg_dynamic_particle_data.m_obj_name);

	//cout << arg_dynamic_particle_data.m_obj_name << endl;

	AU::MoveByLines(m_dynamic_particle_file_stream, 1);

	while(true)
	{
		getline(m_dynamic_particle_file_stream, arg_line);

		if (arg_line.substr(0, 1) == "}")
		{
			break;
		}

		GetSingleObject(arg_dynamic_particle_data, arg_line);

		AU::GetSingleStringAndInterpretConst(arg_line, "Type", arg_dynamic_particle_data.type, m_list_of_type_consts);
		AU::GetSingleStringAndInterpretConst(arg_line, "LightType", arg_dynamic_particle_data.light_type, m_list_of_lighttype_consts);
		AU::GetOneValue<uint32_t>(arg_line, "Additive", "%d", arg_dynamic_particle_data.additive);

		AU::GetTabValues<int32_t>(arg_line, "Frames", "%d", arg_dynamic_particle_data.frames, arg_dynamic_particle_data.frames_size);
		// /\ Dodatkowe obliczenia w metodzie CalculateImportantValues...

		AU::GetTabValues<float>(arg_line, "Light", "%f", arg_dynamic_particle_data.light, arg_dynamic_particle_data.light_size);
		// /\ Dodatkowe obliczenia w metodzie CalculateImportantValues...

		AU::GetTabValues<float>(arg_line, "Size", "%f", arg_dynamic_particle_data.size, arg_dynamic_particle_data.size_size);
		// /\ Dodatkowe obliczenia w metodzie CalculateImportantValues...

		AU::GetTabValues<float>(arg_line, "Size2", "%f", arg_dynamic_particle_data.size_2, arg_dynamic_particle_data.size_2_size);

		AU::GetTabValues<float>(arg_line, "Color", "%f", arg_dynamic_particle_data.color, arg_dynamic_particle_data.color_size);

		AU::GetTwoTypeTabValues<float, arg_dynamic_particle_data.alpha_a_size, uint32_t, arg_dynamic_particle_data.alpha_b_size>
							   (arg_line, "Alpha", "%f", "%d", arg_dynamic_particle_data.alpha);

		AU::GetTabValues<float>(arg_line, "Scale", "%f", arg_dynamic_particle_data.scale, arg_dynamic_particle_data.scale_size);
		 
		AU::GetTabValues<float>(arg_line, "Position", "%f", arg_dynamic_particle_data.position, arg_dynamic_particle_data.position_size);
		// /\ Dodatkowe obliczenia w metodzie CalculateImportantValues...
		
		AU::GetTabValues<float>(arg_line,
							    "Position2", 
								"%f", 
								arg_dynamic_particle_data.position_2, 
								arg_dynamic_particle_data.position_2_size);
		// /\ Dodatkowe obliczenia w metodzie CalculateImportantValues...

		GetTime(arg_dynamic_particle_data, arg_line);
		 
		AU::GetSingleStringFromQueue(arg_line, "Texture", arg_dynamic_particle_data.texture, GlobalQueues::m_dynamic_particle_Texture_q);
		AU::GetSingleStringFromQueue(arg_line, "Mesh", arg_dynamic_particle_data.mesh, GlobalQueues::m_dynamic_particle_Mesh_q);

		AU::GetOneValue<uint32_t>(arg_line, "Layer", "%d", arg_dynamic_particle_data.layer);

		DEBUG_PRINT("\n");
		DEBUG_PRINT("-----------------------\n");
		DEBUG_PRINT(arg_line.c_str()); DEBUG_PRINT("\n");
		DEBUG_PRINT("-----------------------\n");
		DEBUG_PRINT("\n");

	}
	
}


void DynamicParticleClass::WriteToFile(vector<char>& output_file_buff)
{
	WriteDynamicParticleDataToFileBuffer(output_file_buff, m_dynamic_particle_data);
}

void DynamicParticleClass::CalculateImportantValues(DynamicParticleData& arg_dynamic_particle_data)
{
	//---------------------------------------
	// frames ratio:
	arg_dynamic_particle_data.frames[1] = arg_dynamic_particle_data.frames[1] - arg_dynamic_particle_data.frames[0] + 1;


	if (arg_dynamic_particle_data.frames[3] != 0)
	{
		arg_dynamic_particle_data.frames_ratio[0] =
			static_cast<float>(1.0 / static_cast<double>(arg_dynamic_particle_data.frames[3]));
	}
	else
	{
		arg_dynamic_particle_data.frames_ratio[0] = 0;
	}

	if (arg_dynamic_particle_data.frames[4] != 0)
	{
		arg_dynamic_particle_data.frames_ratio[1] =
			static_cast<float>(1.0 / static_cast<double>(arg_dynamic_particle_data.frames[4]));
	}
	else
	{
		arg_dynamic_particle_data.frames_ratio[1] = 0;
	}


	//---------------------------------------
	//Light:
	arg_dynamic_particle_data.light[0] *= arg_dynamic_particle_data.light[3]; // Light_1 = Light_1 * Light_4
	arg_dynamic_particle_data.light[1] *= arg_dynamic_particle_data.light[3]; // Light_2 = Light_2 * Light_4
	arg_dynamic_particle_data.light[2] *= arg_dynamic_particle_data.light[3]; // Light_3 = Light_3 * Light_4

	//---------------------------------------
	// Delta Size:

	arg_dynamic_particle_data.delta_size =
		static_cast<float>((arg_dynamic_particle_data.size[2] - arg_dynamic_particle_data.size[0]) * 0.5);

	//---------------------------------------
	// Position:

	arg_dynamic_particle_data.position[1] *= -1.0;

	//---------------------------------------
	// Position2:
	arg_dynamic_particle_data.position_2[1] *= -1.0;
	//---------------------------------------
}


void DynamicParticleClass::WriteDynamicParticleDataToFileBuffer(vector<char>& output_file_buff, 
															    DynamicParticleData& arg_dynamic_particle_data)
{
	CalculateImportantValues(arg_dynamic_particle_data);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.type);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.light_type);

	BW::WriteTabViaIndexes(output_file_buff, arg_dynamic_particle_data.frames, { 0, 1, 3, 4, 2 });

	if (m_prt_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
	{
		BW::WriteValue<int32_t>(output_file_buff, static_cast<int32_t>(arg_dynamic_particle_data.time[0]));
		BW::WriteValue<int32_t>(output_file_buff, static_cast<int32_t>(arg_dynamic_particle_data.time[1]));
	}

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.frames_ratio);

	BW::WriteTabFromTo(output_file_buff, arg_dynamic_particle_data.size, 0, arg_dynamic_particle_data.size_size - 1 - 1);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.size_2);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.size[4]);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.delta_size);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.null_space);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.additive);

	BW::WriteTabFromTo(output_file_buff, arg_dynamic_particle_data.light, 0, arg_dynamic_particle_data.light_size - 1 - 1);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.color);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.alpha.b[0]);

	BW::WriteTabFromToReverse(output_file_buff, arg_dynamic_particle_data.alpha.a, arg_dynamic_particle_data.alpha_a_size - 1, 0);

	BW::WriteTabFromToReverse(output_file_buff, arg_dynamic_particle_data.scale, arg_dynamic_particle_data.scale_size - 1, 0);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.position);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.position_2);

	BW::WriteString(output_file_buff, arg_dynamic_particle_data.mesh);

	BW::WriteString(output_file_buff, arg_dynamic_particle_data.texture);

	BW::WriteValue(output_file_buff, arg_dynamic_particle_data.number_of_nested_particles);

	// Rekurencyjne wczytywanie dzieci
	for (uint32_t i = 0; i < arg_dynamic_particle_data.number_of_nested_particles; ++i)
	{
		string child_extra_data_file_name = m_input_directory_name + "\\" + arg_dynamic_particle_data.children[i].m_obj_name + "_extra_data.cpp";

		//cout << child_extra_data_file_name << endl;


		IntroductionHeaderClass my_introduction_header(m_prt_file_version_info, child_extra_data_file_name, cout);
		my_introduction_header.GetAndWriteToFile(output_file_buff, true);

		WriteDynamicParticleDataToFileBuffer(output_file_buff, arg_dynamic_particle_data.children[i]);
	}


	if (m_prt_file_version_info.dynamic_particle_version != e2150_old_dynamic_particle)
	{
		BW::WriteValue(output_file_buff, arg_dynamic_particle_data.layer);
	}
}