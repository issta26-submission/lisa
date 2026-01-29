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
//<ID> 955
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"root\":{\"arr\":[1,2]}}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *root_obj = cJSON_GetObjectItem(root, "root");
    cJSON *arr = cJSON_GetObjectItem(root_obj, "arr");
    cJSON *new_num = cJSON_CreateNumber(3.5);
    cJSON_InsertItemInArray(arr, 1, new_num);
    cJSON *meta = cJSON_AddObjectToObject(root_obj, "meta");
    cJSON *str_ref = cJSON_CreateStringReference("generated");
    cJSON_AddItemToObject(meta, "label", str_ref);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root_obj, "arr_ref", arr_ref);

    // step 3: Operate
    char *unfmt = cJSON_PrintUnformatted(root);
    int buf_size = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_size);
    memset(buffer, 0, (size_t)buf_size);
    cJSON_PrintPreallocated(root, buffer, buf_size, 1);

    // step 4: Validate & Cleanup
    cJSON_free(unfmt);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}