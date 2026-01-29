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
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "[1, 2, 3]";
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *replacement = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    double first_value = 0.0;
    double new_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    first_item = cJSON_GetArrayItem(parsed, 0);
    first_value = cJSON_GetNumberValue(first_item);
    new_value = first_value + 10.0;
    replacement = cJSON_CreateNumber(new_value);

    // step 3: Operate / Validate
    cJSON_ReplaceItemInArray(parsed, 1, replacement);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "arr", parsed);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}