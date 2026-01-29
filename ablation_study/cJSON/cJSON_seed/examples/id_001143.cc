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
//<ID> 1143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json1[] = "{\"arr\":[10,20,30],\"name\":\"alpha\"}";
    cJSON *root1 = cJSON_ParseWithLength(json1, sizeof(json1) - 1);
    const char json2[] = "[{\"id\":1},{\"id\":2}]";
    cJSON *root2 = cJSON_Parse(json2);

    // step 2: Configure
    cJSON *container = cJSON_CreateObject();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(container, "parsed_object", root1);
    cJSON_AddItemToObject(container, "parsed_array", root2);
    cJSON_AddItemToObject(container, "empty", null_item);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(root2, 0);
    cJSON *id_item = cJSON_GetObjectItem(first_elem, "id");
    double id0 = cJSON_GetNumberValue(id_item);
    cJSON_AddNumberToObject(container, "first_id", id0);
    cJSON *dup = cJSON_Duplicate(container, 1);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(container);
    // API sequence test completed successfully
    return 66;
}