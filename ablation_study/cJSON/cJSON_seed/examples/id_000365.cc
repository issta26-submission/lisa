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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const words[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *colors = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "colors", colors);
    cJSON *solo = cJSON_CreateString("solo-color");
    cJSON_AddItemToObject(root, "solo", solo);
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe");

    // step 2: Configure
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool maybe_is_null = cJSON_IsNull(maybe_null);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    sprintf(buffer, "is_object=%d is_null=%d count=%d", (int)root_is_obj, (int)maybe_is_null, cJSON_GetArraySize(colors));
    cJSON *diagnostic = cJSON_CreateString(buffer);
    cJSON_free(buffer);
    cJSON_AddItemToObject(root, "diagnostic", diagnostic);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *snapshot = cJSON_CreateString(printed);
    cJSON_free(printed);
    cJSON_AddItemToObject(root, "snapshot", snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}