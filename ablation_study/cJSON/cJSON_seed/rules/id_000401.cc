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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed1 = NULL;
    char *printed2 = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *s_replacement = cJSON_CreateString("replaced");
    cJSON *enabled_item = NULL;
    const char *parse_end = NULL;
    cJSON *parsed = NULL;

    // step 2: Setup / Configure
    enabled_item = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("demo"));
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);

    // step 3: Operate / Validate
    cJSON_ReplaceItemInArray(arr, 0, s_replacement);
    printed1 = cJSON_PrintBuffered(root, 256, 1);
    parsed = cJSON_ParseWithLengthOpts(printed1, (size_t)strlen(printed1), &parse_end, 1);
    printed2 = cJSON_PrintBuffered(parsed, 64, 0);

    // step 4: Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}