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
//<ID> 98
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
    cJSON_AddStringToObject(meta, "name", "test");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(root, "active");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToArray(arr, num);
    cJSON *str = cJSON_CreateString("element");
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    int summary = (int)cJSON_IsTrue(active_item) + (int)cJSON_IsObject(meta_item) + (int)printed[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}