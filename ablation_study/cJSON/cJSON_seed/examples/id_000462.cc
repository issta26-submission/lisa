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
//<ID> 462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *backup = cJSON_CreateObject();
    cJSON_AddStringToObject(backup, "name", "Bob");
    cJSON_AddNumberToObject(backup, "age", 40.0);
    cJSON_AddItemToObject(root, "backup", backup);

    // step 3: Operate & Validate
    cJSON *retrieved_person = cJSON_GetObjectItem(root, "person");
    char *orig_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_person, "name"));
    char status_before[128];
    memset(status_before, 0, sizeof(status_before));
    sprintf(status_before, "before=%s", orig_name ? orig_name : "null");
    cJSON *replacement_name = cJSON_CreateString("Charlie");
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(retrieved_person, cJSON_GetObjectItem(retrieved_person, "name"), replacement_name);
    char *after_name = cJSON_GetStringValue(cJSON_GetObjectItem(retrieved_person, "name"));
    char status_after[128];
    memset(status_after, 0, sizeof(status_after));
    sprintf(status_after, "replaced=%d|after=%s", (int)replaced, after_name ? after_name : "null");
    char *snapshot_text = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot_text);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);
    cJSON_free(snapshot_text);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}