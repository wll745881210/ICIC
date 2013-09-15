#include "cell_int2d.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#define DEG_TO_RAD 0.017453292519943295

struct cell_int2d::sqr_element
{
    double dec_upper_bound;
    std::multimap<double,int>::iterator p_ra_left;
    std::multimap<double,int>::iterator p_ra_right;
};


cell_int2d::cell_int2d(  )
{
    main_tree.clear(  );
    sub_tree.clear(  );
    int_1d_mem.clear(  );
}

cell_int2d::~cell_int2d(  ){  }

void cell_int2d::set_io_file( std::string input_file,
                              std::string output_file )
{
    this->input_file  = input_file;
    this->output_file = output_file;
    return;
}

void cell_int2d::read_data(  )
{
    std::ifstream fin( input_file.c_str(  ) );
    if( !fin )
        throw "Unable to open data file.";

    dd_pair radec;

    while( !fin.eof(  ) )
    {
        fin >> radec.second >> radec.first;
        fin.ignore( 64, '\n' );
        main_tree.insert( radec );
    }

    std::cout << "Main tree generated." << std::endl;

    return;
}

void cell_int2d::set_sqr_scale( double ddec,
                                double dra )
{
    this->ddec = ddec;
    this->dra  = dra;
    return;
}

void cell_int2d::int_1d( const double &
                         new_line_dec  )
{
    if( sub_tree.empty(  ) )
        return;

    std::multimap<double, int>::iterator sub_itr
        = sub_tree.begin(  );

    int i = sub_itr->second;
    double ra_last = sub_itr->first;
    double ra_now( 0. );
    double ra_diff( 0. );
    ++ sub_itr;

    const double dec_diff
        = new_line_dec - sweep_line_dec;
    const double cos_dec
        = cos( sweep_line_dec * DEG_TO_RAD );
    const double ra_max = sub_tree.rbegin( )->first;
    const double ra_min = sub_tree.begin(  )->first;
    const double ra_bound_width = dra / cos_dec;

    while( sub_itr != sub_tree.end(  ) )
    {
        ra_now  = sub_itr->first;
        ra_diff = ra_now - ra_last;
        ra_last = ra_now;


        if( i + 1 > int_1d_mem.size(  ) )
            int_1d_mem.resize( i + 1, 0. );
        if( ra_now > ra_min + ra_bound_width
            && ra_now < ra_max - ra_bound_width )
            int_1d_mem[ i ] += ra_diff
                * cos_dec * dec_diff;

        i += sub_itr->second;
        if( i < 0 )
            throw "Negative overlap number! ";
        ++ sub_itr;
    }
    return;
}

void cell_int2d::int_2d_main_body(  )
{
    std::multimap<double, double>::iterator main_itr
        = main_tree.begin(  );

    sqr_element sqr_ele_this;
    double dec_this( 0. ), ra_this( 0. );
    double dec_temp( 0. );
    di_pair di_temp;

    const double dec_max = main_tree.rbegin( )->first;
    const double dec_min = main_tree.begin(  )->first;
    const double dec_bound_width = ddec;

    double ra_width( 0. );

    sweep_line_dec = dec_min - 0.5 * ddec;

    while( main_itr != main_tree.end(  ) )
    {
        dec_this = main_itr->first - 0.5 * ddec;
        ra_this  = main_itr->second;

        sqr_ele_this.dec_upper_bound
            = dec_this + ddec;

        ra_width = dra / cos( dec_this * DEG_TO_RAD );

        di_temp.first  = ra_this - 0.5 * ra_width;
        di_temp.second = 1;
        sqr_ele_this.p_ra_left
            = sub_tree.insert( di_temp );

        di_temp.first  = ra_this + 0.5 * ra_width;
        di_temp.second = -1;
        sqr_ele_this.p_ra_right
            = sub_tree.insert( di_temp );

        sqr_ele_q.push( sqr_ele_this );

        dec_temp =  sqr_ele_q.front().dec_upper_bound;

        while( dec_temp < dec_this
               && !sqr_ele_q.empty(  ) )
        {
            if( dec_temp > dec_min + dec_bound_width
                && dec_temp    < dec_max - dec_bound_width )
                int_1d( dec_temp );

            sub_tree.erase( sqr_ele_q.front().p_ra_left );
            sub_tree.erase( sqr_ele_q.front().p_ra_right );

            sqr_ele_q.pop(  );
            sweep_line_dec = dec_temp;
            dec_temp =  sqr_ele_q.front().dec_upper_bound;
        }
        if( dec_this > dec_min + dec_bound_width
            && dec_this < dec_max - dec_bound_width )
            int_1d( dec_this );

        sweep_line_dec = dec_this;
        ++ main_itr;
    }

    return;
}

void cell_int2d::int_2d_finalize(  )
{
    const double dec_max = main_tree.rbegin( )->first;
    const double dec_min = main_tree.begin(  )->first;
    const double dec_bound_width = ddec;

    double dec_temp( 0. );

    while( !sqr_ele_q.empty(  ) )
    {
        dec_temp = sqr_ele_q.front().dec_upper_bound;
        if( dec_temp > dec_max - dec_bound_width )
            break;

        int_1d( dec_temp );
        sub_tree.erase( sqr_ele_q.front().p_ra_left );
        sub_tree.erase( sqr_ele_q.front().p_ra_right );
        sweep_line_dec
            = sqr_ele_q.front().dec_upper_bound;
        sqr_ele_q.pop(  );
    }

    std::cout << "Successfully constructed CIC."
              << std::endl;
    return;
}

void cell_int2d::cal_cic(  )
{
    int_2d_main_body(  );
    int_2d_finalize(  );
    return;
}

void cell_int2d::output(  )
{
    std::ofstream fout( output_file.c_str(  ) );

    double normalize( 0. );
    double average( 0. );

    for( unsigned i = 0; i < int_1d_mem.size(); ++ i )
        normalize += int_1d_mem[ i ];

    for( unsigned i = 0; i < int_1d_mem.size(); ++ i )
    {
        fout << i << '\t'
             << int_1d_mem[ i ] / normalize
             << std::endl;
        average += i * int_1d_mem[ i ] / normalize;
    }

    std::cout << "Average: " << average << std::endl;

    std::cout << "Angular area: " << normalize
              << std::endl;

    std::cout << "Output to file " << output_file
              << std::endl;
    return;
}
