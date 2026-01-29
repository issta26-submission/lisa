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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_items = NULL;
    cJSON *retr_second = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_enabled = NULL;
    const char *second_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(meta, "note", "metadata");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_items = cJSON_GetObjectItem(parsed, "items");
    retr_second = cJSON_GetArrayItem(retr_items, 1);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_enabled = cJSON_GetObjectItem(parsed, "enabled");
    second_str = cJSON_GetStringValue(retr_second);
    buffer[0] = (char)('0' + (char)(cJSON_IsTrue(retr_enabled) ? 1 : 0));
    buffer[1] = '\0';
    buffer[2] = (second_str != NULL) ? second_str[0] : '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}