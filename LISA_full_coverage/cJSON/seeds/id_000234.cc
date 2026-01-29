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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num_item = cJSON_CreateNumber(10.5);
    cJSON_AddItemToArray(arr, num_item);
    cJSON *bool_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, bool_item);
    cJSON *total_num = cJSON_AddNumberToObject(root, "total", 100.25);

    // step 2: Configure
    cJSON_bool is_bool = cJSON_IsBool(bool_item);
    cJSON_DeleteItemFromArray(arr, 0);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(arr);
    double total_value = cJSON_GetNumberValue(total_num);
    void *tmp_buf = cJSON_malloc(128);
    memset(tmp_buf, 0, 128);
    int summary = arr_size + (int)is_bool + (int)total_value + (printed ? (int)printed[0] : 0);
    *((int *)tmp_buf) = summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}