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
//<ID> 1249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"ok\":true,\"value\":3.14}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *ok_item = NULL;
    cJSON *val_item = NULL;
    cJSON *double_item = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    char buffer[256];
    cJSON_bool prealloc_result = 0;
    cJSON_bool is_true = 0;
    double val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLength(json, json_len);
    ok_item = cJSON_GetObjectItem(root, "ok");
    val_item = cJSON_GetObjectItem(root, "value");

    // step 3: Operate / Validate
    is_true = cJSON_IsTrue(ok_item);
    val = cJSON_GetNumberValue(val_item);
    double_item = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "double", double_item);
    printed1 = cJSON_PrintUnformatted(root);
    prealloc_result = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    printed2 = cJSON_PrintUnformatted(root);
    cJSON_free(printed1);
    cJSON_free(printed2);
    (void)prealloc_result;
    (void)is_true;
    (void)val;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}