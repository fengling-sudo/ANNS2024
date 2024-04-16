#include <iostream>
#include "flass.h"
#include <cassert>


float* fvecs_read(const char* fname, size_t* d_out, size_t* n_out) {
    FILE* f;
    errno_t err = fopen_s(&f, fname, "rb");
    if (err != 0 || !f) {
        fprintf(stderr, "could not open %s\n", fname);
        perror("");
        abort();
    }
    int d;
    fread(&d, sizeof(int), 1, f);
    assert((d > 0 && d < 1000000) || !"unreasonable dimension");
    fseek(f, 0, SEEK_SET);
    struct stat st;
#ifdef _MSC_VER
    fstat(_fileno(f), &st);
#else
    fstat(fileno(f), &st);
#endif
    size_t sz = st.st_size;
    assert(sz % ((d + 1) * 4) == 0 || !"weird file size");
    size_t n = sz / ((d + 1) * 4);

    *d_out = d;
    *n_out = n;
    float* x = new float[n * (d + 1)];
    size_t nr = fread(x, sizeof(float), n * (d + 1), f);
    assert(nr == n * (d + 1) || !"could not read whole file");

    for (size_t i = 0; i < n; i++) {
        memmove(x + i * d, x + i * (d + 1) + 1, d * sizeof(float));
    }

    fclose(f);
    return x;
}

int* ivecs_read(const char* fname, size_t* d_out, size_t* n_out) {
    return (int*)fvecs_read(fname, d_out, n_out);
}

void test_init() {

    size_t dim, gt_k, nq, N;
    float* base = fvecs_read("./resource/sift_base.fvecs", &dim, &N);
    float* query = fvecs_read("./resource/sift_query.fvecs", &dim, &nq);
    int* gt = ivecs_read("./resource/sift_groundtruth.ivecs", &gt_k, &nq);

    MinimalStructuralInfo basicinfo;
    basicinfo.max_element_count_per_sub_ = N/4;
    basicinfo.ef_construction_ = 100;
    basicinfo.subNeigh_max_ = 32;
    basicinfo.data_dim = dim;
    basicinfo.max_Subgraph_ = 4;
    basicinfo.max_load_Subgraph_ = 4;
    basicinfo.hyperNeigh_max_ = 30;
    basicinfo.ef_hyper_ = 30;
    basicinfo.res_k = 10;

    flasslib::MyHNSW<int>* mh = new flasslib::MyHNSW<int>(basicinfo);
    size_t data_num[] = { N / 4,N / 4,N / 4,N / 4 };
    mh->buildSubgraph_init(base, data_num, 4);

    size_t topk = 10;

    double recall;
    int cnt = 0;
    double qps;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < nq; ++i) {
        std::unordered_set<int> st(gt + i * gt_k, gt + i * gt_k + topk);
        cout << "当前查询第：" << i << "个点！" << endl;
        std::priority_queue<
            std::pair<dist_type, labeltype>,
            std::vector<std::pair<dist_type, labeltype>>,
            CompareByFirst>
            res_candidates = mh->search(query + i * dim);
        end = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < topk; ++j) {
            labeltype label = 0;
            if (!res_candidates.empty()) {
                label = res_candidates.top().second;
                res_candidates.pop();
            }
            if (st.count(label)) {
                cnt++;
            }
        }
    }

    auto ela = std::chrono::duration<double>(end - start).count();

    recall = (double)cnt / (nq * topk);
    qps = nq / ela;
    
    printf("\tRecall@%d = %.4lf, QPS = %.2lf\n", topk, recall, qps);


}

void test_quant(const void* d) {
    uint8_t* data = (uint8_t*)d;
}


int main()
{
    test_init();
}