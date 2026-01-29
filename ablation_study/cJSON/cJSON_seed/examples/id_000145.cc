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
//<ID> 145
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("example-name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *optional = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", optional);

    // step 2: Configure
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON *count = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(stats, "count", count);
    double values_arr[3] = {1.0, 2.0, 3.0};
    cJSON *values = cJSON_CreateDoubleArray(values_arr, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Operate & Validate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "name");
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON_bool meta_is_object = cJSON_IsObject(retrieved_meta);
    int values_count = cJSON_GetArraySize(retrieved_values);
    cJSON *first_value = cJSON_GetArrayItem(retrieved_values, 0);
    double first_value_num = cJSON_GetNumberValue(first_value);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, dup, 1);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)has_name;
    (void)meta_is_object;
    (void)values_count;
    (void)first_value_num;
    (void)equal;
    return 66;
}