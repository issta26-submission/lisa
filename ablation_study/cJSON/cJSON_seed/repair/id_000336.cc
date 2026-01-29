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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(list, n1);
    cJSON_AddItemToArray(list, n2);
    cJSON_AddItemToArray(list, n3);
    cJSON_AddItemToObject(root, "items", list);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *new_meta = cJSON_CreateObject();
    cJSON_AddStringToObject(new_meta, "version", "1.1");
    cJSON_AddStringToObject(new_meta, "status", "updated");
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "meta", new_meta);

    // step 3: Operate and Validate
    cJSON *second = cJSON_GetArrayItem(list, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *summary = cJSON_CreateObject();
    cJSON_AddNumberToObject(summary, "second_plus_one", second_val + 1.0);
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *extra = cJSON_CreateString("extra");
    cJSON_AddItemToArray(list, extra);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)replaced;
    return 66;
}