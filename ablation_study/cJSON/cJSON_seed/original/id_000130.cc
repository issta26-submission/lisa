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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names[] = { "one", "two", "three" };
    float values[] = { 1.5f, 2.5f, 3.5f };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *values_arr = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObject(root, "values", values_arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "id", "root1");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *dup_names = cJSON_Duplicate(names_arr, 1);
    cJSON_AddItemToObject(root, "names_copy", dup_names);
    cJSON *detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");
    cJSON_AddItemToArray(values_arr, cJSON_CreateNumber(4.5));
    cJSON_AddStringToObject(detached_meta, "tag", "detached");
    cJSON_AddItemToObject(root, "detached_meta", detached_meta);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int names_count = cJSON_GetArraySize(names_arr);
    cJSON *first_name_item = cJSON_GetArrayItem(names_arr, 0);
    const char *first_name = cJSON_GetStringValue(first_name_item);
    double first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(values_arr, 0));
    cJSON_bool compare_ok = cJSON_Compare(names_arr, dup_names, 1);
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    int summary = (printed ? (int)printed[0] : 0) + names_count + (int)first_value + (int)compare_ok + (first_name ? (int)first_name[0] : 0) + dup_names->type + (int)scratch[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}