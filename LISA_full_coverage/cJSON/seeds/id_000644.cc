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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"initial\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end, 1);

    // step 2: Configure
    cJSON *flags = cJSON_AddArrayToObject(root, "flags");
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(flags, flag_true);
    cJSON_AddItemToArray(flags, flag_false);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *count = cJSON_CreateNumber(123.0);
    cJSON *status = cJSON_CreateString("ok");
    cJSON_AddItemToObject(meta, "count", count);
    cJSON_AddItemToObject(meta, "status", status);

    // step 3: Operate
    int initial_count = cJSON_GetArraySize(flags);
    cJSON *first_flag = cJSON_GetArrayItem(flags, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first_flag);
    cJSON *has_multiple = cJSON_CreateBool((cJSON_bool)(initial_count > 1));
    cJSON_AddItemToObject(root, "has_multiple_flags", has_multiple);
    cJSON_DeleteItemFromArray(flags, 0);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)first_is_true;
    (void)initial_count;
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}