#include <fstream>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Source/User/Source_user.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Source_user<B>
::Source_user(const int max_data_size, const std::string &filename, const int start_idx)
: Source<B>(max_data_size), source(), src_counter(start_idx)
{
	const std::string name = "Source_user";
	this->set_name(name);

	if (filename.empty())
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'filename' should not be empty.");

	std::ifstream file(filename.c_str(), std::ios::in);

	if (file.is_open())
	{
		int n_src = 0, src_size = 0;

		file >> n_src;
		file >> src_size;

		if (n_src <= 0 || src_size <= 0)
		{
			std::stringstream message;
			message << "'n_src', and 'src_size' have to be greater than 0 ('n_src' = " << n_src
			        << ", 'src_size' = " << src_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->source.resize(n_src);
		for (auto i = 0; i < n_src; i++)
			this->source[i].resize(src_size);

		if (src_size == this->max_data_size)
		{
			for (auto i = 0; i < n_src; i++)
				for (auto j = 0; j < src_size; j++)
				{
					int bit;
					file >> bit;

					this->source[i][j] = bit != 0;
				}
		}
		else
		{
			file.close();

			std::stringstream message;
			message << "The size is wrong (read: " << src_size << ", expected: " << this->max_data_size << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		file.close();
	}
	else
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Can't open '" + filename + "' file.");

	src_counter %= (int)source.size();
}

template <typename B>
Source_user<B>* Source_user<B>
::clone() const
{
	auto m = new Source_user(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Source_user<B>
::_generate(B *out_data, const size_t frame_id)
{
	std::copy(this->source[this->src_counter].begin(),
	          this->source[this->src_counter].end  (),
	          out_data);

	this->src_counter = (this->src_counter +1) % (int)this->source.size();
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Source_user<int8_t>;
template class aff3ct::module::Source_user<uint8_t>;
template class aff3ct::module::Source_user<int16_t>;
template class aff3ct::module::Source_user<uint16_t>;
template class aff3ct::module::Source_user<int32_t>;
template class aff3ct::module::Source_user<uint32_t>;
template class aff3ct::module::Source_user<int64_t>;
template class aff3ct::module::Source_user<uint64_t>;
template class aff3ct::module::Source_user<float>;
template class aff3ct::module::Source_user<double>;
// ==================================================================================== explicit template instantiation
