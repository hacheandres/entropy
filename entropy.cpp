#include <iostream>
#include <string>
#include <sstream>

using namespace std ; 
#define BLOCKSIZE_DEFAULT 1024

bool arguments_process( int _argc, char *_argv[] , int & _blocksize , string & _filename )
{
	bool ret = false ;

	if( _argv == NULL )
		return false ;

	//Si solo ingreso 2 argumentos, solo proceso el archivo y blocksize queda con valor default
	if( _argc == 2 )
	{
		_blocksize = BLOCKSIZE_DEFAULT ;
		_filename = _argv[1] ;
		ret = true ;
	}
	else if( _argc == 4 )
	{
		/* Si ingreso exactamente 4 argumentos, tambien me deberia haber ingresado el 
		tamanio de bloque */
		
		_filename = _argv[3] ;	//El ultimo argumento es el nombre del archivo.

		string blockarg( _argv[1] );
		if( blockarg == "-b")	//Valido que efectivamente me ingreso el argumento de tamanio de bloque
		{
			stringstream  number( _argv[2] );
			number >> _blocksize ;

			if( _blocksize > 0)	 
			{
				ret = true ;
			}
			else
			{
				/* Si el tamanio es menor o igual a 0, solo por precaucion seteo el blocksize 
				con su valor por defecto y devuelvo false para informar que hubo un error */
				cerr << "Error: Invalid block size : "
				 	<< _blocksize << endl ;
				_blocksize = BLOCKSIZE_DEFAULT ;
				ret = false ;
			}
		}
	}
	else 
	{
		cerr << "Error: invalid arguments quantity" << _argc << endl ;
	}

	if( ret == false )
	{
		cout << "Error parsing arguments" << endl ;
		cout << "entropy [ -b <blocksize> ] <filename>" << endl ;
		cout << "<filename> is the path of the file to analyze" << endl ;
		cout << "-b <blocksize> optional argument that specifies the block-size in bytes." << endl ;
		cout << "The default block-size is 1024 bytes." << endl ;
		return false ;
	}

	return ret ;
}

int main( int argc, char* argv[])
{
	int blocksize = 0 ; 
	string filename = ""; 

	if( arguments_process( argc, argv, blocksize , filename ) == false )
	{
		cerr << "Error: Wrong arguments" << endl ;
		return -1 ;
	}

	cout << filename << " " << blocksize << endl ;
	
	cout << "Hi Disarmista" << endl ;
	return 0 ;
}





