#include <bits/stdc++.h>
using namespace std;

class bucket
{
    int ld, size;
    vector<int> b;

public:
    bucket(int ld, int size);
    int isEmpty(void);
    int isFull(void);
    int insert(int);
    int search(int);
    int remove(int);
    int getld(void);
    int incrdepth(void);
    int decrdepth(void);
    int display(void);
    vector<int> copy(void);
    void clr(void);
    int getSize();
};

bucket::bucket(int ld, int size)
{
    this->ld = ld;
    this->size = size;
}

int bucket::isEmpty()
{
    if (b.size() == 0)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int bucket::isFull()
{
    if (b.size() == size)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int bucket::insert(int n)
{
    if (!isFull())
    {
        b.push_back(n);
        return (1);
    }
    else
    {
        return (0);
    }
}

int bucket::search(int n)
{
    vector<int>::iterator it;
    it = find(b.begin(), b.end(), n);
    if (it != b.end())
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int bucket::remove(int n)
{
    vector<int>::iterator it;
    it = find(b.begin(), b.end(), n);
    if (it != b.end())
    {
        b.erase(it);
        return (1);
    }
    else
    {
        return (0);
    }
}

int bucket::getld()
{
    return (ld);
}

int bucket::incrdepth()
{
    ld++;
    return (ld);
}

int bucket::decrdepth()
{
    ld--;
    return (ld);
}

int bucket::display()
{
    vector<int>::iterator it;
    for (it = b.begin(); it != b.end(); it++)
    {
        cout << (*it) << " ";
    }
    cout << endl;
    return (1);
}

vector<int> bucket::copy()
{
    vector<int> temp(b.begin(), b.end());
    return (temp);
}

void bucket::clr()
{
    b.clear();
}

int bucket::getSize()
{
    return (b.size());
}

class directory
{
    int gd, siz;
    vector<bucket *> b;
    vector<int> bno;
    int hash(int, int);
    int split(int);
    int pairIndex(int, int);
    void shrink(void);
    void merge(int bucket_no);

public:
    directory(int gd, int siz);
    int insert(int);
    void display();
    int search(int);
    int remove(int);
};



directory::directory(int gd, int siz)
{
    this->gd = gd;
    this->siz = siz;
    for (int i = 0; i < (1 << gd); i++)
    {
        bno.push_back(i);
        b.push_back(new bucket(gd, siz));
    }
}

int directory::hash(int n, int depth)
{
    return (n & ((1 << depth) - 1));
}

int directory::pairIndex(int bucket_no, int depth)
{
    return bucket_no ^ (1 << (depth - 1));
}

int directory::split(int n)
{
    int ld, pi, id, d;
    vector<int> temp;
    ld = b[n]->incrdepth();
    if (ld > gd)
    {
        for (int i = 0; i < (1 << gd); i++)
        {
            b.push_back(b[i]);
        }
        gd++;
    }
    if (gd > 20)
    {
        return (0);
    }
    pi = pairIndex(n, ld);
    bno.push_back(pi);
    b[pi] = new bucket(ld, siz);
    temp = b[n]->copy();
    b[n]->clr();
    id = 1 << ld;
    d = 1 << gd;
    for (int i = pi - id; i >= 0; i -= id)
    {
        b[i] = b[pi];
    }
    for (int i = pi + id; i < d; i += id)
    {
        b[i] = b[pi];
    }
    for (auto i = temp.begin(); i != temp.end(); i++)
    {
        insert(*i);
    }
    return (1);
}

int directory::insert(int n)
{
    int no;
    int depth = gd;
    while (depth >= 0)
    {
        no = hash(n, depth);
        auto j = find(bno.begin(), bno.end(), no);
        if (j != bno.end())
        {
            break;
        }
        else
        {
            depth--;
        }
    }
    int con = b[no]->insert(n);
    if (con == 0)
    {
        split(no);
        insert(n);
    }
    return (1);
}

int directory::search(int n)
{

    for (auto i = bno.begin(); i != bno.end(); i++)
    {
        int j = b[*i]->search(n);
        if (j == 1)
        {
            return (*i);
        }
    }
    return (-1);
}

int directory::remove(int n)
{
    int j = search(n);
    if (j != -1)
    {
        b[j]->remove(n);
        return (1);
    }
    return (0);
}

void directory::display()
{
    cout << gd << endl;
    cout << bno.size() << endl;
    for (auto i = bno.begin(); i != bno.end(); i++)
    {
        int ld = b[*i]->getld();
        cout << b[*i]->getSize() << " ";
        cout << ld << endl;
    }
}

void directory::shrink(void)
{
    int i,flag=1;
    for( i=0 ; i<b.size(); i++ )
    {
        if(b[i]->getld()==gd)
        {
            flag=0;
            return;
        }
    }
    gd--;
    for(i = 0 ; i < 1<<gd ; i++ )
        b.pop_back();
}

void directory::merge(int bucket_no)
{
    int local_depth,pair_index,index_diff,dir_size,i;

    local_depth = b[bucket_no]->getld();
    pair_index = pairIndex(bucket_no,local_depth);
    index_diff = 1<<local_depth;
    dir_size = 1<<gd;

    if( b[pair_index]->getld() == local_depth )
    {
        b[pair_index]->decrdepth();
        delete(b[bucket_no]);
        b[bucket_no] = b[pair_index];
        for( i=bucket_no-index_diff ; i>=0 ; i-=index_diff )
            b[i] = b[pair_index];
        for( i=bucket_no+index_diff ; i<dir_size ; i+=index_diff )
            b[i] = b[pair_index];
    }
}

int main()
{

    int gd, size;
    cin >> gd;
    cin >> size;
    directory b(gd, size);
    int mode;
    int j;

    while (true)
    {
        cin >> mode;
        if (mode == 6)
        {
            break;
        }
        switch (mode)
        {
        case 2:
            cin >> j;
            b.insert(j);
            break;
        case 3:
            cin >> j;
            b.search(j);
            break;
        case 4:
            cin >> j;
            b.remove(j);
            break;
        case 5:
            b.display();
            break;
        }
    }

    return (0);
}
