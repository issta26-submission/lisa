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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"title\":\"example\",\"count\":3}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *title_item = cJSON_GetObjectItem(root, "title");
    const char *title_text = cJSON_GetStringValue(title_item);

    // step 2: Configure
    cJSON *marker = cJSON_AddNullToObject(root, "marker");
    char empty_buf[32];
    memset(empty_buf, 0, sizeof(empty_buf));
    cJSON *empty_str = cJSON_CreateString(empty_buf);
    cJSON_AddItemToObject(root, "empty", empty_str);
    cJSON *title_copy = cJSON_CreateString(title_text);
    cJSON_AddItemToObject(root, "title_copy", title_copy);

    // step 3: Operate
    char *snapshot = cJSON_PrintBuffered(root, 256, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "title");
    const char *copied_title = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title_copy"));
    (void)copied_title;
    (void)marker;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}