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
//<ID> 418
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *tail = cJSON_CreateString("end");
    cJSON_AddItemToArray(arr, tail);
    int size_after = cJSON_GetArraySize(arr);

    // step 3: Operate & Validate
    cJSON_bool has_maybe = cJSON_HasObjectItem(root, "maybe");
    cJSON *maybe_item = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null = cJSON_IsNull(maybe_item);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "has_maybe=%d|is_null=%d|arr_size=%d", (int)has_maybe, (int)is_null, size_after);
    cJSON *status = cJSON_CreateString(status_buf);
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}