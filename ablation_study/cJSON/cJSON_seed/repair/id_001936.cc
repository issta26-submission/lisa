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
//<ID> 1936
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
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *title = cJSON_CreateString("original");
    cJSON_AddItemToObject(meta, "title", title);

    // step 2: Configure
    double nums[3];
    nums[0] = 1.5;
    nums[1] = 2.5;
    nums[2] = 3.5;
    cJSON *darray = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", darray);
    cJSON *null_item = cJSON_AddNullToObject(meta, "optional");
    cJSON_SetValuestring(title, "updated_title");

    // step 3: Operate and Validate
    cJSON *got_optional = cJSON_GetObjectItemCaseSensitive(meta, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    buffer[0] = (char)('0' + (optional_is_null % 10));
    buffer[1] = printed ? printed[0] : 'X';
    buffer[2] = (char)('0' + (cJSON_GetArraySize(darray) % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(meta, "optional");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}