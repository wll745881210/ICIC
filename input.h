#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <string>
#include <vector>
#include "cell_int2d.h"

class input
{
public:
    input(  );
    input( const std::string & input_par_file );
    ~input(  );

    void read(  );
    void init_obj( cell_int2d & cic_2d );

private:
    std::ifstream fin;
    std::vector<std::string> item_name;
    std::vector<std::string> value;
    int length;
        
    void sort_item(  );
    void get_general(  );
};

#endif

