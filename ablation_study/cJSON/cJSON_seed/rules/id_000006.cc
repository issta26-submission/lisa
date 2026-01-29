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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *raw_item = cJSON_CreateRaw("raw_value");
    // step 2: Configure - add items to object (including required AddNullToObject)
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "data", raw_item);
    cJSON *null_item = cJSON_AddNullToObject(root, "empty");
    // step 3: Operate / Validate - retrieve items and run validators, produce JSON string
    cJSON *got_flag = cJSON_GetObjectItem(root, "flag");
    cJSON *got_data = cJSON_GetObjectItem(root, "data");
    cJSON *got_empty = cJSON_GetObjectItem(root, "empty");
    cJSON_bool is_flag_true = cJSON_IsTrue(got_flag);
    cJSON_bool is_data_raw = cJSON_IsRaw(got_data);
    cJSON_bool is_empty_null = cJSON_IsNull(got_empty);
    char *json_str = cJSON_PrintUnformatted(root);
    // use the validation results to create a simple combined value (data flow)
    int combined_check = (int)is_flag_true + (int)is_data_raw + (int)is_empty_null + (json_str ? 0 : 0);
    (void)combined_check;
    // step 4: Cleanup
    cJSON_free(json_str);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}