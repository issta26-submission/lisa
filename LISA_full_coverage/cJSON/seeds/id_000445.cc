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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"title\":\"example\",\"count\":3}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    const char *title_text = cJSON_GetStringValue(title_item);
    cJSON_AddNullToObject(root, "obsolete");
    cJSON_AddStringToObject(root, "copied_title", title_text);
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "title");
    char *snapshot_after = cJSON_PrintBuffered(root, 128, 0);

    // step 4: Validate & Cleanup
    cJSON *copied_item = cJSON_GetObjectItem(root, "copied_title");
    const char *copied_text = cJSON_GetStringValue(copied_item);
    (void)copied_text;
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}