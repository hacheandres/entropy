#include "entropy.h"

Entropy::Entropy(){};
Entropy::~Entropy(){};

/**
 * @brief Calcula la entropia de un bloque
 * 
 * @param buf Bloque a analizar. Debe apuntar a un bloque de memoria de tamanio n o mayor
 * @param n Cantidad de bytes a analizar en buf.
 * @return float : El valor de entropia de dicho bloque.
 */
float Entropy::entropy_block( const char * buf, int n )
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

/**
 * @brief Calcula la entropia de un archivo filename, analizandolo por bloques 
 * de tamanio _blocksize.
 * 
 * @param filename Nombre del archivo a analizar
 * @param _ent_by_block Acá se guardarán los valores de entropia de cada block 
 * @param _blocksize Tamanio de bloque que se analizaran
 * @param _lowe Devuelve la cantidad de bloques con entropia < 2
 * @param _highe Devuelve la cantidad de bloques con entropia > 7
 * @return Entropy::status Retorna si se ejecuto correctamente o el motivo del fallo.
 */
Entropy::status Entropy::calculate( const string & filename, vector<float> & _ent_by_block , int _blocksize, int & _lowe, int & _highe)
{

	char * block = new char[_blocksize] ;	//Guardare los bytes leidos de a bloques
	int nread = 0 ;							//La cantidad de bytes leidos, por si lei menos de un bloque entero
	float ent = 0.0 ;
	
	ifstream input( filename , ios::binary);
	
	if( input.is_open() == false )
	{
		cerr << "Cannot open the file: " << filename << endl ;
		return Entropy::ERR_OPEN_FILE ;
	}

	if( block == NULL )
	{
		input.close() ;
		cerr << "Error: Cannot allocate " << _blocksize << " bytes in memory" << endl ;
		return Entropy::ERR_NO_MEMORY ;
	}

	input.read ( block , _blocksize );
	nread = input.gcount() ;

	while( nread > 0 )
	{
		ent = entropy_block( block , nread ) ;
		try {
			_ent_by_block.push_back( ent );
		}
		catch(const std::bad_alloc &) {
			cerr << "Memory error. Try with a bigger block size" << endl;
			delete block ;
			input.close() ;
			return ERR_BAD_ALLOC ; 
		}
		catch(...) {
			cerr << "Generic exception" << endl;
			delete block ;
			input.close() ;
			return ERROR ;
		}
		
		if( ent > 7 )
			_highe++;
		else if ( ent < 2)
			_lowe++ ;

		input.read ( block, _blocksize );
		nread = input.gcount() ;
	}

	input.close() ;
	delete block ;
	return OK ;
}






