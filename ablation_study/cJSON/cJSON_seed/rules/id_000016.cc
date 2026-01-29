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
//<ID> 16
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
    cJSON *name_str = cJSON_CreateString("example");
    cJSON *count_item = NULL;
    cJSON *dup_num = NULL;
    double count_value = 0.0;
    int array_size = 0;
    int validation = 0;
    char *printed = NULL;

    // step 2: Setup
    cJSON_AddItemToObject(root, "name", name_str);
    cJSON_AddItemToObject(root, "items", arr);
    count_item = cJSON_AddNumberToObject(root, "count", 42.0);
    count_value = cJSON_GetNumberValue(count_item);
    dup_num = cJSON_CreateNumber(count_value);
    cJSON_AddItemToArray(arr, dup_num);

    // step 3: Operate/Validate
    array_size = cJSON_GetArraySize(arr);
    validation = (int)count_value + array_size;
    cJSON_AddNumberToObject(root, "validation", (double)validation);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}