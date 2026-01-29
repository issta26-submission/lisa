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
//<ID> 199
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

    // step 2: Configure
    cJSON *raw = cJSON_CreateRaw("{\"raw\":true}");
    cJSON_AddItemToArray(array, raw);
    cJSON *str1 = cJSON_CreateString("entry_one");
    cJSON_AddItemToArray(array, str1);
    cJSON *str2 = cJSON_CreateString("entry_two");
    cJSON_AddItemToArray(array, str2);

    // step 3: Operate & Validate
    cJSON_bool raw_flag = cJSON_IsRaw(raw);
    const char *status_text = raw_flag ? "contains_raw" : "no_raw";
    cJSON *status = cJSON_CreateString(status_text);
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}