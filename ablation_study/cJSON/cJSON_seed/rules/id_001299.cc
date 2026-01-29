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
//<ID> 1299
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
    cJSON *true_item = NULL;
    cJSON *detached_num = NULL;
    char *printed = NULL;
    double detached_value = 0.0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.5);
    added1 = cJSON_AddItemToObject(root, "number", num_item);
    true_item = cJSON_CreateTrue();
    added2 = cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddTrueToObject(root, "direct_true");

    // step 3: Operate / Validate
    detached_num = cJSON_DetachItemFromObject(root, "number");
    detached_value = cJSON_GetNumberValue(detached_num);
    printed = cJSON_PrintUnformatted(root);
    (void)detached_value;
    (void)added1;
    (void)added2;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_num);
    // API sequence test completed successfully
    return 66;
}