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
//<ID> 1279
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
    cJSON *n1 = cJSON_CreateNumber(1.23);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "info", "sample");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first_item = cJSON_GetArrayItem(got_items, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);
    (void)first_invalid;
    char *printed = cJSON_Print(root);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}