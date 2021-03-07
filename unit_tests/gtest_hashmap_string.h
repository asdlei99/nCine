#ifndef GTEST_HASHMAP_STRING_H
#define GTEST_HASHMAP_STRING_H

#include <nctl/HashMap.h>
#include <nctl/HashMapIterator.h>
#include <nctl/String.h>
#include "gtest/gtest.h"

namespace {

const unsigned int Capacity = 32;
const unsigned int Size = 6;
const char *Keys[Size] = { "A", "a", "B", "C", "AB", "BA" };
const char *Values[Size] = { "AAAA", "aaaa", "BBBB", "CCCC", "ABABABAB", "BABABABA" };

void initHashMap(nctl::HashMap<nctl::String, nctl::String> &strHashmap)
{
	for (unsigned int i = 0; i < Size; i++)
		strHashmap[Keys[i]] = Values[i];
}

void printHashMap(nctl::HashMap<nctl::String, nctl::String> &strHashmap)
{
	unsigned int n = 0;

	for (nctl::HashMap<nctl::String, nctl::String>::ConstIterator i = strHashmap.begin(); i != strHashmap.end(); ++i)
		printf("[%u] hash: %u, key: %s, value: %s\n", n++, i.hash(), i.key().data(), i.value().data());
	printf("\n");
}

unsigned int calcSize(const nctl::HashMap<nctl::String, nctl::String> &strHashmap)
{
	unsigned int length = 0;

	for (typename nctl::HashMap<nctl::String, nctl::String>::ConstIterator i = strHashmap.begin(); i != strHashmap.end(); ++i)
		length++;

	return length;
}

void assertHashMapsAreEqual(const nctl::HashMap<nctl::String, nctl::String> &strHashmap1, const nctl::HashMap<nctl::String, nctl::String> &strHashmap2)
{
	nctl::HashMap<nctl::String, nctl::String>::ConstIterator strHashmap1It = strHashmap1.begin();
	nctl::HashMap<nctl::String, nctl::String>::ConstIterator strHashmap2It = strHashmap2.begin();
	while (strHashmap1It != strHashmap1.end())
	{
		ASSERT_EQ(strHashmap1It.key(), strHashmap2It.key());
		ASSERT_EQ(*strHashmap1It, *strHashmap2It);

		strHashmap1It++;
		strHashmap2It++;
	}
}

}

#endif
