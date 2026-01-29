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
//<ID> 1713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddStringToObject(data, "name", "fuzz_test");
    cJSON *status_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "status", status_true);

    // step 2: Configure
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("two"));
    cJSON_AddItemToArray(items, cJSON_CreateTrue());

    // step 3: Operate and Validate
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_items = cJSON_GetObjectItem(got_data, "items");
    cJSON_bool items_are_array = cJSON_IsArray(got_items);
    int items_count = cJSON_GetArraySize(got_items);
    char *printed = cJSON_PrintUnformatted(root);
    char *prebuf = (char *)cJSON_malloc((size_t)256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(root, prebuf, 256, 0);
    char probe[8];
    probe[0] = printed ? printed[0] : 'X';
    probe[1] = items_are_array ? 'A' : 'N';
    probe[2] = (char)('0' + (items_count % 10));
    probe[3] = prebuf[0] ? prebuf[0] : 'Z';
    (void)probe;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}