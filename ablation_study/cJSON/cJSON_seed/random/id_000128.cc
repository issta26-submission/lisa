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
//<ID> 128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *desc = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *json_text = (char *)0;
    int array_size = 0;
    float nums[3];

    // step 2: Initialize
    root = cJSON_CreateObject();
    desc = cJSON_CreateString("A float array example");
    cJSON_AddStringToObject(root, "title", "example");

    // step 3: Configure - prepare float array and attach strings/numbers
    nums[0] = 1.5f;
    nums[1] = 2.5f;
    nums[2] = 3.5f;
    arr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "description", desc);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "sum", 7.5);
    cJSON_AddNumberToObject(root, "count", 3.0);

    // step 4: Operate & Validate - inspect the array and record validated count
    retrieved = cJSON_GetObjectItem(root, "numbers");
    array_size = cJSON_GetArraySize(retrieved);
    cJSON_AddNumberToObject(root, "validated_count", (double)array_size);

    // step 5: Produce output and Cleanup
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}