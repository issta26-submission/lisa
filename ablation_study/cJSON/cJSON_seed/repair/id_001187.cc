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
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    double dnums[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(dnums, 3);
    float fnums[2] = {4.4f, 5.5f};
    cJSON *farr = cJSON_CreateFloatArray(fnums, 2);
    cJSON *container = cJSON_CreateArray();
    cJSON_AddItemToArray(container, darr);
    cJSON_AddItemToArray(container, farr);
    cJSON_AddItemToObject(data, "sets", container);
    cJSON_AddItemToObject(root, "data", data);

    // step 2: Configure
    double first_d = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 0));
    double second_f = cJSON_GetNumberValue(cJSON_GetArrayItem(farr, 1));
    cJSON_AddNumberToObject(root, "first_double", first_d);
    cJSON_AddNumberToObject(root, "second_float", second_f);
    double sum = first_d + second_f;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON *copy = cJSON_Duplicate(root, 1);
    cJSON_AddItemToObject(copy, "copied_note", cJSON_CreateString("original_copy"));
    char *copyout = cJSON_PrintUnformatted(copy);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(copyout);
    cJSON_free(scratch);
    cJSON_Delete(copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}