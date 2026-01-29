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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON *tru = cJSON_CreateTrue();
    cJSON *count_item = NULL;
    cJSON *first_item = NULL;
    char *printed = NULL;
    double count_val = 0.0;
    double first_val = 0.0;
    cJSON_bool tru_flag = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, tru);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    count_item = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(count_item);
    first_item = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_item);
    tru_flag = cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));
    cJSON_AddNumberToObject(root, "total", count_val + first_val + (double)tru_flag);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}