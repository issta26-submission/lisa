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
//<ID> 370
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    int ints_data[3] = {10, 20, 30};
    cJSON *ints_array = cJSON_CreateIntArray(ints_data, 3);
    cJSON_AddItemToObject(root, "ints", ints_array);
    cJSON *raw_item = cJSON_CreateRaw("42");
    cJSON_AddItemToObject(root, "raw_value", raw_item);
    cJSON *number_item = cJSON_CreateNumber(99.75);
    cJSON_AddItemToObject(root, "number", number_item);

    // step 2: Configure
    int array_size = cJSON_GetArraySize(ints_array);
    double number_value = cJSON_GetNumberValue(number_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(stats, "array_size", (double)array_size);
    cJSON_AddNumberToObject(stats, "number_value", number_value);
    cJSON_AddNumberToObject(stats, "raw_flag", (double)raw_is_raw);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON_AddNumberToObject(stats, "prealloc_ok", (double)prealloc_ok);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}