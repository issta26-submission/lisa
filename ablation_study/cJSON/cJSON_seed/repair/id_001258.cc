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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"name\":\"one\",\"val\":1},{\"name\":\"two\",\"val\":2}],\"meta\":\"original\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *item0 = cJSON_GetArrayItem(items, 0);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    cJSON *note = cJSON_CreateString("note-initial");
    cJSON_SetValuestring(note, "note-updated");
    cJSON_AddItemToArray(dup_items, note);

    // step 3: Operate and Validate
    cJSON *dup_item0 = cJSON_GetArrayItem(dup_items, 0);
    cJSON *name0 = cJSON_GetObjectItem(item0, "name");
    cJSON *name_dup0 = cJSON_GetObjectItem(dup_item0, "name");
    const char *name0_str = cJSON_GetStringValue(name0);
    const char *name_dup0_str = cJSON_GetStringValue(name_dup0);
    cJSON *mirror = cJSON_CreateString(name_dup0_str);
    cJSON_AddItemToObject(root, "mirror0", mirror);
    double val0 = cJSON_GetNumberValue(cJSON_GetObjectItem(item0, "val"));
    double val_dup0 = cJSON_GetNumberValue(cJSON_GetObjectItem(dup_item0, "val"));
    double computed = val0 + val_dup0;
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}