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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *oldChild = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", oldChild);

    // step 2: Configure
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(oldChild, "name", name_item);
    cJSON_AddNumberToObject(oldChild, "value", 123.0);
    cJSON *newChild = cJSON_CreateObject();
    cJSON_AddStringToObject(newChild, "name", "replaced");
    cJSON_AddNumberToObject(newChild, "value", 456.0);

    // step 3: Operate & Validate
    cJSON *retrieved_before = cJSON_GetObjectItem(root, "child");
    const char *before_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_before, "name"));
    char status_buf[128];
    char status_buf2[128];
    memset(status_buf, 0, sizeof(status_buf));
    memset(status_buf2, 0, sizeof(status_buf2));
    sprintf(status_buf, "before=%s", before_name);
    cJSON_ReplaceItemViaPointer(root, retrieved_before, newChild);
    cJSON *retrieved_after = cJSON_GetObjectItem(root, "child");
    const char *after_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_after, "name"));
    sprintf(status_buf2, "%s|after=%s", status_buf, after_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}