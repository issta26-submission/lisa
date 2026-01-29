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
//<ID> 360
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
    cJSON_AddNumberToObject(meta, "scale", 2.0);
    cJSON_AddBoolToObject(meta, "enabled", 1);
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNullToObject(root, "maybe_null");

    // step 2: Configure
    cJSON *inner = cJSON_AddObjectToObject(root, "inner");
    cJSON_AddItemToObject(inner, "link", cJSON_CreateString("connected"));
    cJSON_AddItemToObject(inner, "meta_ref", cJSON_CreateObject()); /* create nested object for completeness */

    // step 3: Operate and Validate
    cJSON *values_ref = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON_bool values_is_array = cJSON_IsArray(values_ref);
    cJSON *v0 = cJSON_GetArrayItem(values_ref, 0);
    cJSON *v1 = cJSON_GetArrayItem(values_ref, 1);
    cJSON *v2 = cJSON_GetArrayItem(values_ref, 2);
    double d0 = cJSON_GetNumberValue(v0);
    double d1 = cJSON_GetNumberValue(v1);
    double d2 = cJSON_GetNumberValue(v2);
    cJSON *scale_item = cJSON_GetObjectItemCaseSensitive(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    double sum = (d0 + d1 + d2) * scale + (double)values_is_array;
    cJSON *result = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "result", result);
    cJSON *appended = cJSON_CreateNumber(sum);
    cJSON_AddItemToArray(values_ref, appended);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}