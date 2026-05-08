#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef struct {
    int dim1, dim2, dim3;
    float* data;
}tensor;

tensor create_tensor(int N, int T, int C) {
    tensor t;
    t.dim1 = N;
    t.dim2 = T;
    t.dim3 = C;
    t.data = (float*)calloc(N * T * C, sizeof(float));
    return t;
}

void delete_tensor(tensor* t) {
    free(t->data);
    t->data = NULL;
}

void Bypass(tensor* src_orig, tensor* src, tensor* output, float* bypass_scale) {
    output->dim1 = src_orig->dim1;      //dim1 -> N
    output->dim2 = src_orig->dim2;      //dim2 -> T
    output->dim3 = src_orig->dim3;      //din3 -> C

    int C = src->dim3;

    for(int i = 0; i < src->dim1 * src->dim2 * src->dim3; i++) {
        int c = i % C;
        
        output->data[i] = src_orig->data[i] + (src->data[i] - src_orig->data[i]) * bypass_scale[c];
    }
}

void load_data(const char* filename, float* data, int size) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Lỗi: Không thể mở file %s\n", filename);
        return;
    }

    for(int i = 0; i < size; i++) {
        while(1) {
            int ch = fgetc(f);
            if(ch == EOF) break;
            
            if((ch >= '0' && ch <= '9') || ch == '-' || ch == '.') {
                ungetc(ch, f);
                break;
            }
        }
        fscanf(f, "%f", &data[i]);
    }

    fclose(f);
}

void print_data(const char* FILENAME, float* data, int size) {
    FILE* f = fopen(FILENAME, "w");
    if(!f) return;

    fprintf(f, "[");
    for(int i = 0; i < size; i++) {
        fprintf(f, "%f", data[i]);
        if(i < size - 1) fprintf(f, ", ");
    }
    fprintf(f, "]\n");
    fclose(f);
}

int main() {
    tensor src = create_tensor(1, 404, 192);
    tensor src_orig = create_tensor(1, 404, 192);
    tensor output = create_tensor(1, 404, 192);
    float* bypass_scale = (float*)calloc(192, sizeof(float));

    load_data("C:\\Users\\admin\\OneDrive\\Desktop\\Zipforner\\CODE C\\encoder_encoders_0_layers_0_bypass_bypass_scale.txt", bypass_scale, 192);
    load_data("C:\\Users\\admin\\OneDrive\\Desktop\\Zipforner\\CODE C\\input_1_list_format.txt", src.data, 192 * 404);
    load_data("C:\\Users\\admin\\OneDrive\\Desktop\\Zipforner\\CODE C\\input_0_list_format.txt", src_orig.data, 192 * 404);

    Bypass(&src_orig, &src, &output, bypass_scale);

    print_data("bypass_scale.txt", bypass_scale, 192);
    print_data("src.txt", src.data, 192 * 404);
    print_data("src_orgin.txt", src_orig.data, 192 * 404);
    print_data("output.txt", output.data, 192 * 404);

    delete_tensor(&src);
    delete_tensor(&src_orig);
    delete_tensor(&output);
    free(bypass_scale);

    return 0;
}