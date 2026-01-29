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
//<ID> 91
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
    cJSON_AddStringToObject(meta, "name", "fuzzdataset");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToArray(items, num);
    cJSON *str = cJSON_CreateString("element");
    cJSON_AddItemToArray(items, str);

    // step 3: Operate and Validate
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char *printed = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool prealloc_written = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    int summary = (int)meta_is_obj + (int)flag_is_true + (int)prealloc_written + (int)buffer[0] + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}