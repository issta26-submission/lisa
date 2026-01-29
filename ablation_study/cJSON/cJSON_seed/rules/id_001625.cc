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
//<ID> 1625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *num = NULL;
    cJSON *true_item = NULL;
    cJSON *new_obj = NULL;
    cJSON *detached = NULL;
    char *out = NULL;
    const char *json = "{\"obj\": {\"key\": \"value\"}, \"flag\": false}";
    cJSON_bool replaced = 0;

    // step 2: Setup
    root = cJSON_Parse(json);
    obj = cJSON_GetObjectItem(root, "obj");
    num = cJSON_CreateNumber(42);
    cJSON_AddItemToObject(root, "num", num);
    true_item = cJSON_AddTrueToObject(obj, "was_true");

    // step 3: Operate / Validate
    new_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(new_obj, "replaced_key", cJSON_CreateString("new"));
    replaced = cJSON_ReplaceItemViaPointer(root, obj, new_obj);
    detached = cJSON_DetachItemViaPointer(root, num);
    cJSON_AddItemToObject(new_obj, "moved_num", detached);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}