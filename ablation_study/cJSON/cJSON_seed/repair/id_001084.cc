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
//<ID> 1084
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.0f, 2.5f, -3.25f};
    cJSON *float_arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "values", float_arr);
    cJSON_AddFalseToObject(root, "flag");

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *id = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "id", id);
    cJSON *name = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(child, "name", name);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "list", list);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(list, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(list, n2);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(list, n3);

    // step 3: Operate and Validate
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_list = cJSON_GetObjectItem(got_child, "list");
    cJSON *detached_flag = cJSON_DetachItemFromObject(root, "flag");
    cJSON_DeleteItemFromArray(got_list, 1);
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(got_values);
    double first_list_value = cJSON_GetNumberValue(cJSON_GetArrayItem(got_list, 0));
    char *printed = cJSON_Print(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (values_count & 0xF));
    ((char *)scratch)[1] = (char)('0' + ((int)first_list_value % 10));
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached_flag);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}