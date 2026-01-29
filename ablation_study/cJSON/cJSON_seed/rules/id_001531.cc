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
//<ID> 1531
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *num_dup = NULL;
    cJSON *retrieved = NULL;
    cJSON *double_item = NULL;
    const char *key_orig = "pi";
    const char *key_dup = "pi_copy";
    const char *key_double = "pi_double";
    double val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, key_orig, num);
    num_dup = cJSON_Duplicate(num, 1);
    cJSON_AddItemToObject(root, key_dup, num_dup);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, key_orig);
    val = cJSON_GetNumberValue(retrieved);
    double_item = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, key_double, double_item);
    cJSON_DeleteItemFromObjectCaseSensitive(root, key_dup);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}