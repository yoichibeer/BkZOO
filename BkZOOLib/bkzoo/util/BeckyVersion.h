/*
 * BkZOO!
 *
 * Copyright 2011-2017 yoichibeer.
 * Released under the MIT license.
 */

#ifndef BKZ_UTIL_BECKY_VERSION_H
#define BKZ_UTIL_BECKY_VERSION_H

#include "defs.h"

#include <string>

#include "bkzoo_gtest.h"

namespace bkzoo
{
    namespace util
    {

        class BeckyVersion final
        {
            enum VersionType : int
            {
                Major,
                Minor,
                Revision,
                SIZE,
            };

        public:
            BeckyVersion() = default;
            ~BeckyVersion() = default;

            BeckyVersion(BeckyVersion&& rhs);
            BeckyVersion& operator=(BeckyVersion&& rhs);

            void initialize(const std::string& version);
            const std::string& version() const;

            bool operator<(const BeckyVersion& rhs) const;
            bool operator>(const BeckyVersion& rhs) const;
            bool operator<=(const BeckyVersion& rhs) const;
            bool operator>=(const BeckyVersion& rhs) const;
            bool operator==(const BeckyVersion& rhs) const;
            bool operator!=(const BeckyVersion& rhs) const;

            static std::string extractBeckyVersionFrom(const std::string& xMailer);
            static bool isValidBeckyVersion(const std::string& version);

            static util::BeckyVersion getBeckyVersionOfMyself(const std::string& myselfVersion);
            static util::BeckyVersion getBeckyVersionLatest(const std::string& myselfVersion);

        private:
            std::string version_;
            int versions_[3] = { 0 }; // major . minor . revision

#ifndef NDEBUG
            bool isValid_ = false;
#endif // NDEBUG

            FRIEND_GTEST(BeckyVersionTest, moveConstructor);
            FRIEND_GTEST(BeckyVersionTest, moveAssignmentOperator);
            FRIEND_GTEST(BeckyVersionTest, isValid);

            DISALLOW_COPY_AND_ASSIGN(BeckyVersion);
        };

    }
}

#endif // BKZ_UTIL_BECKY_VERSION_H
