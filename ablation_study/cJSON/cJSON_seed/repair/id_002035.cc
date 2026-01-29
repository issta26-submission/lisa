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
//<ID> 2035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *elem0 = cJSON_CreateString("first");
    cJSON *elem1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(array, elem0);
    cJSON_AddItemToArray(array, elem1);

    // step 2: Configure
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("original"));
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "name", replacement);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed) + 1;
    char *buffer = (char *)cJSON_malloc(printed_len);
    memset(buffer, 0, printed_len);
    memcpy(buffer, printed, printed_len - 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    const char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    char *valbuf = (char *)cJSON_malloc(8);
    memset(valbuf, 0, 8);
    valbuf[0] = name_str[0];
    valbuf[1] = first_str[0];
    valbuf[2] = '\0';
    cJSON_free(valbuf);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}