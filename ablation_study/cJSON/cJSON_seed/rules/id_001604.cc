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
//<ID> 1604
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
    cJSON *raw = NULL;
    cJSON *t = NULL;
    cJSON *arrRef = NULL;
    cJSON *detached = NULL;
    cJSON *moved_container = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateArray();
    raw = cJSON_CreateRaw("{\"x\":123}");
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(child, raw);
    cJSON_AddItemToArray(child, t);
    cJSON_AddItemToObject(root, "child", child);
    arrRef = cJSON_CreateArrayReference(child);
    cJSON_AddItemToObject(root, "child_ref", arrRef);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromObject(root, "child");
    moved_container = cJSON_CreateObject();
    cJSON_AddItemToObject(moved_container, "moved", detached);
    cJSON_AddItemToObject(root, "moved_container", moved_container);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}