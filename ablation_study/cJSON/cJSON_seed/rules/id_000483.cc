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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retrieved_meta = NULL;
    cJSON *greet_item = NULL;
    cJSON *active_item = NULL;
    const char *greet_str = NULL;
    cJSON_bool active_bool = 0;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", "fuzzTest");
    cJSON_AddTrueToObject(root, "active");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "greet", "hello");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_meta = cJSON_GetObjectItem(parsed, "meta");
    greet_item = cJSON_GetObjectItem(retrieved_meta, "greet");
    greet_str = cJSON_GetStringValue(greet_item);
    active_item = cJSON_GetObjectItem(parsed, "active");
    active_bool = cJSON_IsTrue(active_item);
    buffer[0] = greet_str[0];
    buffer[1] = (char)('0' + (int)active_bool);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}