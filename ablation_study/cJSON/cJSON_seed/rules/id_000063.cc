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
//<ID> 63
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
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *num2 = cJSON_CreateNumber(20.0);
    cJSON *flag = cJSON_CreateTrue();
    cJSON *added_direct = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON *retrieved_array = NULL;
    char *printed = NULL;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemReferenceToArray(arr, num2);
    added_direct = cJSON_AddNumberToObject(root, "directNumber", 3.14159);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 3: Operate / Validate
    retrieved_flag = cJSON_GetObjectItem(root, "enabled");
    flag_true = cJSON_IsTrue(retrieved_flag);
    cJSON_AddNumberToObject(root, "enabled_as_number", (double)flag_true);
    retrieved_array = cJSON_GetObjectItem(root, "items");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}