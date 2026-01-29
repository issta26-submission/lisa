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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"numbers\":[1.1,2.2,3.3],\"flag\":false}";
    cJSON *root = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool original_flag = cJSON_IsTrue(flag_item);

    // step 2: Configure
    float additional[] = {4.4f, 5.5f, 6.6f};
    cJSON *more_arr = cJSON_CreateFloatArray(additional, 3);
    cJSON *extra_bool = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "more_numbers", more_arr);
    cJSON_AddItemToObject(root, "extra_flag", extra_bool);
    char tmp_buf[32];
    memset(tmp_buf, 0, sizeof(tmp_buf));
    cJSON *empty_str = cJSON_CreateString(tmp_buf);
    cJSON_AddItemToObject(root, "empty", empty_str);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    cJSON_AddItemToArray(more_arr, detached);
    (void)original_flag;

    // step 4: Validate & Cleanup
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}