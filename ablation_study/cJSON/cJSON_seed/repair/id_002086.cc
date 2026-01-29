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
//<ID> 2086
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
    cJSON_AddItemToObject(root, "items", array);
    cJSON *b_enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToArray(array, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(array, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *b_visible = cJSON_AddBoolToObject(root, "visible", 0);
    cJSON_bool is_enabled = cJSON_IsTrue(b_enabled);
    cJSON_bool is_visible = cJSON_IsTrue(b_visible);
    double flag_sum = (double)is_enabled + 2.0 * (double)is_visible;

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromArray(array, 0);
    double detached_val = cJSON_GetNumberValue(detached);
    cJSON *computed = cJSON_CreateNumber(detached_val + flag_sum);
    cJSON_AddItemToObject(root, "computed", computed);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + ((int)detached_val % 10));
    buffer[2] = (char)('0' + (int)flag_sum);
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}