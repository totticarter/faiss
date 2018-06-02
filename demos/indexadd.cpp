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

    const index_type indextype = index_type::IVFL2;
    const char *code_file = "/Users/liyong/software/vectorindex/security/annindexproject/annindex/data/code_file.data";
    const char *data_file = "/Users/liyong/software/vectorindex/security/data_0";
    const char *index_file = "/Users/liyong/software/vectorindex/security/annindexproject/annindex/data/ivfl2_indexfile.data";

    std::cout << code_file << std::endl;
//    faiss::Index *index = faiss::read_index(code_file, true);
    faiss::Index *index = faiss::read_index(code_file, false);
    std::cout << "d: " << index->d << ", ntotal: " << index->ntotal << ", is_trained: " << index->is_trained << std::endl;

    size_t d;
    size_t nt;
    file_stat(data_file, &d, &nt);
    float *xt = new float[d * nt];
    long int *id_t = new long int[nt];

    fvecs_read(data_file, xt, id_t, d, nt);
    long p = 0;
    while (p < nt) {
        if (nt - p > 10000) {
            index->add_with_ids(10000, xt + p * d, id_t + p);
            p += 10000;
        } else {
            index->add_with_ids(nt - p, xt + p * d, id_t + p);
            p = nt;
        }
    }

    faiss::write_index(index, index_file);
    delete[] xt;
    delete[] id_t;
    return 0;
}
