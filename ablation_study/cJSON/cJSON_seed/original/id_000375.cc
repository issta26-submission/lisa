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
//<ID> 375
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    int numbers[4] = {10, 20, 30, 40};
    cJSON *root = cJSON_CreateObject();
    cJSON *int_array = cJSON_CreateIntArray(numbers, 4);
    cJSON_AddItemToObject(root, "ints", int_array);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    cJSON *pi_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", pi_item);

    // step 2: Configure
    int array_count = cJSON_GetArraySize(int_array);
    double pi_value = cJSON_GetNumberValue(pi_item);
    double is_raw_flag = (double)cJSON_IsRaw(raw_item);
    cJSON_AddNumberToObject(root, "array_count", (double)array_count);
    cJSON_AddNumberToObject(root, "pi_plus_one", pi_value + 1.0);
    cJSON_AddNumberToObject(root, "raw_flag", is_raw_flag);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_AddNumberToObject(root, "prealloc_ok", (double)prealloc_ok);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}