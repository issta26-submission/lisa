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
//<ID> 1609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *raw_item = NULL;
    cJSON *true_item = NULL;
    cJSON *arr_ref = NULL;
    cJSON *detached = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    raw_item = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(root, "payload", raw_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag", true_item);
    arr_ref = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_ref", arr_ref);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "payload");
    cJSON_AddItemToObject(child, "moved_payload", detached);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
}