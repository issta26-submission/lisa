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
//<ID> 953
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"meta\":{\"id\":1},\"items\":[10,20]}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *extra = cJSON_AddObjectToObject(root, "extra");
    cJSON *name_ref = cJSON_CreateStringReference("added_name");
    cJSON_AddItemToObject(extra, "name_ref", name_ref);

    // step 3: Operate
    cJSON *new_num = cJSON_CreateNumber(15.5);
    cJSON_InsertItemInArray(items, 1, new_num);
    cJSON *items_ref = cJSON_CreateArrayReference(items);
    cJSON_AddItemToObject(root, "items_ref", items_ref);

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}