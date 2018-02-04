/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#include "BeckyVersion.h"

#include "bkzoo_string.h"
#include "bkzoo_config.h"
#include "bkzoo_log.h"

#include <regex>
#include <vector>
#include <utility>

#include "for_debug/detect_memory_leak.h"

namespace bkzoo
{
    namespace util
    {

        std::string BeckyVersion::extractBeckyVersionFrom(const std::string& xMailer)
        {
            std::regex regex("^Becky! ver\\. ([0-9]+\\.[0-9]+(\\.[0-9]+)?) \\[[a-zA-Z0-0\\-]+\\]$");
            std::smatch results;

            if (!std::regex_search(xMailer, results, regex))
            {
                return "";
            }

            if (results.size() < 2)
            {
                return "";
            }

            if (results.str(1).empty())
            {
                return "";
            }

            return results.str(1);
        }

        bool BeckyVersion::isValidBeckyVersion(const std::string& version)
        {
            std::regex regex("^[0-9]+\\.[0-9]+(\\.[0-9]+)?$");
            return std::regex_match(version, regex);
        }


        BeckyVersion BeckyVersion::getBeckyVersionOfMyself(const std::string& myselfVersion)
        {
            BeckyVersion myselfBeckyVersion;
            {
                if (!BeckyVersion::isValidBeckyVersion(myselfVersion))
                {
                    LOG_ERROR << "isValidBeckyVersion(myselfVersion) is false : myselfVersion=" << myselfVersion;
                    assert(false);
                    return std::move(myselfBeckyVersion);
                }
                myselfBeckyVersion.initialize(myselfVersion);
            }
            return std::move(myselfBeckyVersion);
        }


        BeckyVersion BeckyVersion::getBeckyVersionLatest(const std::string& myselfVersion)
        {
            BeckyVersion latestBeckyVersion;
            {
                const std::string latestVersion(wm_cast<std::string>(Config::instance().version()));
                if (!BeckyVersion::isValidBeckyVersion(latestVersion))
                    latestBeckyVersion.initialize(myselfVersion); // configになければ自分のbeckyVersionいれる
                else
                    latestBeckyVersion.initialize(latestVersion); // configにあればそれがこれまでの最新
            }
            return std::move(latestBeckyVersion);
        }


        BeckyVersion::BeckyVersion(BeckyVersion&& rhs)
        {
            *this = std::move(rhs);
        }
        BeckyVersion& BeckyVersion::operator=(BeckyVersion&& rhs)
        {
            if (this == &rhs)
                return *this;

            version_ = std::move(rhs.version_);
            rhs.version_.clear();

            for (int i = 0; i < 3; ++i)
            {
                versions_[i] = rhs.versions_[i];
                rhs.versions_[i] = 0;
            }

#ifndef NDEBUG
            isValid_ = rhs.isValid_;
            rhs.isValid_ = false;
#endif // NDEBUG

            return *this;
        }

        void BeckyVersion::initialize(const std::string& version)
        {
            assert(isValidBeckyVersion(version));

            version_ = version;

            std::vector<std::string> versions = StringUtils::split(version, '.');
            assert(versions.size() == 3 || versions.size() == 2);

            versions_[Major] = std::stoi(versions[0].c_str());
            versions_[Minor] = std::stoi(versions[1].c_str());
            versions_[Revision] = (versions.size() == 3) ? std::stoi(versions[2].c_str()) : 0;

#ifndef NDEBUG
            isValid_ = true;
#endif // NDEBUG
        }

        const std::string& BeckyVersion::version() const
        {
            assert(isValid_);
            return version_;
        }

        bool BeckyVersion::operator<(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            for (int i = Major; i < VersionType::SIZE; ++i)
            {
                if (versions_[i] < rhs.versions_[i])
                    return true;
                if (versions_[i] > rhs.versions_[i])
                    return false;
                assert(versions_[i] == rhs.versions_[i]);
            }
            return false;
        }
        bool BeckyVersion::operator>(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            if (*this == rhs)
                return false;
            return (*this >= rhs);
        }
        bool BeckyVersion::operator<=(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            return !(*this > rhs);
        }
        bool BeckyVersion::operator>=(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            return !(*this < rhs);
        }
        bool BeckyVersion::operator==(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            for (int i = Major; i < VersionType::SIZE; ++i)
            {
                if (versions_[i] != rhs.versions_[i])
                    return false;
                assert(versions_[i] == rhs.versions_[i]);
            }
            return true;
        }
        bool BeckyVersion::operator!=(const BeckyVersion& rhs) const
        {
            assert(isValid_);
            return !(*this == rhs);
        }

    }
}
