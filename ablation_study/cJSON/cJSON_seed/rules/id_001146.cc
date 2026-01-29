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
//<ID> 1146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *old_name = NULL;
    cJSON *replacement = NULL;
    char *dynbuf = NULL;
    char *printed = NULL;
    const char json[] = "{\"items\":[\"one\",\"two\"]}";
    cJSON *parsed = NULL;
    cJSON *items = NULL;
    cJSON *first = NULL;
    const char *first_val = NULL;
    cJSON *first_item_str = NULL;
    cJSON_bool is_old_str = 0;
    cJSON_bool is_first_str = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    old_name = cJSON_CreateString("original");
    cJSON_AddItemToObject(root, "name", old_name);
    dynbuf = (char *)cJSON_malloc((size_t)6);
    memset(dynbuf, 0, 6);
    dynbuf[0] = 'd';
    dynbuf[1] = 'y';
    dynbuf[2] = 'n';
    dynbuf[3] = 'v';
    dynbuf[4] = '\0';
    replacement = cJSON_CreateString(dynbuf);

    // step 3: Operate / Validate
    is_old_str = cJSON_IsString(old_name);
    cJSON_ReplaceItemViaPointer(root, old_name, replacement);
    printed = cJSON_Print(root);
    parsed = cJSON_Parse(json);
    items = cJSON_GetObjectItem(parsed, "items");
    first = cJSON_GetArrayItem(items, 0);
    is_first_str = cJSON_IsString(first);
    first_val = cJSON_GetStringValue(first);
    first_item_str = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_item", first_item_str);

    // step 4: Cleanup
    cJSON_free(dynbuf);
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}