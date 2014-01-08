#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-MYC");
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", false));
    BOOST_CHECK(GetBoolArg("-MYC", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-MYCo"));
    BOOST_CHECK(!GetBoolArg("-MYCo", false));
    BOOST_CHECK(GetBoolArg("-MYCo", true));

    ResetArgs("-MYC=0");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", false));
    BOOST_CHECK(!GetBoolArg("-MYC", true));

    ResetArgs("-MYC=1");
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", false));
    BOOST_CHECK(GetBoolArg("-MYC", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noMYC");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", false));
    BOOST_CHECK(!GetBoolArg("-MYC", true));

    ResetArgs("-noMYC=1");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", false));
    BOOST_CHECK(!GetBoolArg("-MYC", true));

    ResetArgs("-MYC -noMYC");  // -MYC should win
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", false));
    BOOST_CHECK(GetBoolArg("-MYC", true));

    ResetArgs("-MYC=1 -noMYC=1");  // -MYC should win
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", false));
    BOOST_CHECK(GetBoolArg("-MYC", true));

    ResetArgs("-MYC=0 -noMYC=0");  // -MYC should win
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", false));
    BOOST_CHECK(!GetBoolArg("-MYC", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--MYC=1");
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", false));
    BOOST_CHECK(GetBoolArg("-MYC", true));

    ResetArgs("--noMYC=1");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", false));
    BOOST_CHECK(!GetBoolArg("-MYC", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-MYC", "eleven"), "eleven");

    ResetArgs("-MYC -bar");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-MYC", "eleven"), "");

    ResetArgs("-MYC=");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-MYC", "eleven"), "");

    ResetArgs("-MYC=11");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-MYC", "eleven"), "11");

    ResetArgs("-MYC=eleven");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-MYC", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-MYC", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-MYC", 0), 0);

    ResetArgs("-MYC -bar");
    BOOST_CHECK_EQUAL(GetArg("-MYC", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-MYC=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-MYC", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-MYC=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-MYC", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--MYC");
    BOOST_CHECK_EQUAL(GetBoolArg("-MYC"), true);

    ResetArgs("--MYC=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-MYC", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noMYC");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", true));
    BOOST_CHECK(!GetBoolArg("-MYC", false));

    ResetArgs("-noMYC=1");
    BOOST_CHECK(!GetBoolArg("-MYC"));
    BOOST_CHECK(!GetBoolArg("-MYC", true));
    BOOST_CHECK(!GetBoolArg("-MYC", false));

    ResetArgs("-noMYC=0");
    BOOST_CHECK(GetBoolArg("-MYC"));
    BOOST_CHECK(GetBoolArg("-MYC", true));
    BOOST_CHECK(GetBoolArg("-MYC", false));

    ResetArgs("-MYC --noMYC");
    BOOST_CHECK(GetBoolArg("-MYC"));

    ResetArgs("-noMYC -MYC"); // MYC always wins:
    BOOST_CHECK(GetBoolArg("-MYC"));
}

BOOST_AUTO_TEST_SUITE_END()
