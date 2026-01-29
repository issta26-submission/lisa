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
//<ID> 1222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"ints\":[1,2,3,4],\"name\":\"example\",\"keep\":true}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *ints = cJSON_GetObjectItem(parsed, "ints");
    cJSON *i0 = cJSON_GetArrayItem(ints, 0);
    cJSON *i1 = cJSON_GetArrayItem(ints, 1);
    cJSON *i2 = cJSON_GetArrayItem(ints, 2);
    cJSON *i3 = cJSON_GetArrayItem(ints, 3);
    int doubled_vals[4];
    doubled_vals[0] = (int)(cJSON_GetNumberValue(i0) * 2.0);
    doubled_vals[1] = (int)(cJSON_GetNumberValue(i1) * 2.0);
    doubled_vals[2] = (int)(cJSON_GetNumberValue(i2) * 2.0);
    doubled_vals[3] = (int)(cJSON_GetNumberValue(i3) * 2.0);
    cJSON *doubled_array = cJSON_CreateIntArray(doubled_vals, 4);
    cJSON_AddItemToObject(root, "doubled", doubled_array);
    cJSON *name_item = cJSON_GetObjectItem(parsed, "name");
    cJSON *name_copy = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "name_copy", name_copy);

    // step 3: Operate & Validate
    cJSON *doubled_in_root = cJSON_GetObjectItem(root, "doubled");
    cJSON *second = cJSON_GetArrayItem(doubled_in_root, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *computed = cJSON_CreateNumber(second_val + 0.5);
    cJSON_AddItemToObject(root, "computed", computed);
    cJSON *keep_detached = cJSON_DetachItemFromObject(parsed, "keep");
    cJSON_AddItemToObject(root, "migrated_keep", keep_detached);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}