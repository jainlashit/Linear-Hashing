#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

/*I don't prefer typedef due to pre-processor reasons but still using it..
didn't wanted to write long long int everywhere */
typedef long long int ll;

const ll null = 10000000000001;

typedef struct Bucket{
	vector<ll> records;
	Bucket* oflwBucket;
}Bucket;

ll M, N, L, s, bsize;
vector<Bucket*> table;

void print()
{
	for (ll i = 0; i < table.size(); ++i)
	{
		Bucket* bucket = table[i];
		while(bucket != NULL)
		{
			cout << "|";
			for (ll i = 0; i < bsize; ++i)
			{
				if(bucket->records[i] == null)
					cout << "n|";
				else
					cout << bucket->records[i] << "|";
			}
			bucket = bucket->oflwBucket;
			cout << "->";
		}
		cout << endl;
	}
	cout << "-----------------------------------------------------" << endl;
}

Bucket* init_bucket()
{
	Bucket* b = new Bucket;
	b->records = vector<ll>(bsize, null);
	b->oflwBucket = NULL;
	return b;
}

inline ll eval(ll N, ll L)
{
	return N * pow(2, L);
}

ll getIndex(ll vl)
{
	if(vl < 0)
		vl *= -1;
	ll ix = vl % eval(N, L);
	if(ix < s)
		ix = vl % eval(N, L + 1);
	return ix;
}

bool put(Bucket* bucket, ll value, bool print_flag)
{
	bool ins = false, end = false, oflwFlag = false, dup = false;
	while(true)
	{
		for (ll i = 0; i < bsize; ++i)
		{
			if(bucket->records[i] == null && !ins)
			{
				bucket->records[i] = value;
				ins = true;
			}
			else if(bucket->records[i] == value)
			{
				// cout << "Records repetition" << endl;
				// cout << value << endl;
				dup = true;
				if(ins)
					bucket->records[i] = null;
				else
					ins = true;
			}
		}
		if(!ins)
			oflwFlag = true;
		if(bucket->oflwBucket == NULL)
		{
			if(ins)
				end = true;
			else
				bucket->oflwBucket = init_bucket();
		}
		if(end)
			break;
		bucket = bucket->oflwBucket;
	}
	if(!dup && print_flag)
		cout << value << endl;
	return oflwFlag;
}

void insert(ll value)
{
	ll index = getIndex(value);
	Bucket* bucket = table[index];
	
	// Checks for overflow
	if(put(bucket, value, true))
	{
		// cout << "Overflow Detected and s = " << s  << endl;
		bucket = table[s];
		index = s;
		table.push_back(init_bucket());
		// Add 1 because modulo of all done till n - 1
		s++;
		if(s == eval(N, L))
		{
			s = 0;
			L++;
		}
		ll newIndex;
		while(bucket != NULL)
		{
			for (ll i = 0; i < bsize; ++i)
			{
				if(bucket->records[i] != null)
				{
					newIndex = getIndex(bucket->records[i]);
					if(newIndex != index)
					{
						// cout << "newIndex = " << newIndex << endl;
						put(table[newIndex], bucket->records[i], false);
						bucket->records[i] = null;
					}
				}
			}
			bucket = bucket->oflwBucket;
		}
	}
}


void read_file(char const *filePath)
{
	cout << "N = ";
	cin >> N;
	for (ll i = 0; i < N; ++i)
		table.push_back(init_bucket());
	ifstream infile(filePath);
	ll number;
	while(infile >> number)	
		insert(number);
	// print();
}

int main()
{
	string filePath;
	cout << "Enter file path : ";
	cin >> filePath;
	L = 0, s = 0, bsize = 2;
	read_file(filePath.c_str());
	return 0;
}