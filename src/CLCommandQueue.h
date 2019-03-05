#pragma once

#include <memory>

#include "CLContext.h"
#include "CLDevice.h"
#include "CLTypes.h"
#include "CLBuffer.h"
#include "CLProgram.h"

#include "NamedParameter.h"

namespace CL {
	class CommandQueue {
	public:
		CommandQueue(CommandQueue&& commandQueue);
		CommandQueue(const Context& context, const Device& device);
		CommandQueue(const CommandQueue&) = delete;
		CommandQueue& operator=(const CommandQueue&) = delete;
		~CommandQueue();

		void enqueueWriteBuffer(const Buffer& buffer, size_t bufferSize, const void *const rawBuffer);
		void enqueueReadBuffer(const Buffer& buffer, size_t bufferSize, void *const rawBuffer);

		using GlobalSize = NamedType<std::vector<size_t>, struct GlobalSizeParameter>;
		using LocalSize = NamedType<std::vector<size_t>, struct LocalSize>;
		using Offset = NamedType<std::vector<size_t>, struct OffsetParameter>;

		void enqueueNDRangeKernel(
			const Program::Kernel& kernel,
			size_t globalSize,
			size_t localWorkSize);
		void enqueueNDRangeKernel(
			const Program::Kernel& kernel,
			size_t workDim,
			const GlobalSize& globalSize,
			const LocalSize& localWorkSize);
		void enqueueNDRangeKernel(
			const Program::Kernel& kernel,
			size_t workDim,
			const GlobalSize& globalSize,
			const LocalSize& localWorkSize,
			const Offset& offsets);
		void enqueueNDRangeKernel(
			const Program::Kernel& kernel,
			size_t workDim,
			const GlobalSize& globalSize
		);
		void enqueueNDRangeKernel(
			const Program::Kernel& kernel,
			size_t workDim,
			const GlobalSize& globalSize,
			const Offset& offset
		);

		void finish();
	public:
		cl_command_queue clHandle;
	};
}