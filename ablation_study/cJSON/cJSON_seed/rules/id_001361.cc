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
//<ID> 1361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig = NULL;
    cJSON *rawitem = NULL;
    cJSON *got = NULL;
    char *unprinted = NULL;
    void *prebuf = NULL;
    cJSON_bool replaced = 0;
    cJSON_bool israw = 0;
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    prebuf = cJSON_malloc(256);
    memset(prebuf, 0, 256);
    root = cJSON_CreateObject();
    orig = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "data", orig);
    rawitem = cJSON_CreateRaw("{\"raw_field\":true}");

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObject(root, "data", rawitem);
    got = cJSON_GetObjectItem(root, "data");
    israw = cJSON_IsRaw(got);
    cJSON_AddBoolToObject(root, "is_raw", israw);
    pre_ok = cJSON_PrintPreallocated(root, (char *)prebuf, 256, 0);
    unprinted = cJSON_PrintUnformatted(root);
    (void)replaced;
    (void)pre_ok;

    // step 4: Cleanup
    cJSON_free(unprinted);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}