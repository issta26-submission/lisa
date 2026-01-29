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
//<ID> 1040
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"user\":\"tester\",\"list\":[1,2,3]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *user_item = cJSON_GetObjectItem(root, "user");
    cJSON_SetValuestring(user_item, "tester_modified");
    cJSON *list_item = cJSON_GetObjectItem(root, "list");
    cJSON *out = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(out, "owner", cJSON_CreateString(cJSON_GetStringValue(user_item)));
    cJSON *active_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObjectCS(out, "active", active_bool);
    int array_flag = (int)cJSON_IsArray(list_item);
    cJSON_AddItemToObjectCS(out, "was_array", cJSON_CreateNumber((double)array_flag));

    // step 3: Operate
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);
    char *prebuf = (char *)cJSON_malloc(256);
    memset(prebuf, 0, 256);
    cJSON_PrintPreallocated(out, prebuf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}