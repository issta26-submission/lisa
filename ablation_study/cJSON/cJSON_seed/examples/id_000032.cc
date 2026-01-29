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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *it0 = cJSON_CreateString("alpha");
    cJSON *it1 = cJSON_CreateString("beta");
    cJSON *it2 = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, it0);
    cJSON_AddItemToArray(items, it1);
    cJSON_AddItemToArray(items, it2);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("example_document"));

    // step 3: Operate & Validate
    cJSON *replacement = cJSON_CreateString("BETA_REPLACED");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(items, 1, replacement);
    cJSON *fetched_replaced = cJSON_GetArrayItem(items, 1);
    char *replaced_value = cJSON_GetStringValue(fetched_replaced);
    cJSON_AddStringToObject(root, "replaced_name", replaced_value);
    cJSON *flag_item = cJSON_GetArrayItem(items, 2);
    cJSON_bool flag = cJSON_IsTrue(flag_item);
    cJSON_AddItemToObject(root, "flag", cJSON_CreateBool(flag));
    cJSON_AddItemToObject(root, "replaced_result", cJSON_CreateBool(replaced));
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_name_item = cJSON_GetObjectItem(parsed, "replaced_name");
    char *parsed_name = cJSON_GetStringValue(parsed_name_item);
    (void)parsed_name;

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_free(snapshot);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}