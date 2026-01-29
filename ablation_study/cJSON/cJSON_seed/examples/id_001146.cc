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
//<ID> 1146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"meta\":{\"count\":2},\"values\":[10,20]}";
    size_t len1 = sizeof(json1) - 1;
    cJSON *root = cJSON_ParseWithLength(json1, len1);
    const char json2[] = "[30,40]";
    cJSON *extra = cJSON_Parse(json2);
    cJSON *null_item = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON_AddItemToObject(root, "nullable", null_item);

    // step 3: Operate & Validate
    cJSON *values_arr = cJSON_GetObjectItem(root, "values");
    cJSON *v0 = cJSON_GetArrayItem(values_arr, 0);
    cJSON *v1 = cJSON_GetArrayItem(values_arr, 1);
    double n0 = cJSON_GetNumberValue(v0);
    double n1 = cJSON_GetNumberValue(v1);
    cJSON *extra0 = cJSON_GetArrayItem(extra, 0);
    double e0 = cJSON_GetNumberValue(extra0);
    double total = n0 + n1 + e0;
    cJSON_AddNumberToObject(root, "sum", total);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}