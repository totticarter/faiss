#include <iostream>
#include <sys/stat.h>
#include "../AutoTune.h"
#include "../MetaIndexes.h"
#include "../IndexFlat.h"
#include "../IndexIVFFlat.h"
#include "../index_io.h"
#include "../annindex.h"

using namespace std;

void file_stat(const char *fname, size_t *d_out, size_t *n_out) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        fprintf(stderr, "could not open %s\n", fname);
        perror("");
        abort();
    }
    int d;
    int read_length = fread(&d, 1, sizeof(int), f);
    cout << "in file_stat, d is: " << d << endl;
    assert((d > 0 && d < 1000000) || !"unreasonable dimension");
    fseek(f, 0, SEEK_SET);
    struct stat st;
    fstat(fileno(f), &st);
    size_t sz = st.st_size - 4;
    assert(sz % ((d + 2) * 4) == 0 || !"weird file size");
    size_t n = sz / ((d + 2) * 4);
    *d_out = d;
    *n_out = n;
    fclose(f);
}


void fvecs_read(const char *fname, float *data, long int *id, size_t d,
                size_t n) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        fprintf(stderr, "could not open %s\n", fname);
        perror("");
        abort();
    }
    fseek(f, 4, SEEK_SET);

    size_t p = 0;
    int batchsize = 100000;
    float *x = new float[batchsize * (d + 2)];

    while (p < n) {
        int read_count = batchsize;
        if (read_count + p > n) {
            read_count = n - p;
        }
        size_t nr = fread(x, sizeof(float), read_count * (d + 2), f);
        assert(nr == read_count * (d + 2) || !"read file error!");
        for (size_t i = 0; i < read_count; i++) {
            memcpy(data + (i + p) * d, x + i * (d + 2) + 2, d * sizeof(float));

            id[i + p] = *(long int *) (x + i * (d + 2));;
        }
        p += read_count;
    }
    delete[] x;
    fclose(f);

}



int main(int argc, char **argv) {

    index_type indextype = index_type::IVFPQL2;//index_type::FLATL2;//index_type::IVFL2;IVFPQL2
    const char *data_file = "/Users/liyong/software/vectorindex/security/data_0";
    const char *code_file = "/Users/liyong/software/vectorindex/security/annindexproject/annindex/data/code_file.data";
    int nlist = 100;
    //读取原始数据

    size_t d;
    size_t nt;
    file_stat(data_file, &d, &nt);
    std::cout << "d is: " << d << ", nt is: " << nt << std::endl;
    float *xt = new float[d * nt];
    long int *id_t = new long int[nt];
    fvecs_read(data_file, xt, id_t, d, nt);


    if (indextype == index_type::FLATIP || indextype == index_type::FLATL2) {//flat
        faiss::MetricType metric;
        if (indextype == index_type::FLATIP) {
            metric = faiss::MetricType::METRIC_INNER_PRODUCT;
        } else {
            metric = faiss::MetricType::METRIC_L2;
        }
        faiss::Index *index = faiss::index_factory((int) d, "IDMap,Flat", metric);
        faiss::write_index(index, code_file);
    } else if (indextype == index_type::IVFL2 || indextype == index_type::IVFIP) {//ivf

        faiss::IndexFlatL2 quantizer(d);
        faiss::MetricType metric;
        if (indextype == index_type::IVFIP) {
            metric = faiss::MetricType::METRIC_INNER_PRODUCT;
        } else {
            metric = faiss::MetricType::METRIC_L2;
        }
        faiss::IndexIVFFlat index(&quantizer, d, nlist, metric);
        assert(!index.is_trained);
        index.train(nt, xt);
        index.quantizer = 0x0;
        assert(index.is_trained);
        faiss::write_index(&index, code_file);

    } else if (indextype == index_type::IVFPQL2) {
        int nlist = 100;
        int nseg = 4;
        int nbits = 4;

        faiss::IndexFlatL2 quantizer(d);       // the other index
        faiss::IndexIVFPQ index(&quantizer, d, nlist, nseg, nbits);
        index.train(nt, xt);
        faiss::write_index(&index, code_file);

    }
    delete[] xt;
    delete[] id_t;
    return 0;
}
