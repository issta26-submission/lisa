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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float floats[] = {1.1f, 2.2f, 3.3f};
    cJSON *farr = cJSON_CreateFloatArray(floats, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    const char *strings[] = {"one", "two", "three"};
    cJSON *sarr = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", sarr);
    cJSON_AddStringToObject(root, "title", "cJSON API test");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "author", "tester");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *farr_dup = cJSON_Duplicate(farr, 1);
    cJSON_bool compare_equal = cJSON_Compare(farr, farr_dup, 1);
    cJSON_AddBoolToObject(root, "floats_equal", compare_equal);
    cJSON_bool is_array_farr = cJSON_IsArray(farr);
    cJSON_AddBoolToObject(root, "floats_is_array", is_array_farr);

    // step 3: Operate and Validate
    int s_count = cJSON_GetArraySize(sarr);
    cJSON *first_str_item = cJSON_GetArrayItem(sarr, 0);
    const char *first_str = cJSON_GetStringValue(first_str_item);
    cJSON_AddStringToObject(root, "first_string", first_str);
    cJSON *first_float_item = cJSON_GetArrayItem(farr, 0);
    double first_float = cJSON_GetNumberValue(first_float_item);
    cJSON_AddNumberToObject(root, "first_float", first_float);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(farr_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}