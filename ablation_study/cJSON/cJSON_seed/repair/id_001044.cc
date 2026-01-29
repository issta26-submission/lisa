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
//<ID> 1044
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
    cJSON_AddNullToObject(root, "maybe_null");
    cJSON_AddFalseToObject(root, "flag");
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, n1);
    const char *ver = cJSON_Version();
    cJSON *ver_item = cJSON_CreateString(ver);
    cJSON_AddItemToArray(items, ver_item);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *got_null = cJSON_GetObjectItem(root, "maybe_null");
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    void *scratch = cJSON_malloc(128);
    memset(scratch, 0, 128);
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}