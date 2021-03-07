#ifndef GTEST_HASHSET_CSTRING_H
#define GTEST_HASHSET_CSTRING_H

#include <nctl/HashSet.h>
#include <nctl/HashSetIterator.h>
#include <nctl/String.h>
#include "gtest/gtest.h"

namespace {

const unsigned int Capacity = 32;
const unsigned int Size = 6;
const char *Keys[Size] = { "A", "a", "B", "C", "AB", "BA" };
/// A new set of C-style string keys, same in content but different in memory address
const unsigned int MaxLength = 3;
char KeysCopy[Size][MaxLength];

void initHashSet(nctl::HashSet<const char *> &cstrHashset)
{
	for (unsigned int i = 0; i < Size; i++)
	{
		cstrHashset.insert(Keys[i]);
		strncpy(KeysCopy[i], Keys[i], 5);
	}
}

void printHashSet(nctl::HashSet<const char *> &cstrHashset)
{
	unsigned int n = 0;

	for (nctl::HashSet<const char *>::ConstIterator i = cstrHashset.begin(); i != cstrHashset.end(); ++i)
		printf("[%u] hash: %u, key: %s\n", n++, i.hash(), i.key());
	printf("\n");
}

unsigned int calcSize(const nctl::HashSet<const char *> &cstrHashset)
{
	unsigned int length = 0;

	for (typename nctl::HashSet<const char *>::ConstIterator i = cstrHashset.begin(); i != cstrHashset.end(); ++i)
		length++;

	return length;
}

}

#endif
