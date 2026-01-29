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
//<ID> 695
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *strings[] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *string_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "colors", string_array);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "optional", null_item);
    cJSON *active_flag = cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_payload", raw_item);
    cJSON_bool array_check = cJSON_IsArray(string_array);
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "colors_is_array", array_check);
    cJSON_AddBoolToObject(root, "payload_is_raw", raw_check);

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}