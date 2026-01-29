#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dvals[3] = {1.1, 2.2, 3.3};
    float fvals[2] = {4.4f, 5.5f};
    cJSON *darray = cJSON_CreateDoubleArray(dvals, 3);
    cJSON *farray = cJSON_CreateFloatArray(fvals, 2);
    cJSON *mainarr = cJSON_CreateArray();
    cJSON_AddItemToArray(mainarr, darray);
    cJSON_AddItemToArray(mainarr, farray);
    cJSON_AddItemToObject(root, "arrays", mainarr);
    cJSON_AddNumberToObject(root, "initial_value", 42.0);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    ((char *)scratch)[2] = out[2];
    ((char *)scratch)[3] = out[3];

    // step 3: Operate and Validate
    cJSON *arrays_from_root = cJSON_GetObjectItem(root, "arrays");
    int main_size = cJSON_GetArraySize(arrays_from_root);
    cJSON *first_elem = cJSON_GetArrayItem(arrays_from_root, 0);
    cJSON *first_num = cJSON_GetArrayItem(first_elem, 0);
    cJSON *second_num = cJSON_GetArrayItem(first_elem, 1);
    double v1 = cJSON_GetNumberValue(first_num);
    double v2 = cJSON_GetNumberValue(second_num);
    double sum = v1 + v2 + (double)main_size;
    cJSON_AddNumberToObject(root, "computed_sum", sum);
    cJSON *extra = cJSON_CreateNumber(7.7);
    cJSON_AddItemToArray(first_elem, extra);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}