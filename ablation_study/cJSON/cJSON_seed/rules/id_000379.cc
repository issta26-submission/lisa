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
//<ID> 379
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
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *num_item = NULL;
    cJSON *child_item = NULL;
    cJSON *replaced_item = NULL;
    char *printed = NULL;
    double initial_val = 0.0;
    double replaced_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "initial", 10.0);
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddNumberToObject(child, "sub", 2.5);
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    parsed = cJSON_Parse(printed);
    num_item = cJSON_GetObjectItem(parsed, "initial");
    initial_val = cJSON_GetNumberValue(num_item);
    cJSON_AddNumberToObject(parsed, "doubled", initial_val * 2.0);
    replaced_item = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "child", replaced_item);
    child_item = cJSON_GetObjectItem(parsed, "child");
    replaced_val = cJSON_GetNumberValue(child_item);
    buffer[0] = (char)('0' + (int)initial_val);
    buffer[1] = (char)('0' + (int)replaced_val);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}