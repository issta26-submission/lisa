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
//<ID> 2219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    double set_res = cJSON_SetNumberHelper(num, 3.14159);
    (void)set_res;
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "name", str);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *elem = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, elem);

    // step 3: Operate and Validate
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_bool = cJSON_IsBool(got_flag);
    (void)is_bool;
    cJSON *got_num = cJSON_GetObjectItem(root, "num");
    double num_val = cJSON_GetNumberValue(got_num);
    (void)num_val;
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    buffer[3] = '\0';
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}