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
//<ID> 1180
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
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToArray(container, darray);
    cJSON_AddItemToArray(container, farray);
    cJSON_AddItemToObject(root, "container", container);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "dcount", (double)cJSON_GetArraySize(darray));
    cJSON_AddNumberToObject(root, "fcount", (double)cJSON_GetArraySize(farray));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double v_d0 = cJSON_GetNumberValue(cJSON_GetArrayItem(darray, 0));
    double v_f0 = cJSON_GetNumberValue(cJSON_GetArrayItem(farray, 0));
    int size_d = cJSON_GetArraySize(darray);
    int size_f = cJSON_GetArraySize(farray);
    double total = v_d0 + v_f0 + (double)size_d + (double)size_f;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}