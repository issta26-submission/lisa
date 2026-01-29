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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_items = NULL;
    cJSON *first_item = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_active = NULL;
    const char *first_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("first"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("second"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(meta, "inner", cJSON_CreateString("value"));
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "active", cJSON_CreateTrue());

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_items = cJSON_GetObjectItem(parsed, "items");
    first_item = cJSON_GetArrayItem(retr_items, 0);
    first_str = cJSON_GetStringValue(first_item);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_active = cJSON_GetObjectItem(parsed, "active");
    buffer[0] = (first_str != NULL) ? first_str[0] : '\0';
    buffer[1] = (retr_active != NULL && cJSON_IsTrue(retr_active)) ? '1' : '0';
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}