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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", cfg);

    // step 2: Configure
    float fnums[3] = {1.1f, 2.2f, 3.3f};
    double dnums[2] = {4.4, 5.5};
    cJSON *float_array = cJSON_CreateFloatArray(fnums, 3);
    cJSON *double_array = cJSON_CreateDoubleArray(dnums, 2);
    cJSON_AddItemToObject(cfg, "floats", float_array);
    cJSON_AddItemToObject(cfg, "doubles", double_array);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *first_f = cJSON_GetArrayItem(float_array, 0);
    cJSON *first_d = cJSON_GetArrayItem(double_array, 0);
    double val_f = cJSON_GetNumberValue(first_f);
    double val_d = cJSON_GetNumberValue(first_d);
    double sum = val_f + val_d;
    cJSON_AddNumberToObject(cfg, "sum_first", sum);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(cfg, "floats");
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}