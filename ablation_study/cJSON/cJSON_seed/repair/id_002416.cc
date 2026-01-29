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
//<ID> 2416
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
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate
    cJSON *orig_arr = cJSON_GetObjectItem(root, "data");
    cJSON *second_item = cJSON_GetArrayItem(orig_arr, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    char *buffer = (char *)cJSON_malloc((size_t)32);
    memset(buffer, 0, (size_t)32);
    buffer[0] = second_str && second_str[0] ? second_str[0] : '\0';
    cJSON *dup_arr = cJSON_GetObjectItem(dup_root, "data");
    cJSON *dup_first = cJSON_GetArrayItem(dup_arr, 0);
    cJSON_SetNumberHelper(dup_first, 99.0);

    // step 4: Validate and Cleanup
    char *printed_dup = cJSON_PrintUnformatted(dup_root);
    buffer[1] = printed_dup && printed_dup[0] ? printed_dup[0] : '\0';
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}