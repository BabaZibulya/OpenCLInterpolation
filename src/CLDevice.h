#pragma once

#include <vector>

#include "CLPlatform.h"
#include "CLTypes.h"

namespace CL {
	class Device
	{
	public:
		/*
		 * #define CL_DEVICE_TYPE_DEFAULT                      (1 << 0)
		 * #define CL_DEVICE_TYPE_CPU                          (1 << 1)
		 * #define CL_DEVICE_TYPE_GPU                          (1 << 2)
		 * #define CL_DEVICE_TYPE_ACCELERATOR                  (1 << 3)
		 * #define CL_DEVICE_TYPE_CUSTOM                       (1 << 4)
		 * #define CL_DEVICE_TYPE_ALL                          0xFFFFFFFF
		 */
		enum class DeviceType : unsigned int {
			Default = (1 << 0),
			CPU = (1 << 1),
			GPU = (1 << 2),
			Accelerator = (1 << 3),
			Custom = (1 << 4),
			All = 0xFFFFFFFF
		};

		const cl_device_id deviceId;

		static std::vector<Device> getAllAvailableCLDevices(
			const Platform& platform,
			DeviceType deviceType = DeviceType::All);

		static std::size_t numberOfAvailableCLDevices(
			const Platform& platform,
			DeviceType deviceType = DeviceType::All);

		template<typename OutputType, typename = std::enable_if_t<std::is_scalar<OutputType>::value>>
		OutputType getDeviceInfoDetail(unsigned int detailInd) const
		{
			OutputType result;
			getDeviceInfoDetail(detailInd, sizeof(OutputType), &result);
			return result;
		}

		std::string getDeviceInfoDetail(unsigned int detailInd) const;

	private:
		Device(cl_device_id deviceId);

		std::size_t getDeviceInfoDetailSize(unsigned int detailInd) const;
		void getDeviceInfoDetail(unsigned int detailInd, std::size_t sizeOfOutput, void *output) const;
	};
}
