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
//<ID> 492
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"flags\":[true],\"meta\":{},\"data\":[]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *flags = cJSON_GetObjectItem(root, "flags");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *data = cJSON_GetObjectItem(root, "data");
    cJSON_bool flags_is_array = cJSON_IsArray(flags);

    // step 2: Configure
    cJSON *added_false = cJSON_AddFalseToObject(meta, "enabled");
    cJSON *added_null = cJSON_AddNullToObject(meta, "reason");
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_AddItemReferenceToArray(extras, first_flag);
    cJSON *note = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "note", note);
    cJSON *computed = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(computed, 10.0 + (double)flags_is_array);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *note_str = cJSON_GetStringValue(note);
    (void)note_str;
    (void)added_false;
    (void)added_null;

    // step 4: Validate & Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}