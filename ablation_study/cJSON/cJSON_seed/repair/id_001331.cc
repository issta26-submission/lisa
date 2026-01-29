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
//<ID> 1331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *count = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(20.25);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *tag = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "tag", tag);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *new_count = cJSON_CreateNumber((double)size);
    cJSON *count_ptr = cJSON_GetObjectItem(root, "count");
    cJSON_ReplaceItemViaPointer(root, count_ptr, new_count);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    const char *tag_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "tag"));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size % 10));
    ((char *)scratch)[2] = tag_str ? tag_str[0] : 0;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}