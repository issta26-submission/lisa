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
//<ID> 374
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
    size_t len = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *retrieved = NULL;
    cJSON *replacement = NULL;
    double retrieved_num = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "initial", 42.0);
    cJSON_AddTrueToObject(root, "flag");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    len = strlen(printed);
    buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, printed, len + 1);
    cJSON_Minify(buffer);
    parsed = cJSON_Parse(buffer);
    retrieved = cJSON_GetObjectItem(parsed, "initial");
    retrieved_num = cJSON_GetNumberValue(retrieved);
    replacement = cJSON_CreateNumber(retrieved_num + 1.0);
    cJSON_ReplaceItemInObjectCaseSensitive(parsed, "initial", replacement);
    cJSON_AddNumberToObject(parsed, "copy_of_initial", retrieved_num);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}