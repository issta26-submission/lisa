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
//<ID> 484
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
    cJSON *active_item = cJSON_CreateTrue();
    cJSON *parsed = NULL;
    char *printed = NULL;
    char *greet_str = NULL;
    int is_active = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "rootTitle");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddItemToObject(root, "active", active_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    is_active = (int)cJSON_IsTrue(cJSON_GetObjectItem(parsed, "active"));
    greet_str = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(parsed, "meta"), "greet"));
    buffer[0] = (char)('0' + is_active);
    buffer[1] = '\0';
    (void)greet_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}