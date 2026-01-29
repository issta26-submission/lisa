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
//<ID> 1209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *parsed_item = NULL;
    cJSON *true_item = NULL;
    cJSON *obj_ref = NULL;
    char *printed = NULL;
    cJSON_bool item_is_false = 0;
    const char *json = "{\"key\": false}";
    size_t json_len = sizeof("{\"key\": false}") - 1;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLength(json, json_len);
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "always_true", true_item);
    obj_ref = cJSON_CreateObjectReference(parsed);
    cJSON_AddItemToObject(root, "parsed_ref", obj_ref);

    // step 3: Operate / Validate
    parsed_item = cJSON_GetObjectItem(parsed, "key");
    item_is_false = cJSON_IsFalse(parsed_item);
    printed = cJSON_PrintUnformatted(root);
    (void)item_is_false;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}