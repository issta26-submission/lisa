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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_AddNullToObject(meta, "maybe");
    cJSON_AddFalseToObject(meta, "is_ok");
    cJSON_AddItemToArray(data, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(data, cJSON_CreateNumber(3.0));
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);

    // step 3: Operate and Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON *got_maybe = cJSON_GetObjectItem(got_meta, "maybe");
    cJSON *got_is_ok = cJSON_GetObjectItem(got_meta, "is_ok");
    cJSON_bool maybe_is_null = cJSON_IsNull(got_maybe);
    cJSON_bool is_ok_false = cJSON_IsFalse(got_is_ok);
    cJSON *flag_maybe_null = cJSON_CreateBool(maybe_is_null);
    cJSON *flag_is_ok_false = cJSON_CreateBool(is_ok_false);
    cJSON_AddItemToObject(root, "maybe_is_null", flag_maybe_null);
    cJSON_AddItemToObject(root, "is_ok_false", flag_is_ok_false);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = (char)('0' + (int)maybe_is_null);
    ((char *)scratch)[1] = (char)('0' + (int)is_ok_false);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}