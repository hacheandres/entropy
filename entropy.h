#include <iostream>

#include <sstream>
#include <fstream>
#include <iterator>
#include <cmath>

#include <string>
#include <vector>

using namespace std ; 



class Entropy{

	public:
		typedef enum {
			OK = 0 ,
			ERROR, 
			ERR_NO_MEMORY,
			ERR_BAD_ALLOC,
			ERR_OPEN_FILE,
		} status ;

		Entropy();
		~Entropy();
		status calculate( const string & filename, vector<float> &, int, int &, int &);

	private:
		float entropy_block( const char * buf, int n );

};

