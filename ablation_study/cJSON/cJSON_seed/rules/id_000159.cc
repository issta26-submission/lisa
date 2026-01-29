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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON *name_item = cJSON_CreateString("original");
    cJSON *to_replace = cJSON_CreateString("to_be_replaced");
    cJSON *raw_item = NULL;
    cJSON *replacement = cJSON_CreateString("replaced");
    char *printed = NULL;
    double pi_value = 0.0;
    cJSON_bool added_pi = 0;
    cJSON_bool added_name = 0;
    cJSON_bool added_replace = 0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    added_pi = cJSON_AddItemToObject(root, "pi", pi_item);
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    raw_item = cJSON_AddRawToObject(root, "raw", "{\"rawkey\":true}");
    added_replace = cJSON_AddItemToObject(root, "to_replace", to_replace);

    // step 3: Operate / Validate
    pi_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "pi"));
    replaced = cJSON_ReplaceItemViaPointer(root, to_replace, replacement);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}