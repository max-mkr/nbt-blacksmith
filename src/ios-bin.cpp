#include "ios-bin.hpp"
#include <iomanip>
namespace blacksmith {
  std::ostream& operator<<(ostream& os, sbin& stream) {
    os << hex;
    for (size_t i = 0; i < stream.size(); i++) {
      os << setfill('0') << setw(2) << (int) stream.get() << " ";

      if ((i+1) % 8 == 0) os << " ";
      if ((i+1) % 16 == 0) os << "\n";
    }
    os << dec << setfill(' ');
    return os;
  }

  template<>
  std::ostream& operator<<(ostream& os, const PrimitiveTag<uint8_t>& t) {
    os << hex << "b \""<< t.name << "\": "
       << setfill('0') << setw(2) <<(int) t.payload
       << "\n" << dec << setfill(' ');
    return os;
  }

  template<>
  std::ostream& operator<<(std::ostream& os, const ArrayTag<uint8_t>& t) {
    os << "[b] \""<< t.name << "\": {\n" << hex;

    os << indent_manip::push;
    size_t i = 0;
    for (uint8_t val : t.payload) {
      i++;
      os << setfill('0') << setw(2) << (int)val << " ";
      if (i % 32 == 0) os << "\n";
    }
    if (i % 32 != 0) os << "\n";
    os << indent_manip::pop << "}\n" << dec << setfill(' ');
    return os;
  }

  template<>
  std::ostream& operator<<(std::ostream& os, const ArrayTag<int32_t>& t) {
    os << "[i] \""<< t.name << "\": {\n" << hex;

    os << indent_manip::push;
    size_t i = 0;
    for (int32_t val : t.payload) {
      i++;
      os << setfill('0') << setw(2) << val << " ";
      if (i % 16 == 0) os << "\n";
    }
    if (i % 16 != 0) os << "\n";
    os << indent_manip::pop << "}\n" << dec << setfill(' ');
    return os;
  }

  template<>
  std::ostream& operator<<(std::ostream& os, const ArrayTag<int64_t>& t) {
    os << "[l] \""<< t.name << "\": {\n" << hex;

    os << indent_manip::push;
    size_t i = 0;
    for (int64_t val : t.payload) {
      i++;
      os << setfill('0') << setw(2) << val << " ";
      if (i % 4 == 0) os << "\n";
    }
    if (i % 4 != 0) os << "\n";
    os << indent_manip::pop << "}\n" << dec << setfill(' ');
    return os;
  }
  
  template<>
  std::ostream& operator<<(std::ostream& os, const ListTag<uint8_t>& t) {
    os << "{i} \""<< t.name << "\": {\n" << hex;

    os << indent_manip::push;
    for (uint8_t val : t.payload)
      os << setfill('0') << setw(2) << (int)val << "\n";
  
    os << indent_manip::pop << "}\n" << dec << setfill(' ');
    return os;
  }
  
  ostream& operator<<(ostream& os, const CompoundTag& t) {
    os << "{%} \""<< t.name << "\": {\n" << hex;
    os << indent_manip::push;
    for (const shared_ptr<Tag>& nested : t.payload)
      os << nested;
    os << indent_manip::pop << "}\n";
    return os;
  }
  
  ostream& operator<<(ostream& os, const shared_ptr<Tag>& t) {
    if (t.get() == nullptr) {
      std::cerr << "Trying to display nullptr:" << __FUNCTION__ << " in " << __FILE__;
      exit(1);
    }
    switch (t->kind()) {
    case PRIMITIVE_KIND:
      switch(t->type()) {
      case BYTE:
	os << dynamic_cast<const PrimitiveTag<uint8_t>&>(*t);
	break;
      case SHORT:
	os << dynamic_cast<const PrimitiveTag<int16_t>&>(*t);
	break;
      case INT:
	os << dynamic_cast<const PrimitiveTag<int32_t>&>(*t);
	break;
      case LONG:
	os << dynamic_cast<const PrimitiveTag<int64_t>&>(*t);
	break;
      case FLOAT:
	os << dynamic_cast<const PrimitiveTag<float>&>(*t);
	break;
      case DOUBLE:
	os << dynamic_cast<const PrimitiveTag<double>&>(*t);
	break;
      case STRING:
	os << dynamic_cast<const PrimitiveTag<string>&>(*t);
	break;
      }
      break;
      
    case ARRAY_KIND:
      switch (t->type()) {
      case BYTE_ARRAY:
	os << dynamic_cast<const ArrayTag<uint8_t>&>(*t);
	break;
      case INT_ARRAY:
	os << dynamic_cast<const ArrayTag<int32_t>&>(*t);
	break;
      case LONG_ARRAY:
	os << dynamic_cast<const ArrayTag<int64_t>&>(*t);
	break;
      }
      break;
      
    case LIST_KIND:
      switch(t->type()) {
      case BYTE:
	os << dynamic_cast<const ListTag<uint8_t>&>(*t);
	break;
      case SHORT:
	os << dynamic_cast<const ListTag<int16_t>&>(*t);
	break;
      case INT:
	os << dynamic_cast<const ListTag<int32_t>&>(*t);
	break;
      case LONG:
	os << dynamic_cast<const ListTag<int64_t>&>(*t);
	break;
      case FLOAT:
	os << dynamic_cast<const ListTag<float>&>(*t);
	break;
      case DOUBLE:
	os << dynamic_cast<const ListTag<double>&>(*t);
	break;
      case BYTE_ARRAY:
	os << dynamic_cast<const ListTag<ArrayTag<uint8_t>>&>(*t);
	break;
      case STRING:
	os << dynamic_cast<const ListTag<string>&>(*t);
	break;
      case LIST:
	// not supported, may be coming soon
	break;
      case COMPOUND:
	os << dynamic_cast<const ListTag<CompoundTag>&>(*t);
	break;
      case INT_ARRAY:
	os << dynamic_cast<const ListTag<ArrayTag<int32_t>>&>(*t);
	break;
      case LONG_ARRAY:
	os << dynamic_cast<const ListTag<ArrayTag<int64_t>>&>(*t);
	break;
      }
      break;
      
    case COMPOUND_KIND:
      os << dynamic_cast<const CompoundTag&>(*t);
      break;
    }
    return os;
  }
}