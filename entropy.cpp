#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cmath>


using namespace std ; 
#define BLOCKSIZE_DEFAULT 1024

bool arguments_process( int _argc, char *_argv[] , int & _blocksize , string & _filename );
float entropy_block( const std::vector<char> & , int );
void entropy_file( ifstream &, vector<float> &, int, int &, int &);

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

	ifstream input( filename , ios::binary);
	if( input.is_open() == false )
	{
		cerr << "Cannot open the file: " << filename << endl ;
		return -1 ;
	}

	std::vector<float> entropy_by_block ;
	int lowentropy = 0 , highentropy = 0;

	entropy_file( input, entropy_by_block, blocksize, lowentropy, highentropy );
	input.close() ;

	//Print report 
	cout << "Entropy report for file: " << filename << endl ;
	int nblocks = entropy_by_block.size(); 

	cout.precision(3);
	cout << "block# \t\tentropy" << endl ;
	
	for (int i = 0 ; i < nblocks ; i++ )
	{
		cout << i << "\t\t" << entropy_by_block[i] << endl ;
	}

	cout << "Low entropy blocks: " << lowentropy << endl ;
	cout << "High entropy blocks: " << highentropy << endl ;
 
	return 0 ;
}


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
				cerr << "Error: Invalid block size : " << _blocksize << endl ;
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


float entropy_block( const char * buf, int n )
{ 
	#define BYTES_MAX_VALUE 256 
	
	std::vector<int> bytes_count(BYTES_MAX_VALUE, 0) ;	//Llevare la cuenta de las apariciones de cada byte	
	float entropy = 0.0, prob = 0.0; 

	unsigned char byte = 0 ;
	for( int i = 0 ; i < n ; i++ )
	{
		byte = buf[i] ;
		/* Uso el valor del byte para indexar el vector bytes_count y así incrementar 
		el valor de dicha posición, de esta forma llevo la cuenta de las apariciones
		de cada byte */
		bytes_count[ byte ]++ ;
	} 

	for( int i = 0 ; i < BYTES_MAX_VALUE ; i++ )
	{
		if( bytes_count[i] != 0 )	//Si el byte no aparecio en la secuencia no aporta a la entropia
		{
			prob = (float) bytes_count[i] / n ;
			entropy -= prob * log2( prob ); 
		}
	}

	return entropy ;
}

void entropy_file( ifstream & _input, vector<float> & _ent_by_block , int _blocksize, int & _lowe, int & _highe)
{

	char * block = new char[_blocksize] ;	//Guardare los bytes leidos de a bloques
	int nread = 0 ;				//La cantidad de bytes leidos, por si lei menos de un bloque entero
	float ent = 0.0 ;

	if( block == NULL )
	{
		cerr << "Error: Cannot allocate " << _blocksize << " bytes in memory" << endl ;
		return ;
	}

	_input.read ( block , _blocksize );
	nread = _input.gcount() ;

	while( nread > 0 )
	{
		ent = entropy_block( block , nread ) ;
		try {
			_ent_by_block.push_back( ent );
		}
		catch(const std::bad_alloc &) {
			cerr << "Memory error. Try with a bigger block size" << endl;
			delete block ;
			return ; 
		}
		catch(...) {
			cerr << "Generic exception" << endl;
			delete block ;
			return ; 
		}
		
		if( ent > 7 )
			_highe++;
		else if ( ent < 2)
			_lowe++ ;

		_input.read ( block, _blocksize );
		nread = _input.gcount() ;
	}

	delete block ;
	
}

