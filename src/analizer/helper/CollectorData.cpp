#include "CollectorData.hpp"

namespace Helper
{
// ------------------ PUBLIC FUNCTIONS IMPLEMENTATION -------------------

  void CollectorData::collect_data( const char * format, ... )
  {
    va_list arguments;
    va_start( arguments, format );

    while( *format != EOL )
    {
      switch( *format )
      {
        case PACKAGE:
          this->set_package_name( va_arg( arguments, char * ) );
          break;
        case CLASS:
          this->set_class_name( va_arg( arguments, char * ) );
          break;
        case METHOD:
          this->set_method_name( va_arg( arguments, char * ) );
          break;
      }

      format++;
    }
  }

  Helper::Data* CollectorData::get_data()
  {
    return &data;
  }

// ------------------ PRIVATE FUNCTIONS IMPLEMENTANTION --------------------

  void CollectorData::set_package_name( std::string name )
  {
    this->data.package_name = name;
  }

  void CollectorData::set_class_name( std::string name )
  {
    this->data.class_name = name;
  }

  void CollectorData::set_method_name( std::string name )
  {
    Helper::Method method;
    method.name = name;

    this->data.methods.push_back( method );
  }

  std::string CollectorData::get_package_name()
  {
    return this->data.package_name;
  }

  std::string CollectorData::get_class_name()
  {
    return this->data.class_name;
  }

  std::vector<Helper::Method> CollectorData::get_methods()
  {
    return this->data.methods;
  }
}