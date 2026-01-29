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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(root, "source", "initial_source");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(values, t);
    cJSON_AddStringToObject(meta, "name", "item_meta");
    cJSON_AddItemToObject(meta, "enabled", cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(values, 0);
    cJSON *it1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *name_item = cJSON_GetObjectItem(meta, "name");
    const char *name = cJSON_GetStringValue(name_item);
    cJSON_AddStringToObject(root, "meta_name_copy", name);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}