#include "my_aod_to_particle.hpp"

//------------------------
// Input File Buffer Class
//------------------------

InputBinFile::InputBinFile(string argfilename, ostream& arg_o, string argmode) : 
	m_work_mode(argmode), 
	m_input_file_o(arg_o),
	m_file_name(argfilename), 
	m_is_opened(false),
	m_is_open_now(false), 
	m_file_length(0)
{
	DEBUG_PRINT("Nazwa pliku:");
	DEBUG_PRINT(m_file_name.c_str());
	DEBUG_PRINT("\n");

	OpenFile();

	if (m_is_open_now == true)
	{
		MeasureFile();
		PutFileToBuffer();
		m_input_file.close();
		m_is_open_now = m_input_file.is_open();
	}

}


void InputBinFile::OpenFile()
{
	if (m_work_mode == "binary")
	{
		m_input_file.open(m_file_name, ios::in | ios::binary | ios::ate);
	}

	if (m_work_mode == "text")
	{
		m_input_file.open(m_file_name, ios::in | ios::ate);
	}
	
	if ((m_work_mode != "text") && (m_work_mode != "binary"))
	{
		DEBUG_PRINT("You have entered an invalid mode in the constructor of the InputBinFile class.\n");
		DEBUG_PRINT("You wrote: ");
		DEBUG_PRINT(m_work_mode.c_str());
		DEBUG_PRINT("\nThis will be interpreted as text mode.\n");

		m_input_file.open(m_file_name, std::ios::in | ios::ate);
	}

	m_is_opened = m_input_file.is_open();
	m_is_open_now = m_is_opened;

}


void InputBinFile::MeasureFile()
{
	m_file_length = m_input_file.tellg();

	m_input_file_o << endl 
		           << "***************************************************"		<< endl
				   << "File name = " << m_file_name								<< endl
				   << "File len = " << m_file_length << " (DEC)"				<< endl
				   << "***************************************************"		<< endl << endl;

	m_input_file.seekg(0, ios::beg);
}


void InputBinFile::PutFileToBuffer()
{
	m_file_buffer.resize(m_file_length);
	m_input_file.read(&m_file_buffer[0], m_file_length);
}

bool InputBinFile::GetIsOpened() const { return m_is_opened; }
bool InputBinFile::GetIsOpenNow() const { return m_is_open_now; }
streamsize InputBinFile::GetFileLength() const { return m_file_length; }
string InputBinFile::GetFileName() const { return m_file_name; }



InputBinFile::~InputBinFile()
{
	m_file_buffer.clear();
}

void InputBinFile::Informations() const
{
	if (m_is_opened == 0)
	{
		m_input_file_o << std::endl
					   << "ERROR: The specified file could not be opened." << std::endl
					   << "-------------------------------------" << std::endl
					   << "Opening the file:" << m_file_name << std::endl
					   << "Was the file successfully opened?: " << m_is_opened << std::endl
					   << "Is the file currently open?: " << m_is_open_now << std::endl
					   << "-------------------------------------" << std::endl
					   << std::endl;
	}

}


void InputBinFile::ExitWhenFileDontExist() const
{
	if (m_is_opened == false && m_is_open_now == false)
	{
		m_input_file_o << "The specified file does not exist!" << std::endl;
		exit(EXIT_FAILURE);
	}
}