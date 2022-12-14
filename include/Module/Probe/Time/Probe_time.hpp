/*!
 * \file
 * \brief Class module::Probe_time.
 */
#ifndef PROBE_TIME_HPP_
#define PROBE_TIME_HPP_

#include <string>
#include <vector>
#include <chrono>
#include <typeindex>

#include "Module/Probe/Probe.hpp"

namespace aff3ct
{
namespace module
{
class Probe_time : public Probe<uint8_t>
{
protected:
	std::chrono::time_point<std::chrono::steady_clock> t_start;

public:
	Probe_time(const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames = 1);

	virtual ~Probe_time() = default;

	virtual std::type_index get_datatype() const;

	virtual void reset();

protected:
	virtual void _probe(const uint8_t *in, const size_t frame_id);
};
}
}

#endif /* PROBE_TIME_HPP_ */
