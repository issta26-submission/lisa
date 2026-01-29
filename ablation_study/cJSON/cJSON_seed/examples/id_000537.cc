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
//<ID> 537
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
    cJSON_AddStringToObject(root, "title", "example-list");
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *s0 = cJSON_CreateString("one");
    cJSON *s1 = cJSON_CreateString("two");
    cJSON *s2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_bool is_array_flag = cJSON_IsArray(items);
    cJSON_AddNumberToObject(root, "is_array", (double)is_array_flag);

    // step 3: Operate & Validate
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)size_after);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool parsed_array_flag = cJSON_IsArray(parsed_items);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON_AddBoolToObject(root, "parsed_is_array", parsed_array_flag);
    cJSON_AddBoolToObject(root, "equal", equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)version;
    return 66;
}