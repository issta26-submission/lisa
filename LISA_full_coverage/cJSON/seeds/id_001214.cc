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
//<ID> 1214
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

    // step 2: Configure
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *null_item = cJSON_CreateNull();
    cJSON *str_item = cJSON_CreateString("initial");
    cJSON_SetValuestring(str_item, "updated");
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddNullToObject(root, "explicit_null");

    // step 3: Operate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    char *strval = cJSON_GetStringValue(third);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Cleanup
    (void)arr_size;
    (void)strval;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}