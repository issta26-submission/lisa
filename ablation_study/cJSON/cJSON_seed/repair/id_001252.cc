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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,\"three\"]}";
    cJSON *root = cJSON_ParseWithLength(json, (size_t)(sizeof(json) - 1));
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *third = cJSON_GetArrayItem(items, 2);
    const char *third_str = cJSON_GetStringValue(third);
    cJSON *dup_items = cJSON_Duplicate(items, 1);

    // step 2: Configure
    cJSON *s = cJSON_CreateString("original");
    cJSON_SetValuestring(s, "modified");
    cJSON_AddItemToArray(dup_items, s);
    cJSON_AddItemToObject(root, "dup_items", dup_items);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *mirror = cJSON_CreateString(third_str);
    cJSON_AddItemToObject(root, "mirror", mirror);
    double computed = first_val + (double)(cJSON_IsString(third));
    cJSON *computed_num = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}