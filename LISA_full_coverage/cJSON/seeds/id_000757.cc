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
//<ID> 757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"original\",\"remove_me\":123,\"nested\":{\"inner\":1}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "modified");
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_flag\":true}");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromObject(root, "nested");
    cJSON_AddItemToObject(root, "reattached", detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}