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
//<ID> 2419
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    cJSON *dup_item = cJSON_Duplicate(first_item, 0);
    cJSON_AddItemToArray(arr, dup_item);
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    char *printed_dup = cJSON_PrintUnformatted(dup_root);
    buffer[1] = printed_dup && printed_dup[0] ? printed_dup[0] : '\0';
    buffer[2] = (char)(int)cJSON_GetArraySize(arr);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}