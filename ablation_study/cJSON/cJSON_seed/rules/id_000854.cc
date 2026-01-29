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
//<ID> 854
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *detached = NULL;
    cJSON *dup_root = NULL;
    char *printed = NULL;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool has_values = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    num1 = cJSON_CreateNumber(1.23);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(4.56);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate / Validate
    detached = cJSON_DetachItemFromArray(arr, 1);
    dup_root = cJSON_Duplicate(root, 1);
    printed = cJSON_PrintUnformatted(dup_root);
    has_values = cJSON_HasObjectItem(root, "values");
    equal = cJSON_Compare(root, dup_root, 1);
    buffer[0] = (has_values ? '1' : '0');
    (void)buffer;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}