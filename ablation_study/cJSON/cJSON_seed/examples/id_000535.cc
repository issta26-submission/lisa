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
//<ID> 535
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);

    // step 3: Operate & Validate
    cJSON_bool items_is_array = cJSON_IsArray(items);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON_DeleteItemFromArray(items, 1);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;first=%s;is_array=%d;equal=%d",
            version ? version : "unknown",
            first_str ? first_str : "null",
            (int)items_is_array,
            (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}