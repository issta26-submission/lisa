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
//<ID> 2412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, str1);
    cJSON_AddItemToObject(root, "data", array);
    cJSON *flag = cJSON_AddTrueToObject(root, "ready");

    // step 2: Configure
    char *printed_root = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed_root && printed_root[0] ? printed_root[0] : '\0';
    buffer[1] = (char)(flag ? 1 : 0);

    // step 3: Operate
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_array = cJSON_GetObjectItem(dup_root, "data");
    cJSON *second_item = cJSON_GetArrayItem(dup_array, 1);
    char *newval = cJSON_SetValuestring(second_item, "world");
    buffer[2] = newval && newval[0] ? newval[0] : '\0';

    // step 4: Validate and Cleanup
    char *printed_dup = cJSON_PrintUnformatted(dup_root);
    buffer[3] = printed_dup && printed_dup[0] ? printed_dup[0] : '\0';
    cJSON_free(printed_root);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}