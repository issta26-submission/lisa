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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "myArray", arr);

    // step 2
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *numItem = cJSON_AddNumberToObject(root, "value", 3.14);
    cJSON *nullItem = cJSON_AddNullToObject(root, "none");

    // step 3
    cJSON *repl = cJSON_CreateString("replaced");
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, repl);
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    cJSON_bool is_null = cJSON_IsNull(nullItem);
    cJSON *num_dup = cJSON_CreateNumber(3.14);
    cJSON_bool equal = cJSON_Compare(numItem, num_dup, 1);

    // step 4
    cJSON_Delete(num_dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)replaced;
    (void)got;
    (void)is_null;
    (void)equal;
    return 66;
}