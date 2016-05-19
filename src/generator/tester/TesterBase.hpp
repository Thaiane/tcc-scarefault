#ifndef TESTERBASE_HPP_
#define TESTERBASE_HPP_

#define TESTFILE "testfile.groovy"

#define WRITE std::fstream::out
#define APPEND std::fstream::app

#include <string>
#include <fstream>
#include <iostream>
#include <locale>
#include "../../analizer/helper/Data.hpp"

namespace Tester
{
  class TesterBase
  {
    public:
      void set_data( Helper::Data * );
      void write_data();

    private:
      Helper::Data * data_ptr;
      std::locale locale;

      void make_header( std::fstream * );
      void test_methods( std::fstream * );
      void make_test_create( std::fstream * );
      void make_test_show( std::fstream * );
      void make_test_delete( std::fstream * );
      void make_test_list( std::fstream * );
      void make_test_save( std::fstream * );
      void make_test_edit( std::fstream * );
      void make_test_update( std::fstream * );
      void conclude_data( std:: fstream * );

      std::string convert_to_lower( std::string );
  };
}

#endif // TESTERBASE_HPP_
