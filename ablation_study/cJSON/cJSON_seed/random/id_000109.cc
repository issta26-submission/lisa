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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON *new_count = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *detached_arr = (cJSON *)0;
    int array_size = 0;
    double validated_count = 0.0;

    // step 2: Setup - build array and attach to root, add a placeholder count
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, s3);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddNumberToObject(root, "count", 0.0);

    // step 3: Operate - retrieve array, measure size, replace count, detach array
    retrieved = cJSON_GetObjectItem(root, "letters");
    array_size = cJSON_GetArraySize(retrieved);
    new_count = cJSON_CreateNumber((double)array_size);
    cJSON_ReplaceItemInObject(root, "count", new_count);
    detached_arr = cJSON_DetachItemFromObject(root, "letters");
    cJSON_AddItemToObject(root, "letters_backup", detached_arr);

    // step 4: Validate - read back the count and record it
    validated_count = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    cJSON_AddNumberToObject(root, "validated_count", validated_count);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}