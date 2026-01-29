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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *data_obj = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *got_data = (cJSON *)0;
    cJSON *got_num = (cJSON *)0;
    cJSON *double_item = (cJSON *)0;
    char *json_text = (char *)0;
    double original_value = 0.0;
    double doubled_value = 0.0;
    cJSON_bool data_is_object = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    data_obj = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToObject(data_obj, "answer", num_item);
    cJSON_AddItemToObject(root, "data", data_obj);

    // step 4: Operate
    got_data = cJSON_GetObjectItem(root, "data");
    data_is_object = cJSON_IsObject(got_data);
    got_num = cJSON_GetObjectItem(got_data, "answer");
    original_value = cJSON_GetNumberValue(got_num);
    doubled_value = original_value * 2.0;
    double_item = cJSON_CreateNumber(doubled_value);
    cJSON_AddItemToObject(root, "double_answer", double_item);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "data_is_object", data_is_object);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}