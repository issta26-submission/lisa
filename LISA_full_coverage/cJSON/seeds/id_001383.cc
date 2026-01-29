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
//<ID> 1383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"items\":[\"one\",\"two\",\"three\"],\"flag\":false} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_GetObjectItem(parsed, "items");
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_dup);
    cJSON *local = cJSON_CreateArray();
    cJSON_AddItemToArray(local, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(local, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(local, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "local", local);
    cJSON_AddItemToObject(root, "truth", cJSON_CreateTrue());
    cJSON *replacement = cJSON_CreateString("beta_replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(local, 1, replacement);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const cJSON *repl_item = cJSON_GetArrayItem(local, 1);
    const char *repl_text = cJSON_GetStringValue(repl_item);
    (void)repl_text;
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}