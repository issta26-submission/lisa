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
//<ID> 538
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
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "cjson-test");
    cJSON_AddNullToObject(root, "optional");
    cJSON *item1 = cJSON_CreateString("one");
    cJSON *item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(list, item1);
    cJSON_AddItemToArray(list, item2);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(list);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_list = cJSON_GetObjectItem(parsed, "list");
    cJSON_DeleteItemFromArray(list, 0);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;is_array=%d;orig_size=%d;parsed_has_list=%d",
            version ? version : "unknown",
            (int)is_array,
            cJSON_GetArraySize(list),
            parsed_list ? 1 : 0);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}