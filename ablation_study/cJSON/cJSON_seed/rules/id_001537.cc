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
//<ID> 1537
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
    cJSON *num_orig = NULL;
    cJSON *dup_nested = NULL;
    cJSON *num_added = NULL;
    cJSON *retrieved = NULL;
    double retrieved_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    num_orig = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(nested, "orig", num_orig);
    cJSON_AddItemToObject(root, "nested", nested);
    num_added = cJSON_CreateNumber(100.5);
    cJSON_AddItemToObject(root, "value", num_added);

    // step 3: Operate / Validate
    dup_nested = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_dup", dup_nested);
    retrieved = cJSON_GetObjectItem(root, "nested_dup");
    retrieved = cJSON_GetObjectItem(retrieved, "orig");
    retrieved_val = cJSON_GetNumberValue(retrieved);
    (void)retrieved_val;
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nested");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}