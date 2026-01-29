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
//<ID> 1046
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
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);
    cJSON *missing = cJSON_AddNullToObject(root, "missing");
    (void)missing;
    cJSON *enabled_false = cJSON_AddFalseToObject(root, "enabled");
    (void)enabled_false;

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToArray(items, s);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON_bool is_array = cJSON_IsArray(got_items);
    int size = cJSON_GetArraySize(got_items);
    cJSON *first = cJSON_GetArrayItem(got_items, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_false = cJSON_IsFalse(got_enabled);
    char *flat = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(32);
    memset(scratch, 0, 32);
    ((char *)scratch)[0] = (char)('0' + (int)is_array);
    ((char *)scratch)[1] = (char)('0' + (int)enabled_is_false);
    ((char *)scratch)[2] = (char)('0' + (int)first_val);
    cJSON_free(flat);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}