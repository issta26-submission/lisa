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
//<ID> 1344
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
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON *num_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw", "{\"embedded\":true}");
    cJSON *flag_item = cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_bool added_str = cJSON_AddItemToArray(arr, str_item);
    cJSON_bool added_num = cJSON_AddItemToArray(arr, num_item);
    (void)added_str;
    (void)added_num;
    (void)flag_item;

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "arr_size", (double)arr_size);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    cJSON_AddRawToObject(root, "printed_raw", buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}