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
//<ID> 1478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"person\":{\"id\":1,\"name\":\"Alice\"},\"active\":true}";
    cJSON *root = cJSON_Parse(json_text);
    cJSON *extra = cJSON_CreateObject();
    cJSON *note = cJSON_CreateString("moved");
    cJSON_AddItemToObject(extra, "note", note);

    // step 2: Configure
    cJSON *moved_person = cJSON_DetachItemFromObjectCaseSensitive(root, "person");
    cJSON_AddItemToObject(extra, "person", moved_person);
    cJSON_AddItemToObject(root, "extra", cJSON_Duplicate(extra, 1));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "active");

    // step 3: Operate & Validate
    cJSON_bool moved_is_object = cJSON_IsObject(moved_person);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON_bool roots_equal = cJSON_Compare(root, dup_root, 1);
    char *printed = cJSON_PrintUnformatted(root);
    (void)moved_is_object;
    (void)roots_equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    cJSON_Delete(extra);

    // API sequence test completed successfully
    return 66;
}