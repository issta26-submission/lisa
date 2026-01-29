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
//<ID> 1206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json[] = "{\"child\":{\"flag\":false,\"num\":3}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *flag = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    cJSON_bool was_false = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    child = cJSON_GetObjectItem(root, "child");
    flag = cJSON_GetObjectItem(child, "flag");
    was_false = cJSON_IsFalse(flag);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "new_flag", true_item);
    cJSON_AddItemReferenceToObject(root, "child_ref", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    (void)was_false;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
}