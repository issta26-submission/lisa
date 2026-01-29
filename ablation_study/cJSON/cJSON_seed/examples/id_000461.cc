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
//<ID> 461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *note = cJSON_CreateString("initial note");
    cJSON_AddItemToObject(meta, "note", note);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateString("updated note");
    cJSON_ReplaceItemViaPointer(meta, note, replacement);

    // step 3: Operate & Validate
    cJSON *retrieved_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *retrieved_note = cJSON_GetObjectItem(retrieved_meta, "note");
    const char *note_val = cJSON_GetStringValue(retrieved_note);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    char status[128];
    memset(status, 0, sizeof(status));
    sprintf(status, "name=%s;note=%s", name_val ? name_val : "null", note_val ? note_val : "null");
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}