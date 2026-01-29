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
//<ID> 470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "cJSON test");
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_block", raw);
    cJSON *item_true = cJSON_CreateTrue();
    cJSON *item_false = cJSON_CreateFalse();
    cJSON *item_str = cJSON_CreateString("element");
    cJSON_AddItemToArray(array, item_true);
    cJSON_AddItemToArray(array, item_false);
    cJSON_AddItemToArray(array, item_str);

    // step 3: Operate & Validate
    cJSON_bool is_t = cJSON_IsTrue(item_true);
    cJSON_bool is_f = cJSON_IsFalse(item_false);
    int size_before = cJSON_GetArraySize(array);
    cJSON_DeleteItemFromArray(array, 1);
    int size_after = cJSON_GetArraySize(array);
    char *snapshot = cJSON_PrintUnformatted(root);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s|true=%d|false=%d|before=%d|after=%d", version ? version : "unknown", (int)is_t, (int)is_f, size_before, size_after);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}