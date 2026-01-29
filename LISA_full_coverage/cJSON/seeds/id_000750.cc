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
//<ID> 750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"original\",\"meta\":{\"a\":1,\"b\":\"two\"},\"arr\":[1,2,3],\"remove_me\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *raw_payload = cJSON_CreateRaw("{\"inner\":\"rawval\"}");
    cJSON_AddItemToObject(root, "raw_payload", raw_payload);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "modified");

    // step 3: Operate
    cJSON *detached_meta = cJSON_DetachItemFromObject(root, "meta");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    cJSON_AddItemToObject(root, "meta_reattached", detached_meta);
    char *printed = cJSON_PrintBuffered(root, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}