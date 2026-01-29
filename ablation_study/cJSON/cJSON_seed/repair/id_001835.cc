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
//<ID> 1835
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
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "label", "example");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "threshold", 2.5);
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON *dup_root = cJSON_Duplicate(root, 1);

    // step 3: Operate and Validate
    char *pretty = cJSON_Print(root);
    cJSON_Minify(pretty);
    int original_count = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));
    cJSON_DeleteItemFromArray(cJSON_GetObjectItem(dup_root, "items"), 0);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = pretty ? pretty[0] : 'X';
    buffer[1] = (char)('0' + (original_count % 10));
    double thr = cJSON_GetNumberValue(cJSON_GetObjectItem(config, "threshold"));
    buffer[2] = (char)('0' + (((int)thr) % 10));
    (void)buffer; (void)thr; (void)original_count;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(buffer);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}