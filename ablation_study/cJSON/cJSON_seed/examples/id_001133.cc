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
//<ID> 1133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    double numbers[] = {4.5, -2.25, 7.75};
    cJSON *darr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *label = cJSON_CreateString("dataset");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "values", darr);
    cJSON_AddItemToObjectCS(root, "active_flag", flag);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    cJSON_bool active = cJSON_IsTrue(flag);
    cJSON *first = cJSON_GetArrayItem(darr, 0);
    cJSON *second = cJSON_GetArrayItem(darr, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double computed = v0 + v1 + (double)active;
    cJSON_AddNumberToObject(root, "pair_sum", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}