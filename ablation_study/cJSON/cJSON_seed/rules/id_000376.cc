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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *init_item = NULL;
    cJSON *flag_after_replace = NULL;
    cJSON *new_flag = NULL;
    double init_val = 0.0;
    size_t len = 0;
    cJSON_bool replace_result = 0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "initial", 1.23);
    cJSON_AddTrueToObject(root, "flag");
    cJSON_AddNumberToObject(child, "sub", 42.0);
    cJSON_AddItemToObject(root, "child", child);
    printed = cJSON_Print(root);
    len = strlen(printed);
    buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    strcpy(buffer, printed);
    cJSON_Minify(buffer);
    parsed = cJSON_Parse(buffer);

    // step 3: Operate / Validate
    init_item = cJSON_GetObjectItem(parsed, "initial");
    init_val = cJSON_GetNumberValue(init_item);
    cJSON_AddNumberToObject(parsed, "double", init_val * 2.0);
    new_flag = cJSON_CreateFalse();
    replace_result = cJSON_ReplaceItemInObjectCaseSensitive(parsed, "flag", new_flag);
    flag_after_replace = cJSON_GetObjectItem(parsed, "flag");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}