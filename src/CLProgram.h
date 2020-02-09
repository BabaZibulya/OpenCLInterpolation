#pragma once

#include <vector>
#include <string>
#include <type_traits>

#include "CLContext.h"
#include "CLTypes.h"
#include "CLBuffer.h"

namespace CL {
	class Program
	{
	public:
        enum class ProgramType {
            BINARY,
            SOURCE
        };

		Program(
			const CL::Context& context,
			const std::vector<Device>& devices,
			const std::vector<std::string>& inputFiles,
            ProgramType programType);
		Program(Program&& program);
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;
		~Program();

		static Program createProgram(
			const CL::Context& context,
			const std::vector<Device>& devices,
			const std::vector<std::string>& pathes,
            ProgramType programType);

	public:
		cl_program program;

	public:
		struct Kernel {
			Kernel(const Program& program, const std::string& kernelName);
			Kernel(Kernel&& kernel);
			Kernel(const Kernel&) = delete;
			Kernel& operator=(const Kernel&) = delete;
			~Kernel();

			template<typename ArgType>
			void setKernelArg(size_t argPosition, const ArgType& arg, typename std::enable_if<std::is_arithmetic<ArgType>::value>::type* = 0)
			{
				setKernelArg(argPosition, sizeof(arg), const_cast<ArgType*>(&arg));
			}
			void setKernelArg(size_t argPosition, const Buffer& arg);
			void setKernelArg(size_t argPosition, size_t size, void* arg);

			cl_kernel kernel;
		};

		Kernel createKernel(const std::string& kernelName);

	private:
		std::string compilationLog(const Device& device);
	};
}