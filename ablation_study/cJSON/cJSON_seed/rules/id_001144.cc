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
//<ID> 1144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_item = NULL;
    cJSON *replacement = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_items = NULL;
    cJSON *first_parsed = NULL;
    char *printed_root = NULL;
    char *printed_parsed = NULL;
    char *dyn_str = NULL;
    cJSON_bool is_string = 0;
    const char json[] = "{\"alpha\":\"one\",\"items\":[\"x\",\"y\"]}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(root, "status", old_item);
    dyn_str = (char *)cJSON_malloc((size_t)6);
    memset(dyn_str, 0, 6);
    dyn_str[0] = 'h';
    dyn_str[1] = 'e';
    dyn_str[2] = 'l';
    dyn_str[3] = 'l';
    dyn_str[4] = 'o';
    dyn_str[5] = '\0';
    replacement = cJSON_CreateArray();
    cJSON_AddItemToArray(replacement, cJSON_CreateString("r_elem1"));
    cJSON_AddItemToArray(replacement, cJSON_CreateString(dyn_str));
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString(dyn_str));

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, replacement);
    parsed = cJSON_Parse(json);
    parsed_items = cJSON_GetObjectItem(parsed, "items");
    first_parsed = cJSON_GetArrayItem(parsed_items, 0);
    is_string = cJSON_IsString(first_parsed);
    printed_root = cJSON_Print(root);
    printed_parsed = cJSON_Print(parsed);
    cJSON_free(printed_root);
    cJSON_free(printed_parsed);
    (void)is_string;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dyn_str);
    // API sequence test completed successfully
    return 66;
}