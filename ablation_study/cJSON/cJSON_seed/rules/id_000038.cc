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
//<ID> 38
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
    cJSON *str1 = cJSON_CreateString("alpha");
    cJSON *str2 = cJSON_CreateString("beta");
    cJSON *count_item = NULL;
    cJSON *retrieved = NULL;
    double count_val = 0.0;
    double next_val = 0.0;
    char *printed = NULL;

    // step 2: Setup
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(retrieved);
    next_val = count_val + (double)cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "next", next_val);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}