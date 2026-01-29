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
//<ID> 1840
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(84.0));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "label", cJSON_CreateString("example"));

    // step 2: Configure
    cJSON *count_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count_item);
    const char *lib_ver = cJSON_Version();

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_label = cJSON_GetObjectItem(got_meta, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    int size_before = cJSON_GetArraySize(arr);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 0);
    double detached_val = cJSON_GetNumberValue(detached);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = label_str ? label_str[0] : 'L';
    buffer[2] = (char)('0' + (size_before % 10));
    buffer[3] = (char)('0' + (((int)detached_val) % 10));
    (void)lib_ver; (void)label_str; (void)size_before; (void)detached_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}