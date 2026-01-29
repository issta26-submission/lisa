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
//<ID> 1607
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
    cJSON *rawitem = NULL;
    cJSON *trueitem = NULL;
    cJSON *ref = NULL;
    cJSON *detached = NULL;
    char *out = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    rawitem = cJSON_CreateRaw("{\"a\":123,\"b\":true}");
    cJSON_AddItemToObject(child, "raw", rawitem);
    trueitem = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "enabled", trueitem);

    // step 3: Operate / Validate
    ref = cJSON_CreateArrayReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    detached = cJSON_DetachItemFromObject(root, "child");
    cJSON_AddItemToObject(root, "child_detached", detached);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}