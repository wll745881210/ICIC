#ifndef CELL_INT2D_H_
#define CELL_INT2D_H_

#include <map>
#include <list>
#include <vector>
#include <queue>
#include <string>

class cell_int2d
{
public:
	cell_int2d(  );
	~cell_int2d(  );

	void read_data(  );
	void set_io_file( std::string input_file,
					  std::string output_file );
	void set_sqr_scale( double ddec, double dra );
	void cal_cic(  );
	void output(  );
	
private:
	std::string input_file, output_file;
	
	typedef std::pair<double, int> di_pair;
	typedef std::pair<double, double> dd_pair;
	struct sqr_element;

	double ddec;
	double dra;

	std::multimap<double, double> main_tree;
	std::multimap<double, int> sub_tree;

	double sweep_line_dec;
	std::queue<sqr_element> sqr_ele_q;
	std::vector<double> int_1d_mem;
	void int_1d( const double & new_line_dec );
	
	void int_2d_main_body(  );
	void int_2d_finalize(  );
};

#endif
