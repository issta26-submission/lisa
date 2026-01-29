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
//<ID> 1456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *nums_ref = cJSON_CreateArrayReference(nums);
    cJSON_AddItemToObject(root, "numbers_ref", nums_ref);
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "numbers_ref");
    (void)has_ref;
    cJSON *n3 = cJSON_CreateNumber(33.33);
    cJSON_InsertItemInArray(nums, 1, n3);

    // step 3: Operate & Validate
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    int size = cJSON_GetArraySize(nums);
    cJSON *mid = cJSON_GetArrayItem(nums, 1);
    double mid_val = cJSON_GetNumberValue(mid);
    (void)size;
    (void)mid_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}