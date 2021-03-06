#include "TestfileGrailsBuilder.hpp"
#include "../../helper/Helper.hpp"


namespace Tester
{
  void TestfileGrailsBuilder::build_dependencies()
  {
    std::stringstream stream;
    stream << this->build_package() << this->build_imports();
    this->testfile->set_dependencies( stream.str() );
  }

  std::string TestfileGrailsBuilder::create_params()
  {
    std::stringstream stream;

    for( int i = 0; i < data_ptr->proprieties.size(); i++ )
    {
      Collector::Propriety propriety = data_ptr->proprieties[ i ];

      if( propriety.name.compare( "dateCreated") &&
          propriety.name.compare( "lastUpdated") )
      {
        std::string value = create_value( propriety );

        stream << "  params[\""
               << propriety.name
               << "\"] = "
               << value
               << std::endl;
      } else
      {
        // Nothing to do
      }
    }

    return stream.str();
  }

  std::string TestfileGrailsBuilder::create_value( Collector::Propriety propriety )
  {
    std::string value( "VALID VALUE" );
    std::vector<Collector::Constraint> constraints = propriety.contraints;

    if( is_string( propriety ) )
    {
      value = create_string( propriety );
    } else if( is_integer( propriety ) )
    {
      value = create_integer( propriety );
    } else if( is_floating( propriety ) )
    {
      value = create_floating( propriety );
    } else if( is_boolean( propriety ) )
    {
      value = value_generator->generate_boolean();
    } else
    {
      // Nothing to do
    }

    return value;
  }

  std::string TestfileGrailsBuilder::create_string( Collector::Propriety propriety )
  {
    clear_constraints();

    for( int i = 0; i < propriety.contraints.size(); i++ )
    {
      Collector::Constraint constraint = propriety.contraints[ i ];

      for( int type = 0; type < type_constraint.size(); type++ )
      {
        if( !constraint.name.compare( type_constraint[ type ] ) )
        {
          switch( type )
          {
            case BLANK:
              constraints[ BLANK ] = convert_to_bool( constraint.value );
              break;
            case CREDIT_CARD:
              constraints[ CREDIT_CARD ] = convert_to_bool( constraint.value );
              break;
            case EMAIL:
              constraints[ EMAIL ] = convert_to_bool( constraint.value );
              break;
            case NULLABLE:
              constraints[ NULLABLE ] = convert_to_bool( constraint.value );
              break;
            case SIZE:
              extract_size( constraint.value );
              break;
            case URL:
              constraints[ URL ] = convert_to_bool( constraint.value );
              break;
          }
        }
      }
    }

    return value_generator->generate_string( constraints );
  }

  std::string TestfileGrailsBuilder::create_integer( Collector::Propriety propriety )
  {
    clear_constraints();

    for( int i = 0; i < propriety.contraints.size(); i++ )
    {
      Collector::Constraint constraint = propriety.contraints[ i ];

      for( int type = 0; type < type_constraint.size(); type++ )
      {
        if( !constraint.name.compare( type_constraint[ type ] ) )
        {
          switch( type )
          {
            case NULLABLE:
              constraints[ NULLABLE ] = convert_to_bool( constraint.value );
              break;
            case MAX:
              constraints[ MAX ] = std::stoi( constraint.value );
              break;
            case MIN:
              constraints[ MIN ] = std::stoi( constraint.value );
              break;
            case RANGE:
              extract_range( constraint.value );
              break;
          }
        }
      }
    }

    return value_generator->generate_integer( constraints );
  }

  std::string TestfileGrailsBuilder::create_floating( Collector::Propriety propriety )
  {
    clear_constraints();

    for( int i = 0; i < propriety.contraints.size(); i++ )
    {
      Collector::Constraint constraint = propriety.contraints[ i ];

      for( int type = 0; type < type_constraint.size(); type++ )
      {
        if( !constraint.name.compare( type_constraint[ type ] ) )
        {
          switch( type )
          {
            case NULLABLE:
              constraints[ NULLABLE ] = convert_to_bool( constraint.value );
              break;
            case MAX:
              constraints[ MAX ] = std::stoi( constraint.value );
              break;
            case MIN:
              constraints[ MIN ] = std::stoi( constraint.value );
              break;
            case SCALE:
              constraints[ SCALE ] = std::stoi( constraint.value );
              break;
            case RANGE:
              extract_range( constraint.value );
              break;
          }
        }
      }
    }

    return value_generator->generate_floating( constraints );
  }

  bool TestfileGrailsBuilder::is_string( Collector::Propriety propriety )
  {
    return Helper::is_string( propriety.type );
  }

  bool TestfileGrailsBuilder::is_integer( Collector::Propriety propriety )
  {
    return Helper::is_integer( propriety.type );
  }

  bool TestfileGrailsBuilder::is_floating( Collector::Propriety propriety )
  {
    return Helper::is_floating( propriety.type );
  }

  bool TestfileGrailsBuilder::is_boolean( Collector::Propriety propriety )
  {
    return Helper::is_boolean( propriety.type );
  }

  bool TestfileGrailsBuilder::convert_to_bool( std::string text )
  {
    return ( text == "true" ) ? true : false;
  }

  void TestfileGrailsBuilder::extract_size( std::string text )
  {
    int range_op_position = text.find( ".." );
    std::string min_string = text.substr( 0, range_op_position );
    std::string max_string = text.substr( range_op_position+2 );

    constraints[ MIN_SIZE ] = std::stoi( min_string );
    constraints[ MAX_SIZE ] = std::stoi( max_string );
  }

  void TestfileGrailsBuilder::extract_range( std::string text )
  {
    int range_op_position = text.find( ".." );
    std::string min_string = text.substr( 0, range_op_position );
    std::string max_string = text.substr( range_op_position+2 );

    constraints[ MIN ] = std::stoi( min_string );
    constraints[ MAX ] = std::stoi( max_string );
  }

  void TestfileGrailsBuilder::clear_constraints()
  {
    int QTD_BOOLEAN_CONSTRAINTS = 6;

    for( int i = 0; i < QTD_BOOLEAN_CONSTRAINTS; i++ )
    {
      constraints[ i ] = false;
    }

    constraints[ MIN_SIZE ] = 1;
    constraints[ MAX_SIZE ] = 25;
    constraints[ MIN ] = 1;
    constraints[ MAX ] = 999;
    constraints[ SCALE ] = 2;
  }
}
