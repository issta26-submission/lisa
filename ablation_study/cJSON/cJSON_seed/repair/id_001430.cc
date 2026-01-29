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
//<ID> 1430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "version", 1.1);
    cJSON_AddStringToObject(root, "name", "fuzz_test");

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_arr = cJSON_IsArray(got_items);
    cJSON *got_active = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(got_active);
    char *unfmt = cJSON_PrintUnformatted(root);
    size_t len = unfmt ? (size_t)strlen(unfmt) : (size_t)0;
    char *buf = (char *)cJSON_malloc(len + 8);
    memset(buf, 0, len + 8);
    buf[0] = len > 0 ? unfmt[0] : ' ';
    buf[1] = is_arr ? '1' : '0';
    buf[2] = is_false ? 'F' : 'T';

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}