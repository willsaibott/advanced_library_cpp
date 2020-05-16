#ifndef READ_CSV_H
#define READ_CSV_H

#include <list>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <cmath>
#include <iterator>

namespace advanced {
namespace tools {

enum class CSV_FIELD_TYPE {
	STR,
	INT,
	DOUBLE
};

class generic_field_t {

public:

	inline static
	std::shared_ptr<generic_field_t> make_field(std::string s, CSV_FIELD_TYPE t= CSV_FIELD_TYPE::STR)
	{
		return std::make_shared<generic_field_t>(s,t);
	}

	generic_field_t(std::string s = "") : m_sdata(s)  {
		m_type = CSV_FIELD_TYPE::STR;
		m_idata = static_cast<int>(std::nan(""));
		m_fdata = std::nan("");
	}

	generic_field_t(std::string s, CSV_FIELD_TYPE t) {
		m_sdata = s;
		m_type = t;
		switch (t) {
		case CSV_FIELD_TYPE::INT:
			m_idata = std::stoi(m_sdata);
			m_fdata = std::nan("");
			break;
		case CSV_FIELD_TYPE::DOUBLE:
			m_fdata = std::stod(m_sdata);
			m_idata = static_cast<int>(std::nan(""));
			break;
		default:
			break;
		}
	}

	std::string
	string() {
		return m_sdata;
	}

	int
	get_int() {
		if (std::isnan(m_idata))
			return std::stoi(m_sdata);
		else
			return m_idata;
	}

	double
	get_double() {
		if (std::isnan(m_fdata))
			return std::stod(m_sdata);
		else
			return m_fdata;
	}

private:

	CSV_FIELD_TYPE m_type;
	int m_idata;
	double m_fdata;
	std::string m_sdata;
};

typedef std::shared_ptr<generic_field_t> csv_field_t;

class csv_file_s {
public:
	std::list<std::vector<csv_field_t>> list;
	std::vector<CSV_FIELD_TYPE> file_descriptor;

	csv_file_s(std::vector<CSV_FIELD_TYPE> fdescriptor)
		: file_descriptor(fdescriptor) {}
};

typedef std::shared_ptr<csv_file_s> csv_file_t;

class csv_reader_t {

public:

	csv_reader_t(std::vector<CSV_FIELD_TYPE> file_descriptor,
				 int start_offset=0,
				 char separator = ',') {
		m_file_descriptor = file_descriptor;
		m_separator = separator;
		m_start_offset = start_offset;
	}

	csv_file_t
	read_csv_file(const std::string filepath)    {
		csv_file_t csvfile  = make_csv(m_file_descriptor);
		std::ifstream filestream(filepath.c_str());
		if (filestream.good()) {
			std::string line;
			int line_count = 0;
			while ((line_count++ < m_start_offset) && (getline(filestream, line)));
			while (getline(filestream, line)) {
				std::vector<csv_field_t> vline;
				std::string field;
				std::istringstream split(line);
				int n=0;
				const int size = m_file_descriptor.size();
				while (getline(split, field, m_separator)) {
					if (n < size) {
						csv_field_t f = generic_field_t::make_field(field,m_file_descriptor[n++]);
						vline.push_back(f);
					}
					else {
						std::cout << "File doesn't correspond to given file descriptor!" << std::endl;
						return csvfile;
					}
				}
				csvfile->list.push_back(vline);
			}
		}

		return csvfile;
	}

private:

	char m_separator;
	int m_start_offset;
	std::vector<CSV_FIELD_TYPE> m_file_descriptor;

	csv_file_t make_csv(std::vector<CSV_FIELD_TYPE> file_descriptor)
	{
		csv_file_t x = std::make_shared<csv_file_s>(file_descriptor);
		return x;
	}
};

}
}

#endif // READ_CSV_H
