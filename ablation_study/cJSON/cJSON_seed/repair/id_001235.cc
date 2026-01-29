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
//<ID> 1235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num_a = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(items, num_a);
    cJSON *str_ref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToArray(items, str_ref);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "base", 7.0);
    cJSON *num_extra = cJSON_CreateNumber(3.0);
    cJSON_AddItemToObject(meta, "extra", num_extra);

    // step 2: Configure
    cJSON *dup_meta = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "dup_meta", dup_meta);
    cJSON *num_b = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(items, num_b);

    // step 3: Operate and Validate
    cJSON *it0 = cJSON_GetArrayItem(items, 0);
    cJSON *it2 = cJSON_GetArrayItem(items, 2);
    double v0 = cJSON_GetNumberValue(it0);
    double v2 = cJSON_GetNumberValue(it2);
    cJSON_bool is_bool = cJSON_IsBool(flag_false);
    cJSON_bool is_false = cJSON_IsFalse(flag_false);
    cJSON *meta_base = cJSON_GetObjectItem(meta, "base");
    double base_val = cJSON_GetNumberValue(meta_base);
    double total = v0 + v2 + base_val + (double)is_bool + (double)is_false;
    cJSON *total_num = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "total", total_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}