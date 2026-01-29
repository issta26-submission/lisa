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
//<ID> 194
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
    const char *raw_text = "{\"embedded\":true}";
    cJSON *raw_item = cJSON_CreateRaw(raw_text);
    cJSON_AddItemToArray(array, raw_item);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(array, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(array, s2);
    cJSON *extra_raw = cJSON_CreateRaw("[1,2,3]");
    cJSON_AddItemToObject(root, "extra_raw", extra_raw);

    // step 3: Operate & Validate
    cJSON_bool is_raw = cJSON_IsRaw(raw_item);
    const char *status_opts[2] = { "raw_no", "raw_yes" };
    const char *status_text = status_opts[(int)is_raw];
    cJSON *status_str = cJSON_CreateString(status_text);
    cJSON_AddItemToObject(root, "raw_status", status_str);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}