#include "Tools/system_functions.h"
#include "Tools/Exception/exception.hpp"

using namespace aff3ct::tools;

const std::string aff3ct::tools::exception::empty_string = "";
bool aff3ct::tools::exception::no_backtrace = false;
bool aff3ct::tools::exception::no_addr_to_line = true;

exception
::exception() throw()
{
}

exception
::exception(const std::string &message) throw()
: message(message)
#ifdef AFF3CT_CORE_BACKTRACE
,backtrace(message + "\n" + get_back_trace(3))
#endif
{
}

exception
::exception(const std::string &filename,
            const int line_num,
            const std::string &funcname,
            const std::string &_message) throw()
{
	if (!filename.empty())
		this->message += "In the '" + filename + "' file";
	if (line_num >= 0)
		this->message += " at line " + std::to_string(line_num);
	if (!funcname.empty())
		this->message += " ('" + funcname + "' function)";
	this->message += ": ";
	this->message += "\"" + _message + "\"";

#ifdef AFF3CT_CORE_BACKTRACE
	backtrace = this->message + "\n" + get_back_trace(3);
#endif
}

const char* exception
::what() const throw()
{
#ifdef AFF3CT_CORE_BACKTRACE
	if (no_backtrace)
		return message.c_str();
	else if (no_addr_to_line)
		return backtrace.c_str();

	std::string* bt_a2l = const_cast<std::string*>(&backtrace_a2l);

	if (bt_a2l->empty())
		*bt_a2l = tools::addr_to_line(backtrace);

	return bt_a2l->c_str();
#else
	return message.c_str();
#endif
}