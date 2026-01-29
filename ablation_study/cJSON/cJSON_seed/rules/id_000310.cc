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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON *arr = NULL;
    cJSON *invalid_item = (cJSON *)cJSON_malloc(sizeof(cJSON));
    memset(invalid_item, 0, sizeof(cJSON));
    char *printed = NULL;
    cJSON_bool is_false = 0;
    cJSON_bool is_invalid = 0;
    int arr_size = 0;
    double first_num = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "config", cfg);
    cJSON_AddBoolToObject(cfg, "featureA", 1);
    cJSON_AddBoolToObject(cfg, "featureB", 0);
    arr = cJSON_AddArrayToObject(cfg, "values");
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("ok"));

    // step 3: Operate / Validate
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(cfg, "featureB"));
    is_invalid = cJSON_IsInvalid(invalid_item);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);
    first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    (void)is_false;
    (void)is_invalid;
    (void)arr_size;
    (void)first_num;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(invalid_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}