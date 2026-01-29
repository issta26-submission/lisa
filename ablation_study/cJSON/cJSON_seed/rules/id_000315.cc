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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "items");
    cJSON *added_true = cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *created_false = cJSON_CreateFalse();
    cJSON *created_num = cJSON_CreateNumber(123.0);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *fake = (cJSON *)cJSON_malloc(sizeof(cJSON));
    memset(fake, 0, sizeof(cJSON));

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, created_false);
    cJSON_AddItemToArray(arr, created_num);
    printed = cJSON_PrintUnformatted(root);
    fake->type = 0x7FFFFFFF; /* make an obviously invalid type value */

    // step 3: Operate / Validate
    cJSON_bool false_check = cJSON_IsFalse(created_false);
    cJSON_bool invalid_check = cJSON_IsInvalid(fake);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    buffer[0] = (char)('0' + (int)false_check);
    buffer[1] = (char)('0' + (int)invalid_check);
    buffer[2] = (char)('0' + (arr_size % 10));
    buffer[3] = (char)('A' + ((int)second_value % 26));
    (void)added_true;
    (void)arr_size;
    (void)second_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_free(fake);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}