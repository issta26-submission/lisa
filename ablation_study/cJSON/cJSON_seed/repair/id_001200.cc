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
//<ID> 1200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);

    // step 2: Configure
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(settings, "name", name);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(settings, "enabled", enabled);
    cJSON *threshold = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(settings, "threshold", threshold);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(settings, "maybe", maybe);

    // step 3: Operate and Validate
    cJSON *got_threshold = cJSON_GetObjectItem(settings, "threshold");
    double tval = cJSON_GetNumberValue(got_threshold);
    cJSON *got_name = cJSON_GetObjectItem(settings, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    unsigned char first_char = (unsigned char)name_str[0];
    cJSON *got_maybe = cJSON_GetObjectItem(settings, "maybe");
    cJSON_bool is_null = cJSON_IsNull(got_maybe);
    double final_value = tval + (double)first_char + (double)is_null;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}