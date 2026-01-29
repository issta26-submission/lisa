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
//<ID> 527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"Device\",\"count\":3,\"items\":[{\"id\":1},{\"id\":2}],\"flag\":false}";
    cJSON *root = cJSON_Parse(initial_json);
    cJSON *descriptor = cJSON_CreateString("sensor");
    cJSON_AddItemToObjectCS(root, "descriptor", descriptor);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *set_result = cJSON_SetValuestring(name_item, "Device-42");
    cJSON *items_detached = cJSON_DetachItemFromObject(root, "items");
    cJSON_AddItemToObjectCS(root, "archived_items", items_detached);

    // step 3: Operate
    const char fragment[] = "{\"extra\":true,\"note\":\"imported-note\"}";
    cJSON *imported = cJSON_Parse(fragment);
    cJSON *note_detached = cJSON_DetachItemFromObject(imported, "note");
    cJSON_AddItemToObjectCS(root, "imported_note", note_detached);
    char *name_extracted = cJSON_GetStringValue(name_item);
    char *imported_note_extracted = cJSON_GetStringValue(cJSON_GetObjectItem(root, "imported_note"));

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snaplen = snapshot ? strlen(snapshot) : 0;
    size_t buf_len = snaplen + 32;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    (void)set_result;
    (void)name_extracted;
    (void)imported_note_extracted;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(imported);
    // API sequence test completed successfully
    return 66;
}