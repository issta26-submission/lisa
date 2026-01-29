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
//<ID> 1148
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
    cJSON *parsed_item = NULL;
    const char *parsed_value = NULL;
    char *dyn_str = NULL;
    char *printed = NULL;
    cJSON_bool is_str = 0;
    const char json[] = "{\"external\":\"hello_from_json\"}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "slot", old_item);
    dyn_str = (char *)cJSON_malloc(12);
    memset(dyn_str, 0, 12);
    dyn_str[0] = 'r';
    dyn_str[1] = 'e';
    dyn_str[2] = 'p';
    dyn_str[3] = 'l';
    dyn_str[4] = 'a';
    dyn_str[5] = 'c';
    dyn_str[6] = 'e';
    dyn_str[7] = 'd';
    dyn_str[8] = '\0';
    replacement = cJSON_CreateString(dyn_str);

    // step 3: Operate / Validate
    cJSON_ReplaceItemViaPointer(root, old_item, replacement);
    parsed = cJSON_Parse(json);
    parsed_item = cJSON_GetObjectItem(parsed, "external");
    parsed_value = cJSON_GetStringValue(parsed_item);
    cJSON_AddItemToObject(root, "new_from_parse", cJSON_CreateString(parsed_value));
    printed = cJSON_Print(root);
    is_str = cJSON_IsString(cJSON_GetObjectItem(root, "new_from_parse"));

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_free(dyn_str);
    cJSON_free(printed);
    // API sequence test completed successfully
    return 66;
}