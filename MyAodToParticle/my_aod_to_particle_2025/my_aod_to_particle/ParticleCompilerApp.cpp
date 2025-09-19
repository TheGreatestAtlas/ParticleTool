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
	m_in_stream >> this->m_input_directory_path;

}

string ParticleCompilerApp::CreateOutputFileName(const particle_type_value& version)
{
	string dir_name = this->m_input_directory_path.stem().string();

	switch (version)
	{
	case dynamic_particle:
	case ks_particles_emiter:
		return (this->m_input_directory_path.parent_path() / (dir_name + ".msh" )).string();
	break;

	case e2160_particle:
	case two_worlds_particle:
	case particle_gen_particle:
		return (this->m_input_directory_path.parent_path() / (dir_name + ".prt")).string();
	break;

	case not_particle:
		return (this->m_input_directory_path.parent_path() / (dir_name + ".error")).string();
	break;

	default: 
		return string(); 
	break;
	}
}




ParticleCompilerApp::ParticleCompilerApp(istream& argin, ostream& argo, int my_argc, char* my_argv[]) :
										m_in_stream(argin), m_o(argo),

	m_input_directory_path(fs::path())
{
	if (my_argc == c_correct_number_of_args)
	{
		this->m_input_directory_path = *(my_argv + 1);
		m_o << "Directory = " << this->m_input_directory_path << std::endl;
	}
	else
	{
		StandardProgramExecution();
	}

	AodCompilator my_aod_compilator(m_input_directory_path);
	my_aod_compilator.CompileTxtFileToBinBuffer();

	string m_output_file_path = CreateOutputFileName( my_aod_compilator.GetParticleVersion() );

	my_aod_compilator.SaveOutputFileBufferToBinFile(m_output_file_path);

	m_o << endl << "-----------------------------------------------------" 
		<< endl << "The directory named " << m_input_directory_path << " was successfully compiled..." << endl;
}
