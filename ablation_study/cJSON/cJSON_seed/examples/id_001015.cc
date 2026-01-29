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
//<ID> 1015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"sensor\",\"values\":[1.5,2.5,3.5],\"meta\":{\"id\":7}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);

    // step 2: Configure
    cJSON *status = cJSON_CreateString("parsed");
    cJSON_AddItemToObject(root, "status", status);
    cJSON *extra = cJSON_CreateObject();
    cJSON *tag = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(extra, "tag", tag);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate & Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_val);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *first = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_copy", first_copy);
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON_AddItemToObject(root, "detached_value", cJSON_CreateNumber(detached_val));
    cJSON_AddItemToObject(root, "detached_item", detached);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}