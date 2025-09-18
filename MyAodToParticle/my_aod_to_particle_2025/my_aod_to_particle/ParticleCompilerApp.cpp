#include "my_aod_to_particle.hpp"

//--------------------------------
// Metody Klasy Particle Compiler:
//--------------------------------

void ParticleCompilerApp::ShowStartScreen()
{
	m_o << "**************************************" << std::endl
		<< "|MYAOD TO PARTICLE BY ATLAS AND IMBEE|" << std::endl
		<< "*****************2025*****************" << std::endl;
}

void ParticleCompilerApp::StandardProgramExecution()
{
	//MAIN MENU
	ShowStartScreen();
	m_o << "Specify the name of the input directory:" << std::endl;
	m_in_stream >> m_input_directory_name;

}

string ParticleCompilerApp::CreateOutputFileName(const particle_type_value& version) const
{
	switch (version)
	{
	case dynamic_particle:
	case ks_particles_emiter:
		return m_input_directory_name + ".msh";
	break;

	case e2160_particle:
	case two_worlds_particle:
	case particle_gen_particle:
		return m_input_directory_name + ".prt";
	break;

	case not_particle:
		return m_input_directory_name + ".error";
	break;

	default: 
		return string(); 
	break;
	}
}




ParticleCompilerApp::ParticleCompilerApp(istream& argin, ostream& argo, int my_argc, char* my_argv[]) :
										m_in_stream(argin), m_o(argo),

	m_input_directory_name(string())
{
	if (my_argc == c_correct_number_of_args)
	{
		m_input_directory_name = *(my_argv + 1);
		m_o << "Directory = " << m_input_directory_name << std::endl;
	}
	else
	{
		StandardProgramExecution();
	}

	AodCompilator my_aod_compilator(m_input_directory_name);
	my_aod_compilator.CompileTxtFileToBinBuffer();

	string m_output_file_name = CreateOutputFileName( my_aod_compilator.GetParticleVersion() );

	my_aod_compilator.SaveOutputFileBufferToBinFile(m_output_file_name);

	m_o << endl << "-----------------------------------------------------" 
		<< endl << "The directory named " << m_input_directory_name << " was successfully compiled..." << endl;
}
