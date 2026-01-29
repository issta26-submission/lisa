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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[10,20,30],\"meta\":{\"name\":\"test\"}}";
    size_t len = sizeof(json) - 1;
    const char * parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    cJSON *newobj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newobj, "orig", second_val);
    cJSON_AddStringToObject(newobj, "note", "inserted");

    // step 3: Operate
    cJSON_InsertItemInArray(items, 1, newobj);
    cJSON *inserted = cJSON_GetArrayItem(items, 1);
    cJSON *orig_field = cJSON_GetObjectItemCaseSensitive(inserted, "orig");
    double orig_field_val = cJSON_GetNumberValue(orig_field);
    (void)orig_field_val;
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate and Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}