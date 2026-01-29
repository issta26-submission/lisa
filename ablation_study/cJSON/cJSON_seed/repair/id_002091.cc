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
//<ID> 2091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "rootname");
    cJSON *arr = cJSON_CreateArray();
    cJSON *first = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, first);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_InsertItemInArray(arr, 0, inserted);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.2");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool ok = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    buffer[0] = name_str[0];
    buffer[1] = (char)('0' + (int)ok);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}