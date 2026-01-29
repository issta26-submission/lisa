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
//<ID> 952
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"arr\": [1, 2, 3], \"info\": \"init\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *array = cJSON_GetObjectItem(root, "arr");
    cJSON *new_number = cJSON_CreateNumber(4.0);
    cJSON_InsertItemInArray(array, 3, new_number);
    cJSON *array_ref = cJSON_CreateArrayReference(array);
    cJSON_AddItemToObject(root, "arr_ref", array_ref);
    cJSON *added_obj = cJSON_AddObjectToObject(root, "added");
    cJSON *str_ref = cJSON_CreateStringReference("ref_str");
    cJSON_AddItemToObject(added_obj, "label", str_ref);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, (char *)buf, 256, 1);

    // step 4: Validate & Cleanup
    cJSON *inserted = cJSON_GetArrayItem(array, 3);
    double val = cJSON_GetNumberValue(inserted);
    cJSON_AddNumberToObject(root, "augmented", val + 10.0);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}