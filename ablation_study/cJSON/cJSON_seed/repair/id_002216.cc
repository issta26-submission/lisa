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
//<ID> 2216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    double set_res = cJSON_SetNumberHelper(num, 123.456);
    (void)set_res;
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON *flag2 = cJSON_CreateBool(is_bool);
    cJSON_AddItemToObject(root, "flag2", flag2);

    // step 3: Operate and Validate
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (json && json[1]) ? json[1] : '\0';
    buffer[2] = '\0';
    cJSON *got_num = cJSON_GetObjectItem(root, "num");
    double got_value = cJSON_GetNumberValue(got_num);
    (void)got_value;
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}