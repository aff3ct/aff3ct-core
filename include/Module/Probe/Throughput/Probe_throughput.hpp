/*!
 * \file
 * \brief Class module::Probe_throughput.
 */
#ifndef PROBE_THROUGHPUT_HPP_
#define PROBE_THROUGHPUT_HPP_

#include <string>
#include <vector>
#include <chrono>
#include <typeindex>

#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{
class Probe_throughput : public Probe<uint8_t>
{
	friend tools::Reporter_probe;

protected:
	std::chrono::time_point<std::chrono::steady_clock> t_start;
	size_t data_size;
	double thr;
	double factor;
	Probe_throughput(const size_t data_size, const std::string &col_name, tools::Reporter_probe& reporter);
	Probe_throughput(const size_t data_size, const std::string &col_name, const double factor,
	                 tools::Reporter_probe& reporter);

public:
	virtual ~Probe_throughput() = default;

	virtual std::type_index get_datatype() const;

	virtual void reset();

protected:
	virtual void _probe(const uint8_t *in, const size_t frame_id);
};
}
}

#endif /* PROBE_THROUGHPUT_HPP_ */
