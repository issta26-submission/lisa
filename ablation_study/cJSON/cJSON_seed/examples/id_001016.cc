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
//<ID> 1016
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = "{\"name\":\"sensor\",\"values\":[1,2,3],\"meta\":{\"id\":101}}";
    size_t raw_len = sizeof(raw_json) - 1;
    cJSON *root = cJSON_ParseWithLength(raw_json, raw_len);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON *new_id = cJSON_CreateNumber(202.0);
    cJSON *location = cJSON_CreateString("lab");
    cJSON_AddItemToObject(new_meta, "id", new_id);
    cJSON_AddItemToObject(new_meta, "location", location);
    cJSON_AddItemToObject(root, "new_meta", new_meta);

    // step 3: Operate & Validate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *detached = cJSON_DetachItemFromArray(values, 1);
    cJSON_AddItemToObject(root, "detached_value", detached);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_str = cJSON_GetStringValue(name_item);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON *got_id = cJSON_GetObjectItem(new_meta, "id");
    double id_val = cJSON_GetNumberValue(got_id);
    cJSON *computed = cJSON_CreateNumber(id_val + 0.5);
    cJSON_AddItemToObject(root, "computed_id", computed);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}