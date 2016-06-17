#include <iostream>


#include "Writer.hpp"


namespace Tester
{
  Writer::Writer( Tester::TesterDirector * tester_director, std::string sourcefile )
  {
    this->tester = tester_director;
    this->testfile_name = create_testfile_name( sourcefile );
  }

  Writer::Writer( std::string sourcefile )
  {
    this->sourcefile_name = sourcefile;
  }

  void Writer::write_testcases()
  {
    std::fstream source_stream;

    source_stream.open( sourcefile_name, WRITE );

    if( source_stream.is_open() )
    {
      std::cout << "OPEN file " << sourcefile_name << "\n";
    } else
    {
      std::cout << "Unable to open " << sourcefile_name << "..." << std::endl;
    }
  }

  void Writer::write_testfile()
  {
    std::fstream test_stream;

    test_stream.open( testfile_name, WRITE|APPEND );

    if( test_stream.is_open() )
    {
      test_stream << tester->get_testfile()->get_dependencies()
                  << tester->get_testfile()->get_test_class();

      for( int i = 0; i < tester->get_testfile()->get_valid_testcases()->size(); i++ )
      {
        test_stream << (* tester->get_testfile()->get_valid_testcase( i ) )
                    << std::endl;
      }

      test_stream.close();
    } else
    {
      std::cout << "Unable to open TESTFILE..." << std::endl;
    }
  }

  std::string Writer::create_testfile_name( std::string sourcefile )
  {
    size_t found = sourcefile.find( "." );
    std::string name = sourcefile.substr( 0, found );
    std::string extension = sourcefile.substr( found );

    std::string testfile( name );
    testfile.append( "Tests" );
    testfile.append( extension );

    return testfile;
  }
}
