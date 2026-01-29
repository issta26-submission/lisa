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
//<ID> 31
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
    cJSON *str_item1 = cJSON_CreateString("alpha");
    cJSON *str_item2 = cJSON_CreateString("beta");
    cJSON *count_item = NULL;
    cJSON *double_item = NULL;
    char *printed = NULL;
    int arr_size = 0;
    double count_value = 0.0;
    double doubled_value = 0.0;

    // step 2: Setup
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, str_item1);
    cJSON_AddItemToArray(arr, str_item2);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Core operations / Validate
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    doubled_value = count_value * 2.0;
    double_item = cJSON_AddNumberToObject(root, "double_count", doubled_value);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}