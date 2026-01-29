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
//<ID> 1048
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddNullToObject(config, "optional");
    cJSON_AddFalseToObject(config, "flag");
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *s1 = cJSON_CreateString("entry");
    cJSON_AddItemToArray(arr, s1);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 3: Operate and Validate
    cJSON *got_config = cJSON_GetObjectItem(root, "config");
    cJSON *got_flag = cJSON_GetObjectItem(got_config, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    cJSON *flag_check = cJSON_CreateBool(flag_is_false);
    cJSON_AddItemToObject(root, "flag_was_false", flag_check);
    cJSON *got_optional = cJSON_GetObjectItem(got_config, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    cJSON *optional_check = cJSON_CreateBool(optional_is_null);
    cJSON_AddItemToObject(root, "optional_was_null", optional_check);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (int)flag_is_false);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}