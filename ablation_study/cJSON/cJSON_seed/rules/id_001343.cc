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
//<ID> 1343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"value\":42,\"active\":false}";
    cJSON *root = NULL;
    cJSON *name = NULL;
    cJSON *value = NULL;
    cJSON *active = NULL;
    cJSON *true_item = NULL;
    cJSON *note_item = NULL;
    cJSON *got_enabled = NULL;
    char *printed = NULL;
    const char *name_str = NULL;
    double value_num = 0.0;
    cJSON_bool add_ok = 0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    name = cJSON_GetObjectItem(root, "name");
    value = cJSON_GetObjectItem(root, "value");
    active = cJSON_GetObjectItem(root, "active");
    name_str = cJSON_GetStringValue(name);
    value_num = cJSON_GetNumberValue(value);
    true_item = cJSON_CreateTrue();
    add_ok = cJSON_AddItemToObject(root, "enabled", true_item);
    note_item = cJSON_CreateString("temporary_note");
    cJSON_AddItemToObject(root, "note", note_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 128, 1);
    got_enabled = cJSON_GetObjectItem(root, "enabled");
    is_true = cJSON_IsTrue(got_enabled);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "note");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)name_str;
    (void)value_num;
    (void)add_ok;
    (void)is_true;
    // API sequence test completed successfully
    return 66;
}