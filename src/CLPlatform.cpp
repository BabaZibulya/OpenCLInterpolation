#include "CLPlatform.h"

#include <algorithm>
#include <memory>

#include "CL.h"

CLPlatform::CLPlatform(cl_platform_id platformId) :
    platformId(platformId)
{}

std::string getPlatformInfoDetail(cl_platform_id platformId, unsigned int detailInd)
{
    std::size_t detail_length;
    CL(clGetPlatformInfo(platformId, detailInd, 0, nullptr, &detail_length));
    std::unique_ptr<char[]> platform_detail_buf(new char[detail_length + 1]);
    CL(clGetPlatformInfo(platformId, detailInd, detail_length, &platform_detail_buf[0], nullptr));
    std::string platform_detail(platform_detail_buf.get());
    return platform_detail;
}

CLPlatform::PlatformInfo getPlatformInfo(cl_platform_id platformId)
{
    return CLPlatform::PlatformInfo{
        getPlatformInfoDetail(platformId, CL_PLATFORM_PROFILE),
        getPlatformInfoDetail(platformId, CL_PLATFORM_VERSION),
        getPlatformInfoDetail(platformId, CL_PLATFORM_NAME),
        getPlatformInfoDetail(platformId, CL_PLATFORM_VENDOR),
        getPlatformInfoDetail(platformId, CL_PLATFORM_EXTENSIONS),
    };
}

CLPlatform::PlatformInfo CLPlatform::getPlatformInfo()
{
    return ::getPlatformInfo(platformId);
}

std::string CLPlatform::getPlatformInfoDetail(unsigned int detailInd)
{
    return ::getPlatformInfoDetail(platformId, detailInd);
}

CLPlatform platformFromId(cl_platform_id platformId)
{
    return CLPlatform(platformId);
}

std::vector<CLPlatform> CLPlatform::getAllAvailableCLPlatforms()
{
    std::size_t numberOfPlatforms = numberOfAvailablePlatforms();
    std::vector<cl_platform_id> platformIds(numberOfPlatforms);
    CL(clGetPlatformIDs(static_cast<cl_uint>(numberOfPlatforms), &platformIds[0], nullptr));

    std::vector<CLPlatform> platforms;

    std::transform(platformIds.begin(), platformIds.end(), std::back_inserter(platforms), platformFromId);

    return platforms;
}

std::size_t CLPlatform::numberOfAvailablePlatforms()
{
    cl_uint numPlatforms;
    CL(clGetPlatformIDs(0, nullptr, &numPlatforms));
    return numPlatforms;
}

std::ostream& operator<< (std::ostream& ostream, const CLPlatform::PlatformInfo& platformInfo)
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
