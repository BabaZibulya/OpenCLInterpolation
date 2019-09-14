#pragma once

#include <vector>
#include <string>
#include <ostream>

#include "CLTypes.h"

namespace CL {
	class Platform {
	public:

		static const int CL_PLATFORM_PROFILE     = 0x0900;
		static const int CL_PLATFORM_VERSION     = 0x0901;
		static const int CL_PLATFORM_NAME        = 0x0902;
		static const int CL_PLATFORM_VENDOR      = 0x0903;
		static const int CL_PLATFORM_EXTENSIONS  = 0x0904;
		
		explicit Platform(cl_platform_id platformId);

		struct PlatformInfo {
			const std::string profile, version, name, vendor, extensions;
			PlatformInfo() = delete;
			friend std::ostream& operator<< (std::ostream& ostream, const PlatformInfo& platformInfo);
		};
		PlatformInfo getPlatformInfo() const;
		std::string getPlatformInfoDetail(unsigned int detailInd);

		const cl_platform_id clHandle;
	};
}

