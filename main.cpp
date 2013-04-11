#include "cell_int2d.h"
#include "input.h"

#include <iostream>
#include <string>


int main( int argn, char * argv[  ] )
{
	try
	{
		std::string input_par;
		if( argn < 2 )
			input_par = "par.txt";
		else if( argn == 2 )
			input_par = argv[ 1 ];
		else
			throw "Incorrect number of arguments.";

		input input_2d( input_par );
		input_2d.read(  );

		cell_int2d cic_calc_2d;
		input_2d.init_obj( cic_calc_2d );

		cic_calc_2d.read_data(  );
		cic_calc_2d.cal_cic(  );
		cic_calc_2d.output(  );
	}
	catch( const char * Err )
	{
		std::cerr << "Error: " << Err
				  << std::endl;
	}
	
	return 0;
}
