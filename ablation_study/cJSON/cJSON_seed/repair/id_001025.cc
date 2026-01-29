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
//<ID> 1025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNumberToObject(root, "value", 42.0);
    cJSON_AddNumberToObject(meta, "version", 1.5);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_dup", meta_dup);
    cJSON_AddNumberToObject(root, "computed_base", 7.25);

    // step 3: Operate and Validate
    cJSON *array_item = cJSON_GetObjectItem(root, "array");
    cJSON_bool array_check = cJSON_IsArray(array_item);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(meta_item, "version");
    double version_val = cJSON_GetNumberValue(version_item);
    cJSON_AddNumberToObject(root, "version_copy", version_val * 2.0);
    char *temp_buf = (char *)cJSON_malloc(64);
    memset(temp_buf, 0, 64);
    temp_buf[0] = (array_check ? '1' : '0');
    temp_buf[1] = '\0';

    // step 4: Cleanup
    cJSON_free(temp_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}