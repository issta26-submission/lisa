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
//<ID> 69
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
    cJSON *n5 = cJSON_CreateNumber(5.0);
    cJSON *n10 = cJSON_CreateNumber(10.0);
    cJSON *t = cJSON_CreateTrue();
    cJSON *count_item = NULL;
    cJSON *printed_obj = NULL;
    char *printed = NULL;
    double count_value = 0.0;
    double first_value = 0.0;
    double total = 0.0;
    cJSON_bool ref_added = 0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, n5);
    cJSON_AddItemToArray(arr, n10);
    cJSON_AddItemToArray(arr, t);
    ref_added = cJSON_AddItemReferenceToArray(arr, n5);
    cJSON_AddNumberToObject(root, "count", (double)cJSON_GetArraySize(arr));

    // step 3: Operate / Validate
    count_item = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(count_item);
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    is_true = cJSON_IsTrue(t);
    total = count_value + first_value + (double)is_true + (double)ref_added;
    cJSON_AddNumberToObject(root, "total", total);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}