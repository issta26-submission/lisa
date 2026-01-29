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
//<ID> 957
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"items\": [1, 2, 3], \"meta\": {\"name\":\"orig\"}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *array = cJSON_GetObjectItem(root, "items");
    cJSON *new_num = cJSON_CreateNumber(4.0);
    cJSON_InsertItemInArray(array, 1, new_num);
    cJSON *details = cJSON_AddObjectToObject(root, "details");
    cJSON *ref_str = cJSON_CreateStringReference("ref-name");
    cJSON_AddItemToObject(details, "ref", ref_str);
    cJSON *array_ref = cJSON_CreateArrayReference(array);
    cJSON_AddItemToObject(details, "items_ref", array_ref);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}