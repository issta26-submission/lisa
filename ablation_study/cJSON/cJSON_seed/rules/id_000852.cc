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
//<ID> 852
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *detached = NULL;
    cJSON *dup_detached = NULL;
    char *printed = NULL;
    cJSON_bool has_items = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_AddArrayToObject(root, "items");
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(array, num1);
    num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddNumberToObject(root, "other", 7.0);
    has_items = cJSON_HasObjectItem(root, "items");

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(array, 0);
    dup_detached = cJSON_Duplicate(detached, 1);
    equal = cJSON_Compare(detached, dup_detached, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(dup_detached);
    cJSON_Delete(root);
    (void)has_items;
    (void)equal;
    // API sequence test completed successfully
    return 66;
}