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
//<ID> 1066
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *active = cJSON_AddTrueToObject(root, "active");
    (void)active;
    cJSON *manual_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "manual_flag", manual_flag);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddStringToObject(root, "desc", "example");

    // step 3: Operate and Validate
    cJSON_bool is_obj = cJSON_IsObject(root);
    int arr_size = cJSON_GetArraySize(arr);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (is_obj ? 1 : 0));
    ((char *)scratch)[1] = (char)('0' + (arr_size & 0xF));
    char *flat = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}