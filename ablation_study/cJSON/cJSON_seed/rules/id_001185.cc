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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *nested = NULL;
    cJSON *flag = NULL;
    cJSON *got_flag = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double replaced_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    flag = cJSON_AddTrueToObject(nested, "flag");
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber(2.0));

    // step 3: Operate / Validate
    got_flag = cJSON_GetObjectItem(nested, "flag");
    replacement = cJSON_CreateNumber(7.5);
    cJSON_ReplaceItemViaPointer(nested, got_flag, replacement);
    replaced_value = cJSON_GetNumberValue(cJSON_GetObjectItem(nested, "flag"));
    printed = cJSON_PrintUnformatted(root);
    (void)replaced_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}