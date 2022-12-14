#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{

template <typename T>
runtime::Task& Probe<T>
::operator[](const prb::tsk t)
{
	return Module::operator[]((int)t);
}

template <typename T>
runtime::Socket& Probe<T>
::operator[](const prb::sck::probe s)
{
	assert((*this)[prb::tsk::probe].get_n_input_sockets() == 1);
	return Module::operator[]((int)prb::tsk::probe)[(int)s];
}

template <typename T>
runtime::Socket& Probe<T>
::operator[](const prb::sck::probe_noin s)
{
	assert((*this)[prb::tsk::probe].get_n_input_sockets() == 0);
	return Module::operator[]((int)prb::tsk::probe)[(int)s];
}

template <typename T>
Probe<T>
::Probe(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: AProbe(), size(size), col_name(col_name), reporter(reporter)
{
	const std::string name = "Probe<" + col_name + ">";
	this->set_name(name);
	this->set_short_name(name);
	AProbe::set_n_frames(n_frames);

	if (size < 0)
	{
		std::stringstream message;
		message << "'size' has to be greater or equal to 0 ('size' = " << size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("probe");
	auto p1s_in = (size > 0) ? (int)this->template create_socket_in<T>(p1, "in", this->size) : (int)-1;
	this->create_codelet(p1, [p1s_in](Module &m, runtime::Task &t, const size_t frame_id) -> int
	{
		auto &prb = static_cast<Probe<T>&>(m);
		const T* in = (p1s_in != -1) ? static_cast<const T*>(t[p1s_in].get_dataptr()) : nullptr;
		prb._probe(in, frame_id);
		return runtime::status_t::SUCCESS;
	});
}

template <typename T>
void Probe<T>
::set_n_frames(const size_t n_frames)
{
	const size_t old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		std::stringstream message;
		message << "This method is disabled in the 'Probe' class.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
template <class AT>
void Probe<T>
::probe(const std::vector<T,AT>& in, const int frame_id, const bool managed_memory)
{
	if ((*this)[prb::tsk::probe].get_n_input_sockets() != 1)
	{
		std::stringstream message;
		message << "The number of input sockets has to be equal to 1 ('get_n_input_sockets()' = "
		        << (*this)[prb::tsk::probe].get_n_input_sockets() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	(*this)[prb::sck::probe::in].bind(in);
	(*this)[prb::tsk::probe].exec(frame_id, managed_memory);
}

template <typename T>
void Probe<T>
::probe(const T *in, const int frame_id, const bool managed_memory)
{
	if ((*this)[prb::tsk::probe].get_n_input_sockets() != 1)
	{
		std::stringstream message;
		message << "The number of input sockets has to be equal to 1 ('get_n_input_sockets()' = "
		        << (*this)[prb::tsk::probe].get_n_input_sockets() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	(*this)[prb::sck::probe::in].bind(in);
	(*this)[prb::tsk::probe].exec(frame_id, managed_memory);
}

template <typename T>
void Probe<T>
::probe(const int frame_id, const bool managed_memory)
{
	if ((*this)[prb::tsk::probe].get_n_input_sockets() != 0)
	{
		std::stringstream message;
		message << "The number of input sockets has to be equal to 0 ('get_n_input_sockets()' = "
		        << (*this)[prb::tsk::probe].get_n_input_sockets() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	(*this)[prb::tsk::probe].exec(frame_id, managed_memory);
}

template <typename T>
void Probe<T>
::_probe(const T *in, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename T>
void Probe<T>
::reset()
{
}

}
}
