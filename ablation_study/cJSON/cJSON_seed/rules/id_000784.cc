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
//<ID> 784
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "[10, 20, 30]";
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *newnum = NULL;
    char *printed = NULL;
    cJSON_bool replace_ok = 0;
    cJSON_bool add_ok = 0;
    double extracted_value = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();

    // step 3: Operate / Validate
    newnum = cJSON_CreateNumber(99.5);
    replace_ok = cJSON_ReplaceItemInArray(parsed, 1, newnum);
    add_ok = cJSON_AddItemToObject(root, "array", parsed);
    printed = cJSON_PrintUnformatted(root);
    extracted_value = cJSON_GetNumberValue(cJSON_GetArrayItem(parsed, 1));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}