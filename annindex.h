/*
 * index.h
 *
 *  Created on: 2018年4月28日
 *      Author: xueliang
 */

#ifndef ANN_INDEX_H_
#define ANN_INDEX_H_

#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "IndexFlat.h"
#include "IndexIVF.h"
#include "IndexIVFPQ.h"
#include "index_io.h"
#include "rwlock.hpp"
#include "vector"

#include "IndexIVFFlat.h"

using namespace std;
enum index_type {
    FLATIP = 1,
    FLATL2,
    IVFIP,
    IVFL2,
    IVFPQL2 = 8
};


struct match_info {
    long id;
    float distance;

    match_info(long _id, float _distance) {
        id = _id;
        distance = _distance;
    }
};

struct match_result {
    int query_id;
    int match_count;
    vector<match_info> matches;

    match_result(int _query_id, int _match_count, vector<match_info> _matches) {
        query_id = _query_id;
        match_count = _match_count;
        matches = _matches;
    }
};

class annindex {
public:
    annindex(const std::string index_file);

    ~annindex();

    vector<match_result> search(int query_count, float dis_thresh, int topn, float *feature);

    void index_delete(int delete_count, long *ids);

    void index_add(int add_count, long *ids, float *features);


private:
    faiss::Index *index;
    bool is_ip;//是否内积距离
    rwlock *lock;
};


#endif /* ANN_INDEX_H_ */
