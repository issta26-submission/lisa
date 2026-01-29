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
//<ID> 343
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
    cJSON *elem1 = cJSON_CreateString("one");
    cJSON *elem2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToArray(arr, elem2);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *msg = cJSON_CreateString("hello world");
    cJSON_AddItemToObject(root, "message", msg);
    cJSON *raw = cJSON_CreateRaw("{\"inner\":true}");
    cJSON_AddItemToObject(root, "raw", raw);
    cJSON_AddNullToObject(root, "nullable");

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *count = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count);

    // step 3: Operate & Validate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}