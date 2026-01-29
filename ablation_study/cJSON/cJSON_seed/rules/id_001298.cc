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
//<ID> 1298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *manual_true = NULL;
    cJSON *detached_item = NULL;
    char *printed = NULL;
    double extracted_number = 0.0;
    cJSON_bool add_result = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    add_result = cJSON_AddItemToObject(root, "number_field", num_item);
    manual_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "manual_true", manual_true);
    cJSON_AddTrueToObject(root, "auto_true");

    // step 3: Operate / Validate
    detached_item = cJSON_DetachItemFromObject(root, "number_field");
    extracted_number = cJSON_GetNumberValue(detached_item);
    printed = cJSON_PrintUnformatted(root);
    (void)add_result;
    (void)extracted_number;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}