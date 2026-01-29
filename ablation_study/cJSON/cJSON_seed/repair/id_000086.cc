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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *dup_values = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "backup", dup_values);
    cJSON *extra = cJSON_CreateNumber(cJSON_GetNumberValue(n1) + cJSON_GetNumberValue(n2));
    cJSON_AddItemToArray(dup_values, extra);

    // step 3: Operate and Validate
    int size_values = cJSON_GetArraySize(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddItemToObject(summary, "count", cJSON_CreateNumber((double)size_values));
    cJSON_AddItemToObject(summary, "first", cJSON_CreateNumber(first_val));
    cJSON_AddItemToObject(root, "summary", summary);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}