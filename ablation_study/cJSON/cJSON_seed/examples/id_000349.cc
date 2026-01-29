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
//<ID> 349
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
    cJSON *str_item = cJSON_CreateString("alpha");
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    char count_buf[32];
    sprintf(count_buf, "%d", count);
    cJSON *count_str = cJSON_CreateString(count_buf);
    cJSON_AddItemToObject(root, "count", count_str);
    char print_buffer[256];
    cJSON_bool ok = cJSON_PrintPreallocated(root, print_buffer, (int)sizeof(print_buffer), 1);
    (void)ok;
    cJSON *snapshot = cJSON_CreateString(print_buffer);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}