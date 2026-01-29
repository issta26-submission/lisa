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
//<ID> 2085
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
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON *str2 = cJSON_CreateString("three");
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *flag = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_true = cJSON_IsTrue(got_flag);
    cJSON *flag_num = cJSON_CreateNumber((double)is_true);
    cJSON_AddItemToObject(root, "flag_as_number", flag_num);
    cJSON *got_detached = cJSON_GetObjectItem(root, "detached");
    double detached_val = cJSON_GetNumberValue(got_detached);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)is_true);
    buffer[2] = (char)('0' + ((int)detached_val % 10));
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}