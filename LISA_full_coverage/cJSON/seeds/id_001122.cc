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
//<ID> 1122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old_name\",\"value\":42.5}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *previous_name = cJSON_SetValuestring(name_item, "updated_name");
    double extra_vals[] = {3.14, 2.718};
    cJSON *double_arr = cJSON_CreateDoubleArray(extra_vals, 2);
    cJSON_AddItemToObject(root, "extras", double_arr);
    cJSON *note_item = cJSON_CreateString("initial_note");
    cJSON_AddItemToObject(root, "note", note_item);
    cJSON_SetValuestring(note_item, "final_note");

    // step 3: Operate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(value_item);
    (void)previous_name;
    (void)value_is_number;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);
    cJSON_free(out);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}