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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nums = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    char *printed = NULL;
    char *temp = (char *)cJSON_malloc(16);
    memset(temp, 0, 16);
    int size = 0;
    cJSON *first_item = NULL;
    double first_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(nums, n1);
    cJSON_AddItemToArray(nums, n2);
    cJSON_AddItemToArray(nums, n3);
    cJSON_AddItemToObject(root, "nums", nums);
    cJSON_AddNumberToObject(root, "count", 3.0);
    printed = cJSON_Print(root);

    // step 3: Operate / Validate
    cJSON_Minify(printed);
    size = cJSON_GetArraySize(nums);
    first_item = cJSON_GetArrayItem(nums, 0);
    first_value = cJSON_GetNumberValue(first_item);
    temp[0] = (char)('0' + (size % 10));
    temp[1] = (char)('0' + ((int)first_value % 10));

    // step 4: Cleanup
    cJSON_free(temp);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}