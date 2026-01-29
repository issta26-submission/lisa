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
//<ID> 74
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
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    float nums[3];
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = 3.5f;
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", farr);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *tag = cJSON_CreateString("extra");
    cJSON_AddItemToObject(root, "tag", tag);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char *printed_buf = cJSON_PrintBuffered(root, 256, 1);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool arr_is_obj = cJSON_IsObject(farr);
    int arr_size = cJSON_GetArraySize(farr);
    cJSON *first = cJSON_GetArrayItem(farr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, dup, 1);
    char *dup_str = cJSON_Print(dup);
    (void)root_is_obj;
    (void)arr_is_obj;
    (void)arr_size;
    (void)first_val;
    (void)same;
    (void)printed;
    (void)printed_buf;
    (void)dup_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_buf);
    cJSON_free(dup_str);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    // API sequence test completed successfully
    return 66;
}