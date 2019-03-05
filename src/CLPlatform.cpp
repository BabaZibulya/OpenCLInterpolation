#include "CLPlatform.h"

#include <algorithm>
#include <memory>

#include "CL.h"

using namespace CL;

Platform::Platform(cl_platform_id platformId) :
    clHandle(platformId)
{
}

std::string getPlatformInfoDetail(cl_platform_id platformId, unsigned int detailInd)
{
    std::size_t detail_length;
    CL(clGetPlatformInfo(platformId, detailInd, 0, nullptr, &detail_length));
    std::unique_ptr<char[]> platform_detail_buf(new char[detail_length + 1]);
    CL(clGetPlatformInfo(platformId, detailInd, detail_length, &platform_detail_buf[0], nullptr));
    std::string platform_detail(platform_detail_buf.get());
    return platform_detail;
}

Platform::PlatformInfo getPlatformInfo(cl_platform_id platformId)
{
    return Platform::PlatformInfo{
        getPlatformInfoDetail(platformId, CL_PLATFORM_PROFILE),
        getPlatformInfoDetail(platformId, CL_PLATFORM_VERSION),
        getPlatformInfoDetail(platformId, CL_PLATFORM_NAME),
        getPlatformInfoDetail(platformId, CL_PLATFORM_VENDOR),
        getPlatformInfoDetail(platformId, CL_PLATFORM_EXTENSIONS),
    };
}

Platform::PlatformInfo Platform::getPlatformInfo() const
{
    return ::getPlatformInfo(clHandle);
}

std::ostream& CL::operator<< (std::ostream& ostream, const Platform::PlatformInfo& platformInfo)
{
    ostream << "Platform info: \n"
            << "Profile " << platformInfo.profile << '\n'
            << "Version " << platformInfo.version << '\n'
            << "Name " << platformInfo.name << '\n'
            << "Vendor " << platformInfo.vendor << '\n'
            << "Extensions " << platformInfo.extensions << '\n'
            << '\n';
    return ostream;
}
