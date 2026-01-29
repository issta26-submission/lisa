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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *newnum = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *retrieved_item = NULL;
    char *printed = NULL;
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));
    cJSON_bool is_num = 0;
    cJSON_bool replaced = 0;
    cJSON_bool added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added = cJSON_AddItemToObject(root, "myarray", arr);
    num0 = cJSON_CreateNumber(1.5);
    num1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);

    // step 3: Operate / Validate
    newnum = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInArray(arr, 0, newnum);
    retrieved_array = cJSON_GetObjectItem(root, "myarray");
    retrieved_item = cJSON_GetArrayItem(retrieved_array, 0);
    is_num = cJSON_IsNumber(retrieved_item);
    printed = cJSON_Print(root);
    if (printed != NULL) strncpy(scratch, printed, sizeof(scratch) - 1);
    (void)added;
    (void)replaced;
    (void)is_num;
    (void)scratch;

    // step 4: Cleanup
    if (printed != NULL) cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}