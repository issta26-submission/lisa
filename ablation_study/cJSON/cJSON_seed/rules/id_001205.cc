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
//<ID> 1205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *flag = NULL;
    cJSON *num = NULL;
    cJSON *true_item = NULL;
    char *printed = NULL;
    cJSON_bool flag_was_false = 0;
    double num_val = 0.0;
    const char json[] = "{\"flag\": false, \"num\": 3, \"inner\": {\"s\":\"v\"}}";

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    flag = cJSON_GetObjectItem(root, "flag");
    num = cJSON_GetObjectItem(root, "num");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "new_true", true_item);
    cJSON_AddItemReferenceToObject(root, "num_ref", num);

    // step 3: Operate / Validate
    flag_was_false = cJSON_IsFalse(flag);
    num_val = cJSON_GetNumberValue(num);
    printed = cJSON_PrintUnformatted(root);
    (void)flag_was_false;
    (void)num_val;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}