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
//<ID> 1030
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
    cJSON *name = cJSON_CreateString("device-42");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *values = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_item);

    // step 3: Operate & Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *name_item = cJSON_GetObjectItem(meta_item, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    cJSON *v0 = cJSON_GetArrayItem(values_item, 0);
    cJSON *v1 = cJSON_GetArrayItem(values_item, 1);
    cJSON *v2 = cJSON_GetArrayItem(values_item, 2);
    double d0 = cJSON_GetNumberValue(v0);
    double d1 = cJSON_GetNumberValue(v1);
    double d2 = cJSON_GetNumberValue(v2);
    double sum = d0 + d1 + d2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    cJSON_bool flag_true = cJSON_IsTrue(flag_item);
    cJSON *flag_num = cJSON_CreateNumber((double)flag_true);
    cJSON_AddItemToObject(root, "flag_num", flag_num);
    cJSON_bool are_equal = cJSON_Compare(root, dup, 1);
    cJSON *equal_num = cJSON_CreateNumber((double)are_equal);
    cJSON_AddItemToObject(root, "equal", equal_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}