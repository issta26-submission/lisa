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
//<ID> 897
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *payload = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "payload", payload);

    // step 2: Configure
    double dnums[2] = { 3.25, 4.75 };
    float fnums[2] = { 1.5f, 2.5f };
    cJSON *double_arr = cJSON_CreateDoubleArray(dnums, 2);
    cJSON *float_arr = cJSON_CreateFloatArray(fnums, 2);
    cJSON_AddItemToObject(payload, "doubleArray", double_arr);
    cJSON_AddItemToObject(payload, "floatArray", float_arr);

    // step 3: Operate
    cJSON *d_item0 = cJSON_GetArrayItem(double_arr, 0);
    cJSON *f_item1 = cJSON_GetArrayItem(float_arr, 1);
    double d0 = cJSON_GetNumberValue(d_item0);
    double f1 = cJSON_GetNumberValue(f_item1);
    double total = d0 + f1;
    cJSON_AddNumberToObject(payload, "combinedTotal", total);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_DeleteItemFromObject(payload, "floatArray");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}