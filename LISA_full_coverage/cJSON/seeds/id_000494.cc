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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"meta\":{}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool items_is_array = cJSON_IsArray(items);
    double first_val = cJSON_GetNumberValue(first);

    // step 2: Configure
    cJSON_AddNullToObject(meta, "nullable");
    cJSON_AddFalseToObject(meta, "flag");
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);
    cJSON *computed = cJSON_CreateNumber(first_val + (double)items_is_array * 10.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate
    cJSON_AddItemReferenceToArray(extras, first);
    cJSON_AddItemReferenceToArray(extras, computed);
    char *snapshot = cJSON_PrintUnformatted(root);
    double extra0 = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 0));
    double extra1 = cJSON_GetNumberValue(cJSON_GetArrayItem(extras, 1));
    double sum = first_val + extra0 + extra1;

    // step 4: Validate & Cleanup
    (void)sum;
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}