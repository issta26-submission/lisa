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
//<ID> 1047
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
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe");
    cJSON *flag_false = cJSON_AddFalseToObject(root, "flag");
    const char *version = cJSON_Version();

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, n3);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *got_maybe = cJSON_GetObjectItem(root, "maybe");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool items_is_array = cJSON_IsArray(got_items);
    cJSON_bool maybe_is_null = cJSON_IsNull(got_maybe);
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    char *buf = (char *)scratch;
    buf[0] = items_is_array ? 'A' : 'a';
    buf[1] = maybe_is_null ? 'N' : 'n';
    buf[2] = flag_is_false ? 'F' : 'f';
    if (version && version[0]) { /* harmless branch only to access version bytes */ }
    buf[3] = version && version[0] ? version[0] : '?';
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}