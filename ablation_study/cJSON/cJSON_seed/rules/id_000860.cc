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
//<ID> 860
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
    cJSON *str_ref = NULL;
    cJSON *num_item_created = NULL;
    char *printed = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.0);
    arr = cJSON_AddArrayToObject(root, "items");
    str_ref = cJSON_CreateStringReference("referenced_string");
    num_item_created = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, str_ref);
    cJSON_InsertItemInArray(arr, 1, num_item_created);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)(cJSON_GetArraySize(arr) + '0');
    buffer[1] = (char)((int)cJSON_GetNumberValue(num_item_created) % 10 + '0');
    (void)cJSON_GetStringValue(str_ref);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    return 66;
}