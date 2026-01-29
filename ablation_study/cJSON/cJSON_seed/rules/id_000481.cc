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
//<ID> 481
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
    cJSON *items = cJSON_CreateArray();
    cJSON *trueItem = cJSON_CreateTrue();
    cJSON *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *greet_item = NULL;
    char *greet_value = NULL;
    cJSON_bool is_active = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddItemToObject(root, "active", trueItem);

    // step 3: Operate / Validate
    printed = (cJSON *)cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse((const char *)printed);
    retrieved_meta = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(retrieved_meta, "greet");
    greet_value = cJSON_GetStringValue(greet_item);
    is_active = cJSON_IsTrue(cJSON_GetObjectItem(parsed, "active"));
    buffer[0] = (char)('0' + (int)is_active);
    buffer[1] = '\0';
    (void)greet_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}