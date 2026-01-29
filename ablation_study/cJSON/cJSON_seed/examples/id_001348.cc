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
//<ID> 1348
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
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("element_one");
    cJSON *printed_raw = NULL;
    cJSON_bool printed_ok = 0;

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddRawToObject(root, "embedded", "{\"inner\":123}");
    cJSON_AddFalseToObject(root, "enabled");

    // step 3: Operate & Validate
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "items_count", size_num);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    printed_raw = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "printed_snapshot", printed_raw);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}