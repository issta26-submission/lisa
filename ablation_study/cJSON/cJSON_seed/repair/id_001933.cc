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
//<ID> 1933
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
    cJSON *name_item = cJSON_CreateString("original");
    cJSON_AddItemToObject(meta, "name", name_item);
    double numbers[3] = {3.14, 2.718, -0.5};
    cJSON *dblarr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "numbers", dblarr);
    cJSON *optional = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON_SetValuestring(name_item, "updated_name");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_bool optional_is_null = cJSON_IsNull(optional);
    buffer[0] = (char)('0' + (optional_is_null % 10));
    buffer[1] = printed && printed[0] ? printed[0] : 'X';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}