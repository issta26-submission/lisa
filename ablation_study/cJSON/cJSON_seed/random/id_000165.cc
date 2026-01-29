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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *data = (cJSON *)0;
    cJSON *num_answer = (cJSON *)0;
    cJSON *num_pi = (cJSON *)0;
    cJSON *flag_num = (cJSON *)0;
    cJSON *pi_copy = (cJSON *)0;
    cJSON *data_got = (cJSON *)0;
    cJSON *pi_got = (cJSON *)0;
    cJSON_bool data_is_object = (cJSON_bool)0;
    double pi_value = 0.0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    data = cJSON_CreateObject();
    num_answer = cJSON_CreateNumber(42.0);
    num_pi = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    cJSON_AddItemToObject(root, "data", data);
    cJSON_AddItemToObject(data, "answer", num_answer);
    cJSON_AddItemToObject(data, "pi", num_pi);

    // step 4: Operate
    data_got = cJSON_GetObjectItem(root, "data");
    data_is_object = cJSON_IsObject(data_got);
    flag_num = cJSON_CreateNumber((double)data_is_object);
    cJSON_AddItemToObject(root, "data_is_object", flag_num);
    pi_got = cJSON_GetObjectItem(data, "pi");
    pi_value = cJSON_GetNumberValue(pi_got);
    pi_copy = cJSON_CreateNumber(pi_value);
    cJSON_AddItemToObject(root, "pi_copy", pi_copy);

    // step 5: Validate
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}