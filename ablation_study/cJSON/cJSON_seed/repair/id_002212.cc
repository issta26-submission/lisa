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
//<ID> 2212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *item_num = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(list, item_num);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);

    // step 2: Configure
    double set_result = cJSON_SetNumberHelper(item_num, 42.5);
    cJSON *dup_num = cJSON_Duplicate(item_num, 0);
    cJSON_AddItemToArray(list, dup_num);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = (json && json[2]) ? json[2] : '\0';
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool got_is_bool = cJSON_IsBool(got_flag);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(json);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)flag_is_bool;
    (void)set_result;
    (void)got_is_bool;
    return 66;
}