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
//<ID> 1042
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *empty = cJSON_AddNullToObject(root, "empty");
    cJSON *flag = cJSON_AddFalseToObject(root, "flag");
    const char *version = cJSON_Version();
    cJSON_AddStringToObject(root, "version", version);

    // step 3: Operate and Validate
    cJSON *got_empty = cJSON_GetObjectItem(root, "empty");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool empty_is_null = cJSON_IsNull(got_empty);
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    cJSON *report_empty = cJSON_CreateBool(empty_is_null);
    cJSON *report_flag = cJSON_CreateBool(flag_is_false);
    cJSON_AddItemToObject(root, "empty_is_null", report_empty);
    cJSON_AddItemToObject(root, "flag_is_false", report_flag);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (int)empty_is_null);
    ((char *)scratch)[1] = (char)('0' + (int)flag_is_false);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}