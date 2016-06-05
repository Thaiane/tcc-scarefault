#include "CollectorBase.hpp"

namespace Collector
{
  ADDRESS address_collector;

// ------------------ PUBLIC FUNCTIONS IMPLEMENTATION -------------------
  CollectorBase::CollectorBase()
  {
    address_collector = this;
  }

  void CollectorBase::collect_data( const char * format, ... )
  {
    va_list arguments;
    va_start( arguments, format );

    std::vector<std::string> info_method;

    while( *format != EOL )
    {
      std::string collected_text;

      switch( *format )
      {
        case PACKAGE:
          this->set_package( va_arg( arguments, char * ) );
          break;
        case CLASS:
          this->set_class( va_arg( arguments, char * ) );
          break;
        case METHOD:
          collected_text.assign( va_arg( arguments, char * ) );
          info_method.push_back( collected_text );
          break;
        case PARAM:
          collected_text.assign( va_arg( arguments, char * ) );
          info_method.push_back( collected_text );
          break;
      }

      format++;
    }

    this->set_methods( info_method );
  }

  Collector::FileGrails* CollectorBase::get_data()
  {
    return &data;
  }

// ------------------ PRIVATE FUNCTIONS IMPLEMENTANTION --------------------

  void CollectorBase::set_package( std::string name )
  {
    this->data.package = name;
  }

  void CollectorBase::set_class( std::string name )
  {
    this->data.class_name = name;

    identify_category( name );
  }

  void CollectorBase::set_methods( std::vector<std::string> info )
  {
    if( !info.empty() )
    {
      Collector::Method method;
      method.name = info[ 0 ];

      method.params = collect_params( info );

      this->set_params_range( &method );
      this->data.methods.push_back( method );
    } else
    {
      // Nothing To do
    }
  }

  void CollectorBase::set_params_range( Collector::Method * method )
  {
    if( !method->params.empty() )
    {
      int index = this->collector_scarefault.get_params()->size()-1;

      while( !this->collector_scarefault.get_params()->empty() )
      {
        Collector::Param compared_param = collector_scarefault.get_param( index );

        for( int i = method->params.size()-1; i >= 0; i-- )
        {
          if( !method->params[ i ].name.compare( compared_param.name ) )
          {
            method->params[ i ].range = compared_param.range;
            this->collector_scarefault.get_params()->pop_back();
          } else
          {
            // Nothing to do
          }
        }

        index--;
      }
    } else
    {
      // Nothing to do
    }
  }

  std::vector<Collector::Param>
  CollectorBase::collect_params( std::vector<std::string> info )
  {
    std::vector<Collector::Param> params;

    if( info.size() > 1 )
    {
      std::string collected_params = info[ 1 ];

      while( !collected_params.empty() )
      {
        Collector::Param new_param = create_param( &collected_params );
        params.push_back( new_param );
      }
    } else
    {
      // Nothing To do
    }

    return params;
  }

  Collector::Param CollectorBase::create_param( std::string * text )
  {
    std::size_t comma_position = text->find( "," );

    Collector::Param new_param;

    if( comma_position != std::string::npos )
    {
      std::string found_param = text->substr( 0, comma_position );

      new_param = find_param( found_param );

      text->erase( text->begin(), text->begin()+comma_position+1 );
    } else
    {
      new_param = find_param( *text );

      text->clear();
    }

    return new_param;
  }

  Collector::Param CollectorBase::find_param( std::string text )
  {
    std::size_t blank_position = text.find( " " );
    std::string type = text.substr( 0, blank_position );
    std::string name = text.substr( blank_position+1 );

    Collector::Param new_param;

    new_param.name = name;
    new_param.type = type;

    return new_param;
  }

  void CollectorBase::identify_category( std::string name )
  {
    if( name.find( "Controller" ) != std::string::npos )
    {
      std::size_t found = name.find( "Controller" );
      std::string category = name.substr( found );
      std::string model_base = name.substr( 0, found );

      collect_proprieties( model_base );

      this->data.model_base = model_base;
      this->data.layer = category;
    } else
    {
      this->data.model_base = name;
      this->data.layer = "Domain";
    }
  }

  void CollectorBase::collect_proprieties( std::string domain )
  {
    std::string content_file = extract_content_file( domain );

    if( !content_file.empty() )
    {
      std::vector<std::string> content = extract_words( content_file, " ={" );

      for( int i = 0; i < content.size(); i++ )
      {
        Collector::Propriety new_propriety;

        if( Helper::is_type( content[ i ] ) )
        {
          new_propriety.name = content[ i+1 ];
          new_propriety.type = content[ i ];

          data.proprieties.push_back( new_propriety );
        }

        if( !content[i].compare( "constraints" ) )
        {
          collect_constraints( content_file );
        }
      }
    }
  }

  void CollectorBase::collect_constraints( std::string text )
  {
    std::size_t found = text.find( "constraints" );
    std::string sub = text.substr( found+11 );
    std::size_t bracket_found = sub.find( "}" );
    std::string substring = sub.substr( 0, bracket_found );

    std::vector<std::string> words = extract_words( substring, " ={:,(" );
    std::vector<Propriety> proprieties = data.proprieties;

    for( int i = 0; i < proprieties.size(); i++ )
    {
      if( !proprieties[ i ].name.compare( words[ 0 ] ) )
      {
        int k = 1;

        while( words[ k ].compare( ")" ) )
        {
          Collector::Constraint new_constraint;
          new_constraint.name = words[ k ];
          new_constraint.value = words[ k+1 ];

          data.proprieties[ i ].contraints.push_back( new_constraint );
          k += 2;
        }

          words.erase( words.begin(), words.begin()+k+1 );
      }
    }
  }

  std::string CollectorBase::extract_content_file( std::string domain )
  {
    std::fstream domain_stream;

    std::string domain_file( PATH_DOMAIN );
    domain_file.append( domain );
    domain_file.append( ".groovy" );

    domain_stream.open( domain_file, READ );
    std::string content;

    if( domain_stream.is_open() )
    {
      std::string line;

      while( std::getline( domain_stream, line ) )
      {
        content.append( line );
      }

      domain_stream.close();
    } else
    {
      std::cout << "Unable to open "<< domain_file << "..." << std::endl;
    }

    return content;
  }

  std::vector<std::string>
  CollectorBase::extract_words( std::string text, std::string delimiters )
  {
    char * cdelimiters = Helper::convert_string_to_cstring( delimiters );
    char * ctext = Helper::convert_string_to_cstring( text );

    std::vector<std::string> words;
    char * phrase = strtok( ctext, cdelimiters );

    while( phrase != NULL )
    {
      std::string token( phrase );
      words.push_back( token );
      phrase = strtok( NULL, cdelimiters );
    }

    delete[] cdelimiters;
    delete[] ctext;
    delete[] phrase;

    return words;
  }
}