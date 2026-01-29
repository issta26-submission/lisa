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
//<ID> 1083
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float nums[3] = {1.1f, 2.2f, 3.3f};
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON *flag_false = cJSON_AddFalseToObject(root, "active");
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(list, n1);
    cJSON_AddItemToArray(list, n2);
    cJSON_AddItemToArray(list, n3);

    // step 2: Configure
    cJSON *got_list = cJSON_GetObjectItem(root, "list");
    int list_size_before = cJSON_GetArraySize(got_list);
    cJSON *got_floats = cJSON_GetObjectItem(root, "floats");
    int floats_count = cJSON_GetArraySize(got_floats);

    // step 3: Operate and Validate
    cJSON_DeleteItemFromArray(got_list, 1);
    int list_size_after = cJSON_GetArraySize(got_list);
    cJSON *detached_sub = cJSON_DetachItemFromObject(root, "sub");
    cJSON_AddStringToObject(detached_sub, "note", "detached");
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = (char)('0' + (list_size_before & 0xF));
    ((char *)scratch)[1] = (char)('0' + (list_size_after & 0xF));
    ((char *)scratch)[2] = (char)('0' + (floats_count & 0xF));
    cJSON *first_item = cJSON_GetArrayItem(got_list, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    int first_digit = (int)first_val % 10;
    ((char *)scratch)[3] = (char)('0' + (first_digit & 0xF));
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(detached_sub);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}