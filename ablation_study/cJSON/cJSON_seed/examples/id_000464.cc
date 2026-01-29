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
//<ID> 464
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
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30);
    char old_name_buf[64];
    memset(old_name_buf, 0, sizeof(old_name_buf));
    cJSON *name_ptr = cJSON_GetObjectItem(person, "name");
    const char *old_name = cJSON_GetStringValue(name_ptr);
    sprintf(old_name_buf, "%s", old_name);

    // step 3: Operate & Validate
    cJSON *new_name_item = cJSON_CreateString("Bob");
    cJSON_ReplaceItemViaPointer(person, name_ptr, new_name_item);
    cJSON *replaced_name_ptr = cJSON_GetObjectItem(person, "name");
    const char *new_name = cJSON_GetStringValue(replaced_name_ptr);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "old=%s|new=%s", old_name_buf, new_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *snapshot_str = cJSON_CreateString(snapshot);
    cJSON_AddItemToObject(root, "snapshot", snapshot_str);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}