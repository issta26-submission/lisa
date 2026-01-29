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
//<ID> 893
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *items = cJSON_AddArrayToObject(meta, "items");
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t);
    cJSON *n = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(items, n);
    cJSON *s = cJSON_CreateString("hello_cjson");
    cJSON_AddItemToArray(items, s);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_GetArrayItem(items, 1);
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON *str_item = cJSON_GetArrayItem(items, 2);
    const char *str_val = cJSON_GetStringValue(str_item);
    int bufsize = 512;
    char *buffer = (char *)cJSON_malloc((size_t)bufsize);
    memset(buffer, 0, (size_t)bufsize);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, bufsize, 0);
    cJSON_AddBoolToObject(meta, "printed_ok", printed_ok);
    (void)num_val;
    (void)str_val;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}